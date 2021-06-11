#pragma once

#include <QMouseEvent>

#include <qwt_plot.h>

// 2D function graph

class ControllablePlot : public QwtPlot
{ Q_OBJECT
public:
	ControllablePlot();

	void mousePressEvent( QMouseEvent * me ) override;
	void mouseReleaseEvent( QMouseEvent * me ) override;
	void mouseMoveEvent( QMouseEvent * me ) override;
	void wheelEvent( QWheelEvent * me ) override;

	bool isClicked = false;
	QPoint position;
	double scale = 2.0;

signals:
	void viewChanged();

private:
	void resizeEvent( QResizeEvent * ) override;
	QSize minimumSizeHint() const override;
	void showEvent( QShowEvent *event ) override;
};
