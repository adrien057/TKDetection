#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>

#include "sliceview_def.h"
#include "slicezoomer.h"
#include "slicesinterval.h"
#include "intensityinterval.h"
#include <qwt_plot_curve.h>

namespace Ui {
	class MainWindow;
}

#include "billon_def.h"
class Marrow;
class SliceView;
class SliceHistogram;
class PieChart;
class PieChartDiagrams;

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
	void drawSlice( const int &sliceNumber );
	void setTypeOfView( const int &type );
	void setLowThreshold( const int &threshold );
	void setHighThreshold( const int &threshold );
	void updateSliceHistogram();
	void highlightSliceHistogram( const int &slicePosition );
	void updateMarrow();
	void updateSectorsHistograms();
	void selectSectorHistogram( const int &sectorIdx );
	void setMinimumOfSlicesIntervalToCurrentSlice();
	void setMaximumOfSlicesIntervalToCurrentSlice();
	void setMinimalDifferenceForSectors( const int &minimalDifference );
	void previousMaximumInSliceHistogram();
	void nextMaximumInSliceHistogram();
	void zoomInSliceView( const qreal &zoomFactor, const QPoint &focalPoint );
	void dragInSliceView( const QPoint &movementVector );
	void setMovementThreshold( const int &threshold );
	void enableMovementWithBackground( const bool &enable );
	void useNextSliceInsteadOfCurrentSlice( const bool &enable );
	void flowApplied();
	void setRestrictedAreaResolution( const int &resolution );
	void setRestrictedAreaThreshold( const int &threshold );
	void enableRestrictedAreaCircle( const bool &enable ) ;
	void exportToDat();
	void exportToOfs();
	void exportToV3D();
	void exportFlowToV3D();
	void exportDiagramToV3D();

private:
	void openNewBillon(const QString &folderName = "");
	void drawSlice();
	void updateComponentsValues();
	void enabledComponents();

private:
	Ui::MainWindow *_ui;
	QImage _pix;
	QButtonGroup _groupSliceView;

	Billon *_billon;
	Marrow *_marrow;
	SliceView *_sliceView;
	SliceHistogram *_sliceHistogram;
	QwtPlotCurve _histogramCursor;

	PieChart *_pieChart;
	PieChartDiagrams *_pieChartDiagrams;
	QList<QwtPlot *> _pieChartPlots;

	SliceZoomer _sliceZoomer;
	SlicesInterval _slicesInterval;
	IntensityInterval _intensityInterval;

	int _currentSlice;
	int _currentMaximum;
};

#endif // MAINWINDOW_H
