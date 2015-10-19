#include "inc/segmentation/ellipseradiihistogram.h"

#include "inc/billon.h"
#include "inc/segmentation/pithprofile.h"
#include "inc/lowess.h"

#include <QtGlobal>

EllipseRadiiHistogram::EllipseRadiiHistogram()
{
}

EllipseRadiiHistogram::~EllipseRadiiHistogram()
{
}

const EllipticalAccumulationHistogram & EllipseRadiiHistogram::ellipticalHistogram( const int &sliceIndex ) const
{
	Q_ASSERT_X( sliceIndex<_ellipticalHistograms.size(), QObject::tr("ellipticalHistogram( uint sliceIndex )").toStdString().c_str(), QObject::tr("sliceIndex trop grand").toStdString().c_str() );
	return _ellipticalHistograms[sliceIndex];
}

void EllipseRadiiHistogram::construct( const Billon &tangentialBillon, const PithProfile &knotPithProfile,
										   const qreal & lowessBandWidth, const uint &smoothingRadius, const qreal &iqrCoeff,
										   const uint &percentageOfFirstValidSlicesToExtrapolate, const uint &percentageOfLastValidSlicesToExtrapolate  )
{
	const uint &nbSlices = tangentialBillon.n_slices;

	this->clear();
	_ellipticalHistograms.clear();

	if ( !nbSlices ) return;

	this->resize(nbSlices);
	_ellipticalHistograms.resize(nbSlices);

//	const int &firstValidSliceIndex = validSlices.min();
//	const int &lastValidSliceIndex = validSlices.max();

	qreal ellipticityRate;
	for ( uint k=0 ; k<nbSlices ; ++k )
	{
		EllipticalAccumulationHistogram &ellipticalHistogram = _ellipticalHistograms[k];
		ellipticityRate = (tangentialBillon.voxelWidth()/tangentialBillon.voxelHeight()) / knotPithProfile[k];
		ellipticalHistogram.construct( tangentialBillon.slice(k), tangentialBillon.pithCoord(k), ellipticityRate,
									   smoothingRadius, k/static_cast<qreal>(nbSlices) );
		(*this)[k] = ellipticalHistogram.detectedRadius();
	}

	extrapolation(tangentialBillon.validSlices(),percentageOfFirstValidSlicesToExtrapolate,percentageOfLastValidSlicesToExtrapolate);

	// LOESS
	// Careful : lowessBandWidth must be 0 <= lowessBandWidth <= 1
	if ( !qFuzzyIsNull(lowessBandWidth) )
	{
		QVector<qreal> residus, interpolatedValues;
		Lowess lowess(lowessBandWidth);
		lowess.compute( *this, interpolatedValues, residus );
		outlierInterpolation( residus, iqrCoeff );
		lowess.compute( interpolatedValues, *this, residus );
	}
//	else
//		_lowessData = *this;
}

void EllipseRadiiHistogram::extrapolation( const Interval<uint> &validSlices, const uint &percentageOfFirstValidSlicesToExtrapolate,
											   const uint &percentageOfLastValidSlicesToExtrapolate )
{
	const int size = this->size();

	if ( !size || !validSlices.width() ) return;

	const int firstValidSliceIndex = validSlices.min()+validSlices.count()*(percentageOfFirstValidSlicesToExtrapolate/100.);
	const int lastValidSliceIndex = validSlices.max()-validSlices.count()*(percentageOfLastValidSlicesToExtrapolate/100.);

	qreal compareZero, compareOne, compareTwo;

	compareZero = (*this)[firstValidSliceIndex];
	compareOne = (*this)[firstValidSliceIndex+1];
	compareTwo = (*this)[firstValidSliceIndex+2];
	const int indexFirstValid = compareZero < compareOne ?
									( compareOne < compareTwo ? 1
															  : (compareZero < compareTwo ? 2 : 0))
								  : ( compareZero < compareTwo ? 0
															   : (compareOne < compareTwo ? 2 : 1));

	compareZero = qAbs(((*this)[lastValidSliceIndex] - (*this)[lastValidSliceIndex-2]));
	compareOne = qAbs(((*this)[lastValidSliceIndex-1] - (*this)[lastValidSliceIndex-3]));
	compareTwo = qAbs(((*this)[lastValidSliceIndex-2] - (*this)[lastValidSliceIndex-4]));
	const int indexLastValid = compareZero < compareOne ?
								   ( compareZero < compareTwo ? 0 : 2)
								 : ( compareOne < compareTwo ? 1 : 2 );


	const qreal firstValidValueIncrement = (*this)[firstValidSliceIndex+indexFirstValid]/static_cast<qreal>(firstValidSliceIndex+indexFirstValid);
	//const qreal lastValidIncrement = ((*this)[lastValidSliceIndex-indexLastValid] - (*this)[lastValidSliceIndex-2-indexLastValid])/2.;

	int k;
	for ( k=0 ; k<firstValidSliceIndex+indexFirstValid; ++k )
	{
		(*this)[k] = firstValidValueIncrement*static_cast<qreal>(k);
	}

	for ( k=lastValidSliceIndex+1-indexLastValid ; k<size; ++k )
	{
		(*this)[k] = (*this)[k-1];// + lastValidIncrement;
	}
}

void EllipseRadiiHistogram::outlierInterpolation( const QVector<qreal> &residus, const qreal &iqrCoeff )
{
	const int &size = this->size();

	QVector<qreal> sortedResidus(residus);
	qSort(sortedResidus);

	const qreal &q1 = sortedResidus[size/4];
	const qreal &q3 = sortedResidus[3*size/4];

	const Interval<qreal> inlierInterval( q1-iqrCoeff*(q3-q1), q3+iqrCoeff*(q3-q1) );

	int startSliceIndex, newK, startSliceIndexMinusOne;
	qreal interpolationStep, currentInterpolatedRadius;

	for ( int k=1 ; k<size-1 ; ++k )
	{
		if ( !inlierInterval.containsOpen(residus[k]) )
		{
			startSliceIndex = k++;
			startSliceIndexMinusOne = startSliceIndex-1;

			while ( k<size-1 && !inlierInterval.containsOpen(residus[k]) ) ++k;
			k = qMin(k,size-2);

//			qDebug() << "Outlier interpolation [" << startSliceIndex << ", " << k << "]";

			interpolationStep = ((*this)[k] - (*this)[startSliceIndexMinusOne]) / static_cast<qreal>( k-startSliceIndexMinusOne );
			currentInterpolatedRadius = (*this)[startSliceIndexMinusOne] + interpolationStep;
			for ( newK = startSliceIndex ; newK < k ; ++newK, currentInterpolatedRadius += interpolationStep )
			{
				(*this)[newK] = currentInterpolatedRadius;
			}
		}
	}
}
