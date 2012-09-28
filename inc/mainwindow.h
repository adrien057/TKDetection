#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>

#include "sliceview_def.h"
#include "slicezoomer.h"
#include "interval.h"
#include "global.h"
#include <qwt_plot_curve.h>
#include <qwt_plot_histogram.h>

namespace Ui {
	class MainWindow;
}

#include "billon_def.h"
class Marrow;
class SliceView;
class SliceHistogram;
class PieChart;
class PieChartDiagrams;
class ContourCurve;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow( QWidget *parent = 0 );
	~MainWindow();

	bool eventFilter( QObject *obj, QEvent *event );

private slots:
	void openDicom();
	void closeImage();
	void drawSlice();
	void drawSlice( const int &sliceNumber );
	void setSlice( const int &sliceNumber );
	void setTypeOfView( const int &type );
	void updateSliceHistogram();
	void highlightSliceHistogram( const int &slicePosition );
	void updateMarrow();
	void highlightSectorHistogram( const int &sectorIdx );
	void setMinimumOfSliceInterval( const int &min );
	void setMinimumOfSliceIntervalToCurrentSlice();
	void setMaximumOfSliceInterval( const int &max );
	void setMaximumOfSliceIntervalToCurrentSlice();
	void previousMaximumInSliceHistogram();
	void nextMaximumInSliceHistogram();
	void zoomInSliceView( const qreal &zoomFactor, const QPoint &focalPoint );
	void dragInSliceView( const QPoint &movementVector );
	void flowApplied();
	void setRestrictedAreaResolution( const int &resolution );
	void setRestrictedAreaThreshold( const int &threshold );
	void setRestrictedAreaBeginRadius( const int &radius );
	void setEdgeDetectionType( const int &type );
	void setCannyRadiusOfGaussianMask( const int &radius );
	void setCannySigmaOfGaussianMask( const double &sigma );
	void setCannyMinimumGradient( const int &minimumGradient );
	void setCannyMinimumDeviation( const double &minimumDeviation );
	void exportToDat();
	void exportToOfs();
	void exportToOfsAll();
	void exportToOfsRestricted();
	void exportSectorToOfs();
	void exportAllSectorInAllIntervalsToOfs();
	void exportToV3D();
	void exportFlowToV3D();
	void exportHistogramToSep();
	void exportMovementsToV3D();
	void selectSliceInterval( const int &index );
	void selectCurrentSliceInterval();
	void selectSectorInterval( const int &index );
	void selectCurrentSectorInterval();
	void exportSectorToPgm3D();
	void exportConnexComponentToPgm3D();
	void exportSliceHistogram();
	void exportSectorDiagramAndHistogram();
	void exportKnotIntervalHistogram();
	void exportContours();
	void exportContourComponentToPgm3D();
		void createVoxelSetAllIntervals(std::vector<iCoord3D> &vectVoxels, bool useOldMethod=false);
		void createVoxelSet(std::vector<iCoord3D> &vectVoxels);
		void exportAllContourComponentOfVoxels();
		void exportAllContourComponentOfVoxelsAllIntervals();
		void exportAllContourComponentOfVoxelsAllIntervalsOldMethod();


private:
	void openNewBillon(const QString &folderName = "");
	void initComponentsValues();
	void updateUiComponentsValues();
	void enabledComponents();
	void computeSectorsHistogramForInterval( const Interval &interval );

private:
	Ui::MainWindow *_ui;
	QImage _pix;
	QButtonGroup _groupSliceView;

	Billon *_billon;
	Billon *_sectorBillon;
	Billon *_componentBillon;
	Marrow *_marrow;
	SliceView *_sliceView;
	SliceHistogram *_sliceHistogram;
	QwtPlotCurve _histogramCursor;

	PieChart *_pieChart;
	PieChartDiagrams *_pieChartDiagrams;

	ContourCurve *_contourCurve;

	SliceZoomer _sliceZoomer;

	QwtPlotHistogram _histogramDistanceMarrowToNearestPoint;
	QwtPlotCurve _histogramDistanceMarrowToNearestPointCursor;
	Interval _knotIntervalInDistanceMarrowToNearestPointHistogram;

	int _currentSlice;
	int _currentMaximum;
	int _currentSector;
};

#endif // MAINWINDOW_H
