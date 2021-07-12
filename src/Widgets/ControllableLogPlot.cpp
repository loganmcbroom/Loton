#include "ControllableLogPlot.hpp"

#include <QMouseEvent>
#include <QPainterPath>

#include <qwt_plot.h>
#include <qwt_scale_div.h>
#include <qwt_scale_engine.h>

#include "NodeDataTypes/FunctionData.hpp"
#include "Settings.hpp"

float logBase( float arg, float base ){ return std::log( arg ) / std::log( base ); };
float logCeil( float x ){ return std::pow( 2.0, std::floor( std::log2( x ) ) ); };

ControllableLogPlot::ControllableLogPlot( bool _logMode )
	: QwtPlot()
	, logMode( _logMode )
	{
	setLogMode( logMode );
	}

bool ControllableLogPlot::getLogMode() const
	{
	return logMode;
	}

void ControllableLogPlot::setLogMode( bool newMode )
	{
	if( newMode )
		{
		setAxisScaleEngine( yLeft, new QwtLogScaleEngine( 10 ) );
		setAxisScale( yLeft, logLowerBound, logUpperBound );
		setAxisScale( xBottom , -4, 4 );
		}
	else
		{
		setAxisScaleEngine( yLeft, new QwtLinearScaleEngine( 10 ) );
		setAxisScale( yLeft, -4, 4 );
		setAxisScale( xBottom , -4, 4 );
		}
	updateAxes();
	logMode = newMode;
	}

void ControllableLogPlot::mousePressEvent( QMouseEvent * me )
	{
	QwtPlot::mousePressEvent( me );
	isClicked = true;
	position = me->pos();
	}

void ControllableLogPlot::mouseReleaseEvent( QMouseEvent * )
	{
	isClicked = false;
	}

void ControllableLogPlot::mouseMoveEvent( QMouseEvent * me )
	{
	auto moveAxisWithMode = [this]( bool isLogMode, float oldPos, float newPos, QwtPlot::Axis axis )
		{
		const auto div = axisScaleDiv( axis );
		if( isLogMode )
			{
			const float delta = invTransform( axis, newPos ) / invTransform( axis, oldPos );
			const float yLow = std::max( div.lowerBound() / delta, logLowerBound );
			const float yHigh = std::min( div.upperBound() / delta, logUpperBound );
			if( yLow != logLowerBound && yHigh != logUpperBound )
				setAxisScale( axis, yLow, yHigh );
			}
		else
			{
			const float delta = invTransform( axis, newPos ) - invTransform( axis, oldPos );
			const float low = div.lowerBound() - delta;
			const float high = div.upperBound() - delta;
			setAxisScale( axis, low, high );
			}
		};

	moveAxisWithMode( false, position.x(), me->pos().x(), xBottom );
	moveAxisWithMode( logMode, position.y(), me->pos().y(), yLeft );

	updateAxes();
	emit viewChanged();

	position = me->pos();
	}

void ControllableLogPlot::wheelEvent( QWheelEvent * me )
	{
	auto scaleAxisWithMode = [this]( bool isLogMode, float angle, QwtPlot::Axis axis )
		{
		auto div = axisScaleDiv( axis );
		if( isLogMode )
			{
			const double alpha = -angle * .0004;
			const double a = div.lowerBound();
			const double b = div.upperBound();
			const double low = std::max( std::pow( a, 1.0 + alpha ) * std::pow( b, -alpha ), logLowerBound );
			const double high = std::min( std::pow( a, -alpha ) * std::pow( b, 1.0 + alpha ), logUpperBound );
			setAxisScale( axis, low, high );
			}
		else
			{
			const double amt = 1.0 + angle * .001;
			const double newXWidth = ( div.upperBound() - div.lowerBound() ) / amt;
			const double xCenter = ( div.upperBound() + div.lowerBound() ) / 2.0;
			const double low = xCenter - newXWidth / 2;
			const double high = xCenter + newXWidth / 2;
			setAxisScale( axis, low, high );
			}
		};

	me->accept();

	if( logMode )
		{
		if( me->modifiers() & Qt::ControlModifier ) // y scale
			scaleAxisWithMode( true, me->angleDelta().y(), yLeft );
		else // x scale
			scaleAxisWithMode( false, me->angleDelta().y(), xBottom );
		}
	else
		{
		scaleAxisWithMode( false, me->angleDelta().y(), yLeft );
		scaleAxisWithMode( false, me->angleDelta().y(), xBottom );
		}

	updateAxes();
	emit viewChanged();
	}

void ControllableLogPlot::resizeEvent( QResizeEvent * e )
	{
	QwtPlot::resizeEvent( e );

	if( e->oldSize().width() <= 0 || e->oldSize().height() <= 0 ||
		e->   size().width() <= 0 || e->   size().height() <= 0 )
		return;

	QwtInterval interval = axisInterval( QwtPlot::Axis::xBottom );
	const double center = ( interval.maxValue() + interval.minValue() ) / 2.0;
	const double ratio = double( e->size().width() ) / double( e->oldSize().width() );
	const float min = (interval.minValue() - center) * ratio + center;
	const float max = (interval.maxValue() - center) * ratio + center;
	setAxisScale( QwtPlot::Axis::xBottom, (interval.minValue() - center) * ratio + center, (interval.maxValue() - center) * ratio + center );
	updateAxes();

	emit viewChanged();
	}

QSize ControllableLogPlot::minimumSizeHint() const
	{
	return {0,0};
	}



void ControllableLogPlot2x1::drawCanvas( QPainter * p )
	{
	auto atan_ish = []( float x ){ return x / ( x + 1 ); };

	p->setRenderHint( QPainter::Antialiasing );
	QwtPlot::drawCanvas( p );

	if( ! fs ) return;

	auto pal = Settings::palette();
	const float hueN = pal.color( QPalette::Highlight ).hueF();
	const float hueP = pal.color( QPalette::Light     ).hueF();

	for( auto s : *fs )
		{
		const float val = atan_ish( std::abs( s.z ) );
		const float size = 3 + 10 * val * val;

		const float xPos = transform( QwtPlot::Axis::xBottom, s.x );
		const float yPos = transform( QwtPlot::Axis::yLeft, s.y );

		QPainterPath path;
		path.addEllipse( xPos - size/2, yPos - size/2, size, size );
		p->fillPath( path, QColor::fromHslF( s.z > 0 ? hueP : hueN, .5, .2 + .7 * val ) );
		}
	}

void ControllableLogPlot2x2::drawCanvas( QPainter * p )
	{
	auto atan_ish = []( float x ){ return x / ( x + 1 ); };

	p->setRenderHint( QPainter::Antialiasing );
	QwtPlot::drawCanvas( p );

	if( ! fs ) return;

	auto pal = Settings::palette();
	const QColor highlight = pal.color( QPalette::Highlight );

	for( auto s : *fs )
		{
		const flan::vec2 d = { s.z - s.x, s.w - s.y };
		const float mag = d.mag();
		const float val = atan_ish( mag / 2 );
		const float scale = ( 4 + 8 * val * val ) / mag;

		const float xPos = transform( QwtPlot::Axis::xBottom, s.x );
		const float yPos = transform( QwtPlot::Axis::yLeft, s.y );

		auto color = QColor::fromHslF( highlight.hueF(), highlight.saturationF(), .2 + .7 * val );

		QPainterPath linePath;
		linePath.moveTo( xPos, yPos );
		linePath.lineTo( xPos + ( s.z - s.x ) * scale, yPos - ( s.w - s.y ) * scale );
		p->setPen( QPen( color, 1, Qt::SolidLine ) );
		p->drawPath( linePath );

		QPainterPath circlePath;
		const float size = 5;
		circlePath.addEllipse( xPos - size/2, yPos - size/2, size, size );
		p->fillPath( circlePath, color );
		}
	}



