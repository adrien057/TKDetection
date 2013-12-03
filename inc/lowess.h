#ifndef LOWESS_H
#define LOWESS_H

#include <QtGlobal>

template <typename T> class QVector;

class Lowess
{
public:
	Lowess( const qreal &bandwidth = 0.33 );

	const qreal &bandWidth() const;
	void setBandWidth( const qreal &bandwidth );

	void compute( const QVector<qreal> &datas, QVector<qreal> &interpolatedDatas , QVector<qreal> &residus ) const;

private:
	qreal tricube( const qreal &u, const qreal &t ) const;

private:
	qreal _bandWidth;
	uint _robustness;
};

#endif // LOWESS_H
