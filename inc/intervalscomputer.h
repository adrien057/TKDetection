#ifndef INTERVALSCOMPUTER_H
#define INTERVALSCOMPUTER_H

#include <QVector>
#include "inc/interval.h"
#include "inc/intervalscomputerdefaultparameters.h"

namespace IntervalsComputer
{
	QVector<qreal> meansSmoothing( const QVector<qreal> &hist, int maskRadius = DEFAULT_MASK_RADIUS, bool loop = false );
	QVector<qreal> gaussianSmoothing( const QVector<qreal> &hist, int maskRadius = DEFAULT_MASK_RADIUS , bool loop = false );
	QVector<int> maximumsComputing( const QVector<qreal> &hist, qreal minimumThreshold, int minimumWidthOfNeighborhood = DEFAULT_MINIMUM_WIDTH_OF_NEIGHBORHOOD, bool loop = false );
	QVector<Interval> intervalsComputing( const QVector<qreal> &hist, const QVector<int> &maximums, qreal derivativeThreshold, int minimumWidthOfIntervals = DEFAULT_MINIMUM_WIDTH_OF_INTERVALS, bool loop = false );
	QVector<Interval> defaultComputingOfIntervals( const QVector<qreal> hist, bool loop = false );

	qreal minimumThresholdPercentage( const QVector<qreal> &hist );
	qreal minValue( QVector<qreal>::ConstIterator begin, QVector<qreal>::ConstIterator end );
	qreal maxValue( QVector<qreal>::ConstIterator begin, QVector<qreal>::ConstIterator end );
	qreal firstdDerivated( const QVector<qreal> &hist, const int &index, bool loop );
}

#endif // INTERVALSCOMPUTER_H