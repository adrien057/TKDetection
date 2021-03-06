#ifndef PLOTSECTORHISTOGRAM_H
#define PLOTSECTORHISTOGRAM_H

#include "def/def_coordinate.h"

#include <qwt_polar_curve.h>
#include <qwt_plot_histogram.h>
#include <qwt_plot_curve.h>

class SectorHistogram;
class PointPolarSeriesData;
class QwtPlot;

class PlotSectorHistogram
{
public:
	PlotSectorHistogram();
	~PlotSectorHistogram();

	void attach( QwtPolarPlot * const polarPlot );
	void attach( QwtPlot * const plot );
	void clear();

	void update(const SectorHistogram &histogram);

	void moveCursor( const uint &index );

private:
	void updateDatas(const SectorHistogram &histogram);
	void updateMaximums( const SectorHistogram &histogram);
	void updateIntervals(const SectorHistogram &histogram);

private:
	QwtPlotHistogram _histogramData;
	QwtPlotHistogram _histogramMaximums;
	QwtPlotHistogram _histogramIntervals;
	QwtPlotHistogram _histogramCursor;

	QwtPlotCurve _curvePercentage;

	QwtPolarCurve _curveData;
	QwtPolarCurve _curveMaximums;
	QwtPolarCurve _curveIntervals;
	QwtPolarCurve _curveCursor;

	PointPolarSeriesData *_datasCurve;
	PointPolarSeriesData *_datasCurveMaximums;
	PointPolarSeriesData *_datasCurveIntervals;
	PointPolarSeriesData *_datasCurveCursor;
};

#endif // PLOTSECTORHISTOGRAM_H
