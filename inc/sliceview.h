#ifndef SLICEVIEW_H
#define SLICEVIEW_H

#include "sliceview_def.h"
#include "marrowextractor_def.h"
#include <QObject>

class Billon;
class QPixmap;

class SliceView : public QObject
{
	Q_OBJECT

public:
	explicit SliceView();
	~SliceView();

	void setModel( const Billon * billon );
	void setModel( const Marrow* marrow );

	int currentSlice() const;

public slots:
	void drawSlice( const int &sliceNumber = -1 );
	void update();
	void setLowThreshold(const int &threshold);
	void setHighThreshold(const int &threshold);
	void setTypeOfView(const int &type = SliceType::CURRENT);
	void drawMarrow( bool enable );

private :
	void drawCurrentSlice();
	void drawAverageSlice();
	void drawMedianSlice();
	void drawMarrow();

signals:
	void updated(const QPixmap &);
	void thresholdUpdated();
	void typeOfViewChanged(const SliceType::SliceType &);

private:
	QPixmap *_pix;
	const Billon * _billon;
	const Marrow *_marrow;

	int _currentSlice;
	int _lowThreshold;
	int _highThreshold;
	SliceType::SliceType _typeOfView;
	bool _drawMarrow;
};


/**********************************
 * Définition des fonctions inline
 **********************************/

inline
int SliceView::currentSlice() const {
	return _currentSlice;
}

#endif // SLICEVIEW_H
