#include "PointInPlane.hpp"

#include <QPainter>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include "Settings.hpp"
#include "PointsInPlaneModel.hpp"
#include "MainWindow.hpp"
#include "LotonController.hpp"

PointInPlane::PointInPlane( QPointF initial, QUuid id )
	: LotonController( initial, id )
	, size( defaultSize )
	{
	setPos( initial );
	setCursor( Qt::ArrowCursor );
	//setFlag( QGraphicsItem::ItemIsMovable );
	setFlag( QGraphicsItem::ItemIgnoresTransformations );
	setAcceptHoverEvents( true );
	}

QRectF PointInPlane::boundingRect() const
	{
	return { -size / 2, -size / 2, size, size };
	}

void PointInPlane::paint( QPainter * painter, const QStyleOptionGraphicsItem *, QWidget * )
	{
	auto light = Settings::palette().color( QPalette::Light );
	QPainterPath path;
	path.addEllipse( -size/2, -size/2, size, size );

	painter->fillPath( path, light );

	painter->setPen( Qt::white );
	painter->drawPath( path );
	}

void PointInPlane::mousePressEvent( QGraphicsSceneMouseEvent * event )
	{
	grabMouse();
	event->accept();
	}

void PointInPlane::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
	{
	setPos( event->scenePos() );

	// Lock to fine grid when holding shift
	if( event->modifiers() & Qt::ShiftModifier )
		{
		auto roundToGrid = []( float x )
			{
			const auto s = PointsInPlaneModel::fineGridSize;
			return std::round( x / s ) * s;
			};
		setPos( roundToGrid( pos().x() ), roundToGrid( pos().y() ) );
		}

	emit stateChangedDynamic();

	event->accept();
	}

void PointInPlane::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
	{
	ungrabMouse();
	if( previousState() != state() )
		pushStateChange( "Point Moved" );
	event->accept();
	}

void PointInPlane::hoverEnterEvent( QGraphicsSceneHoverEvent * event )
	{
	// bring all the colliding nodes to background
	QList<QGraphicsItem *> overlapItems = collidingItems();
	for( QGraphicsItem * item : overlapItems )
		if( item->zValue() > 0.0 )
			item->setZValue( 0.0 );

	// bring this node forward
	setZValue( 1.0 );

	size = defaultSize * 1.2f;
	update();
	event->accept();
	}

void PointInPlane::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
	{
	size = defaultSize;
	update();
	event->accept();
	}

//void PointInPlane::grabMouse()
//	{
//	auto e = new QGraphicsSceneMouseEvent();
//	e->setPos( pos() );
//	mousePressEvent( e );
//	}

QVariant PointInPlane::state() const
	{
	return pos();
	}

void PointInPlane::setState( const QVariant & s )
	{
	bool b = blockSignals( true );
	setPos( s.toPointF() );
	update();
	blockSignals( b );
	}

//==================================================================================
// PointMoveCommand


//PointInPlane::PointMoveCommand::PointMoveCommand( PointInPlane & model )
//	: pos( model.pos() )
//	, oldPos( model.lastStaticPos )
//	, sceneId( dynamic_cast<PointsInPlaneModel *>( model.scene() )->id() )
//	{
//	}

//void PointInPlane::PointMoveCommand::undo()
//	{
//	auto o = dynamic_cast<PointsInPlaneModel::ControllerProxy *>(
//		LotonController::getControllerFromManager( sceneId ) )->owner;

//	o->itemAt( pos,
//	}

//void PointInPlane::PointMoveCommand::redo()
//	{
//	}



