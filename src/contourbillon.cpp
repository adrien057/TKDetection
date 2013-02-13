#include "inc/contourbillon.h"

#include "inc/contourslice.h"
#include "inc/curvaturehistogram.h"
#include "inc/nearestpointshistogram.h"

ContourBillon::ContourBillon()
{
}

ContourBillon::~ContourBillon()
{
}

const QVector<ContourSlice> &ContourBillon::contourSlices() const
{
	return _contourSlices;
}

const ContourSlice &ContourBillon::contourSlice( const uint &sliceIndex ) const
{
	Q_ASSERT_X( sliceIndex<static_cast<uint>(_contourSlices.size()) , "ContourCurveBillon::contour", "sliceIndex out of bounds" );
	return _contourSlices[sliceIndex];
}

void ContourBillon::clear()
{
	_contourSlices.clear();
}

bool ContourBillon::isEmpty()
{
	return _contourSlices.isEmpty();
}

void ContourBillon::compute( Billon &resultBillon, const Billon &billon, const int &intensityThreshold, const int &blurredSegmentThickness,
							 const int &smoothingRadius, const int &curvatureWidth, const int &minimumOriginDistance )
{
	const uint &nbSlices = billon.n_slices;
	resultBillon = billon;
	_contourSlices.resize(nbSlices);
	for ( uint k=0 ; k<nbSlices ; ++k )
	{
		qDebug() << QString("Calcul de la coupe de contour : %1/%2").arg(k+1).arg(nbSlices);
		_contourSlices[k].compute( resultBillon.slice(k), billon.slice(k), billon.pithCoord(k), intensityThreshold,
								   blurredSegmentThickness, smoothingRadius, curvatureWidth, minimumOriginDistance );
	}
}