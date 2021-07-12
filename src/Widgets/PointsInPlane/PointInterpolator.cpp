#include "PointInterpolator.hpp"

#include <QJsonObject>
#include <QPainterPath>

#include "Widgets/Interpolator.hpp"
#include "NodeDataTypes/InterpolatorData.hpp"
#include "NodeDataTypes/FunctionData.hpp"

PointInterpolatorModel::PointInterpolatorModel()
	: interpModel( new InterpolatorModel() )
	{
	QObject::connect( interpModel.get(), &InterpolatorModel::stateChanged,
					  this, &PointsInPlaneModel::stateChanged );
	}

PointInterpolatorModel::~PointInterpolatorModel() = default;

std::shared_ptr<Func1x1Data>
PointInterpolatorModel::function()
	{
	auto ps = pointPositions();

	if( ps.empty() )
		return std::make_shared<Func1x1Data>( []( float ){ return 0; } );

	std::sort( ps.begin(), ps.end(), PointsInPlaneModel::xThenYSortCompare );

	auto i = interpModel->getInterpolator();

	return std::make_shared<Func1x1Data>( [i, ps]( float t )
		{
		//Negation on returns to correct upside down computer coordinates

		auto bound = std::upper_bound( ps.begin(), ps.end(), t, []( float t, QPointF a )
			{
			return t < a.x();
			} );

		if( bound == ps.end() )
			return -ps.back().y();
		else if( bound == ps.begin() )
			return -ps.front().y();
		else
			{
			QPointF p0 = *( bound - 1 );
			QPointF p1 = *( bound     );
			return -( p0.y() + ( p1.y() - p0.y() ) * i( ( t - p0.x() ) / ( p1.x() - p0.x() ) ) );
			}
		} );
	}

QJsonObject PointInterpolatorModel::save() const
	{
	auto j = PointsInPlaneModel::save();
	j["interpolator"] = interpModel->save();
	return j;
	}

void PointInterpolatorModel::restore( const QJsonObject & j )
	{
	PointsInPlaneModel::restore( j );
	interpModel->restore( j["interpolator"].toObject() );
	}

//========================================================================

PointInterpolatorView::PointInterpolatorView( PointInterpolatorModel * _model )
	: PointsInPlaneView( _model )
	{
	}

void PointInterpolatorView::drawForeground( QPainter * painter, const QRectF & r )
	{
	if( !model ) return;

	auto ps = model->pointPositions();
	if( ps.empty() ) return;
	std::sort( ps.begin(), ps.end(), PointsInPlaneModel::xThenYSortCompare );

	auto iModel = static_cast<PointInterpolatorModel *>( model );
	auto interpType = iModel->interpModel->getInterpolatorType();

	painter->setPen( QPen( Qt::white, 0 ) );

	if( interpType == InterpolatorType::linear )
		{
		for( int i = 0; i < ps.size() - 1; ++i )
			painter->drawLine( ps[i], ps[i+1] );
		}
	else if( interpType == InterpolatorType::nearest )
		{
		for( int i = 0; i < ps.size() - 1; ++i )
			{
			const float deltaX = ps[i+1].x() - ps[i].x();

			QPainterPath path;
			path.moveTo( ps[i] );
			path.lineTo( ps[i] + QPointF( deltaX / 2.0f, 0.0f ) );
			path.lineTo( ps[i+1] - QPointF( deltaX / 2.0f, 0.0f ) );
			path.lineTo( ps[i+1] );

			painter->drawPath( path );
			}
		}
	else // Catch-all function sampling paint routine
		{
		std::shared_ptr<Func1x1Data> func = iModel->function();
		const float unitsPerSample = 5.0f * float( r.width() ) / float( rect().width() );

		QPainterPath path;
		path.moveTo( ps.front() );
		for( float x = ps.front().x(); x < ps.back().x(); x += unitsPerSample )
			path.lineTo( x, -func->f( x ) );
		path.lineTo( ps.back() );

		painter->drawPath( path );
		}

	//Draw flat lines from start and end points
	painter->drawLine( ps.front(), QPointF( r.left(), ps.front().y() ) );
	painter->drawLine( ps.back(),  QPointF( r.right(), ps.back().y() ) );

	update();
	}
