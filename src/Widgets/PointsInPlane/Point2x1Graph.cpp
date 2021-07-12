#include "Point2x1Graph.hpp"

#include <QJsonObject>
#include <QPainterPath>

#include "Widgets/Interpolator.hpp"
#include "NodeDataTypes/InterpolatorData.hpp"
#include "NodeDataTypes/FunctionData.hpp"

Point2x1GraphModel::Point2x1GraphModel()
	: out()
	{
	}

Point2x1GraphModel::~Point2x1GraphModel() = default;

QJsonObject Point2x1GraphModel::save() const
	{
	auto j = PointsInPlaneModel::save();
	return j;
	}

void Point2x1GraphModel::restore( const QJsonObject & j )
	{
	PointsInPlaneModel::restore( j );
	}

std::shared_ptr<Func2x1Data> Point2x1GraphModel::getOut()
	{
	return out;
	}

void Point2x1GraphModel::setOut( std::shared_ptr<Func2x1Data> o )
	{
	out = o;
	}

//========================================================================

Point2x1GraphView::Point2x1GraphView( Point2x1GraphModel * _model )
	: PointsInPlaneView( _model )
	{
	}

void Point2x1GraphView::drawForeground( QPainter * painter, const QRectF & r )
	{
	if( !model ) return;

	const int xStart = std::ceil ( r.left()		);
	const int xEnd	 = std::floor( r.right()	);
	const int yStart = std::ceil ( r.bottom()	);
	const int yEnd   = std::floor( r.top()		);

	painter->setPen( Qt::white );

	auto & f = static_cast<Point2x1GraphModel *>( model )->getOut()->f;

	for( int x = xStart; x <= xEnd; ++x )
		for( int y = yEnd; y <= yStart; ++y )
			{
			const float pi = 3.1416f;
			const float val = std::atan( std::abs( f( x, y ) ) ) / ( pi / 2 );

			QPainterPath path;
			const float size = .3f;
			path.addEllipse( -size/2 + x, -size/2 + y, size, size );
			painter->fillPath( path, QColor::fromHslF( 0, .5, val ) );
			}

	update();
	}



