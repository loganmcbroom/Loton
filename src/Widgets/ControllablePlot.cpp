#include "ControllablePlot.hpp"

#include <QMouseEvent>

#include <qwt_plot.h>
#include <qwt_scale_div.h>


ControllablePlot::ControllablePlot()
	: QwtPlot()
	{
	setAxisScale( QwtPlot::Axis::yLeft, -pow(2,scale), pow(2,scale) );
	setAxisScale( QwtPlot::Axis::xBottom , -pow(2,scale), pow(2,scale) );
	}

void ControllablePlot::mousePressEvent( QMouseEvent * me )
	{
	QwtPlot::mousePressEvent( me );
	isClicked = true;
	position = me->pos();
	//me->accept();
	}

void ControllablePlot::mouseReleaseEvent( QMouseEvent * me )
	{
	isClicked = false;
	//me->accept();
	}

void ControllablePlot::mouseMoveEvent( QMouseEvent * me )
	{
	const QPointF delta = me->pos() - position;

	const auto xScale = axisScaleDiv( xBottom );
	const auto yScale = axisScaleDiv( yLeft );

	const double xValueDelta = invTransform( QwtPlot::Axis::xBottom, me->pos().x() ) - invTransform( QwtPlot::Axis::xBottom, position.x() );
	setAxisScale( QwtPlot::Axis::xBottom, xScale.lowerBound() - xValueDelta, xScale.upperBound() - xValueDelta );

	const double yValueDelta = invTransform( QwtPlot::Axis::yLeft, me->pos().y() ) - invTransform( QwtPlot::Axis::yLeft, position.y() );
	setAxisScale( QwtPlot::Axis::yLeft, yScale.lowerBound() - yValueDelta, yScale.upperBound() - yValueDelta );

	emit viewChanged();

	position = me->pos();
	//me->accept();
	}

void ControllablePlot::wheelEvent( QWheelEvent * me )
	{
	me->accept();
	const double amt = me->angleDelta().y() * .001;
	scale -= amt;

	const auto xScale = axisScaleDiv( xBottom );
	const auto yScale = axisScaleDiv( yLeft );
	const double aspect = yScale.interval().width() / xScale.interval().width();
	const double xCenter = ( xScale.upperBound() + xScale.lowerBound() ) / 2.0;
	const double yCenter = ( yScale.upperBound() + yScale.lowerBound() ) / 2.0;
	setAxisScale( QwtPlot::Axis::xBottom, xCenter - pow(2,scale), xCenter + pow(2,scale) );
	setAxisScale( QwtPlot::Axis::yLeft,   yCenter - pow(2,scale) * aspect, yCenter + pow(2,scale) * aspect );
	emit viewChanged();
	}

void ControllablePlot::resizeEvent( QResizeEvent * e )
	{
	QwtPlot::resizeEvent( e );

	if( e->oldSize().width() <= 0 || e->oldSize().height() <= 0 ||
		e->   size().width() <= 0 || e->   size().height() <= 0 )
		return;

	auto updateAxis = [this]( QwtPlot::Axis axis, double ratio )
		{
		QwtInterval interval = axisScaleDiv( axis ).interval();
		const double center = ( interval.maxValue() + interval.minValue() ) / 2.0;
		setAxisScale( axis, (interval.minValue() - center) * ratio + center, (interval.maxValue() - center) * ratio + center );
		};

	updateAxis( QwtPlot::Axis::xBottom, double( e->size().width() ) / double( e->oldSize().width() ) );
	updateAxis( QwtPlot::Axis::yLeft, double( e->size().height() ) / double( e->oldSize().height() ) );

	emit viewChanged();
	}

void ControllablePlot::showEvent( QShowEvent * )
	{
	//Enforce square grid
	const double delta = double( size().height() ) / double( size().width() ) * axisScaleDiv( QwtPlot::Axis::xBottom ).interval().width() / 2.0;
	QwtInterval interval = axisScaleDiv( QwtPlot::Axis::yLeft ).interval();
	const double center = ( interval.maxValue() + interval.minValue() ) / 2.0;
	setAxisScale( QwtPlot::Axis::yLeft, center - delta, center + delta );
	}

QSize ControllablePlot::minimumSizeHint() const
	{
	return {0,0};
	}


