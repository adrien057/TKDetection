#include "inc/detection/slicehistogram.h"

#include "inc/billon.h"

SliceHistogram::SliceHistogram() : Histogram<qreal>()
{
}

SliceHistogram::~SliceHistogram()
{
}

/**********************************
 * Public setters
 **********************************/

void SliceHistogram::construct( const Billon &billon, const Interval<int> &intensity, const uint &zMotionMin, const int &radiusAroundPith )
{
	if ( !billon.hasPith() ) return;

	const int width = billon.n_cols;
	const int height = billon.n_rows;
	const int depth = billon.n_slices;
	const qreal squareRadius = qPow(radiusAroundPith,2);

	clear();
	resize(depth-1);

	QVector<int> circleLines;
	circleLines.reserve(2*radiusAroundPith+1);
	for ( int lineIndex=-radiusAroundPith ; lineIndex<=radiusAroundPith ; ++lineIndex )
	{
		circleLines.append(qSqrt(squareRadius-qPow(lineIndex,2)));
	}

	const Interval<uint> &validSlices = billon.validSlices();
	const uint &lastSlice = validSlices.max();
	int i, j, iRadius, iRadiusMax;
	uint diff, k;
	iCoord2D currentPos;
	qreal cumul;

	for ( k=validSlices.min() ; k<lastSlice ; ++k )
	{
		cumul = 0.;
		currentPos.y = billon.pithCoord(k).y-radiusAroundPith;
		for ( j=-radiusAroundPith ; j<=radiusAroundPith ; ++j, currentPos.y++ )
		{
			iRadius = circleLines[j+radiusAroundPith];
			iRadiusMax = iRadius+1;
			currentPos.x = billon.pithCoord(k).x-iRadius;
			for ( i=-iRadius ; i<iRadiusMax ; ++i, currentPos.x++ )
			{
				if ( currentPos.x >= 0 && currentPos.y >= 0 && currentPos.x < width && currentPos.y < height )
				{
					if ( intensity.containsClosed(billon(currentPos.y,currentPos.x,k)) && intensity.containsClosed(billon.previousSlice(k)(currentPos.y,currentPos.x)) )
					{
						diff = billon.zMotion( currentPos.x, currentPos.y, k );
						if ( diff > zMotionMin ) cumul += diff-zMotionMin;
					}
				}
			}
		}
		(*this)[k] = cumul;
	}
}