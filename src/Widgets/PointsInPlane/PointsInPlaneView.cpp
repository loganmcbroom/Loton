#include "PointsInPlaneView.hpp"

#include <QWheelEvent>
#include <QGraphicsItem>
#include <QDebug>

#include "Settings.hpp"

#include "PointsInPlaneModel.hpp"
#include "PointInPlane.hpp"

PointsInPlaneView::PointsInPlaneView( PointsInPlaneModel * pip )
	: model( pip )
	, sceneClickPos()
	, controller( this )
	{

	QObject::connect( model, &PointsInPlaneModel::destroyed,
		 this, [this](){ model = nullptr; } );

	QObject::connect( model, &PointsInPlaneModel::stateChanged,
		this, [this](){ update(); } );

	setScene( pip );
	setRenderHint( QPainter::Antialiasing );
	setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	//setTransformationAnchor( QGraphicsView::NoAnchor );
	setCacheMode( QGraphicsView::CacheBackground );

	// Infinite grid is achieved by moving the sceneRect around
	setSceneRect( -1, -1, 2, 2 );

	// Set zoom level
	fitInView( -5, -5, 10, 10, Qt::AspectRatioMode::KeepAspectRatio );

	controller.setInitialState();
	}

void PointsInPlaneView::scaleUp()
	{
	const float step = 6.0f / 5.0f;
	//QTransform t = transform();
	//if( t.m11() > 3.0 ) return;
	//scale( step, step );
	LotonController::manager().undoStack->push(
		new ZoomCommand( *this, step ) );
	}

void PointsInPlaneView::scaleDown()
	{
	const float step = 5.0f / 6.0f;
	//scale( step, step );
	//controller.pushStateChange( "PointInPlane zoom" );
	LotonController::manager().undoStack->push(
		new ZoomCommand( *this, step ) );
	}

void PointsInPlaneView::drawBackground( QPainter * painter , const QRectF & r )
	{
	auto drawGrid = [this, painter, r]( double gridStep )
		{
		//Find first line on gridStep grid inside the viewable rect
		const double leftLine   = std::ceil( r.left()   / gridStep ) * gridStep;
		const double bottomLine = std::ceil( r.bottom() / gridStep ) * gridStep;

		// vertical lines
		for( double xi = leftLine; xi <= r.right(); xi += gridStep )
			painter->drawLine( QLineF( xi, r.top(), xi, r.bottom() ) );

		// horizontal lines
		for( double yi = bottomLine; yi >= r.top(); yi -= gridStep )
			painter->drawLine( QLineF( r.left(), yi, r.right(), yi ) );
		};

	//find pixel size in scene
	const float unitsPerPixel = r.width() / double( rect().width() );

	auto & pal = Settings::palette();

	//background color
	painter->setBrush( pal.dark() );
	painter->setPen( pal.color( QPalette::Dark ) );
	painter->drawRect( r );

	//fine grid
	QPen pfine( pal.mid(), unitsPerPixel * 0.8 );
	painter->setPen( pfine );
	drawGrid( PointsInPlaneModel::fineGridSize );

	//coarse grid
	QPen p( pal.highlight(), unitsPerPixel );
	painter->setPen( p );
	drawGrid( PointsInPlaneModel::coarseGridSize );

	//axes
	QPen pAxis( pal.light(), unitsPerPixel * 1.2 );
	painter->setPen( pAxis );
	painter->drawLine( QLineF( 0, r.bottom(), 0, r.top() ) );
	painter->drawLine( QLineF( r.left(), 0, r.right(), 0 ) );
	}

void PointsInPlaneView::wheelEvent( QWheelEvent * event )
	{
	event->accept();
	const double d = event->angleDelta().y();

	if( d == 0 )
		event->ignore();
	else
		d > 0.0? scaleUp() : scaleDown();
}

void PointsInPlaneView::mousePressEvent( QMouseEvent * event )
	{
	QGraphicsView::mousePressEvent( event );
	if( event->button() == Qt::LeftButton )
		{
		sceneClickPos = mapToScene( event->pos() );
		}
	else if( event->button() == Qt::RightButton )
		{
		if( QGraphicsItem * item = itemAt( event->pos() ) )
			{
			auto * point = qgraphicsitem_cast<PointInPlane *>( item );
			if( model && point )
				model->removePoint( point );
			}
		}
	//event->accept();
	}

void PointsInPlaneView::mouseMoveEvent( QMouseEvent * event )
	{
	QGraphicsView::mouseMoveEvent( event );

	if( scene()->mouseGrabberItem() == nullptr && event->buttons() == Qt::LeftButton )
		{
		translate( sceneClickPos - mapToScene( event->pos() ) );
		}
	//event->accept();
	}

void PointsInPlaneView::mouseReleaseEvent( QMouseEvent * event )
	{
	QGraphicsView::mouseReleaseEvent( event );
	if( ! event->isAccepted() )
		{
		controller.pushStateChange( "PointsInPlane view changed" );
		}
//	event->accept();
	}

void PointsInPlaneView::mouseDoubleClickEvent( QMouseEvent * event )
	{
	auto itemsAtPos = items( event->pos() );
	if( ! itemsAtPos.empty() )
		return;

	if( model )
		{
		auto scenePos = mapToScene( event->pos() );
		model->createPointAt( scenePos );

		itemsAtPos = items( event->pos() );
		Q_ASSERT( ! itemsAtPos.empty() );
		auto item = itemsAtPos[0];
		releaseMouse();
		item->grabMouse();
		}

	event->accept();
	}

void PointsInPlaneView::translate( QPointF d )
	{
	setSceneRect( sceneRect().translated( d ) );
	}

//===================================================================================
// Controller

PointsInPlaneView::ControllerProxy::ControllerProxy( PointsInPlaneView * _owner )
	: LotonController( QVariant() )
	, owner( _owner )
	{
	}

QVariant PointsInPlaneView::ControllerProxy::state() const
	{
	return owner->sceneRect();
	}

void PointsInPlaneView::ControllerProxy::setState( const QVariant & s )
	{
	owner->setSceneRect( s.toRectF() );
	}

//===================================================================================
// Zoom Command

PointsInPlaneView::ZoomCommand::ZoomCommand( const PointsInPlaneView & v, float s )
	: QUndoCommand( "View zoom changed" )
	, viewId( v.controller.id() )
	, oldScale( v.transform().m11() )
	, newScale( s * oldScale )
	{
	}

void PointsInPlaneView::ZoomCommand::undo()
	{
	auto * view = dynamic_cast<PointsInPlaneView::ControllerProxy *>(
		LotonController::getControllerFromManager( viewId ) )->owner;
	const float factor = oldScale / view->transform().m11();
	view->scale( factor, factor );
	}

void PointsInPlaneView::ZoomCommand::redo()
	{
	auto * view = dynamic_cast<PointsInPlaneView::ControllerProxy *>(
		LotonController::getControllerFromManager( viewId ) )->owner;
	const float factor = newScale / view->transform().m11();
	view->scale( factor, factor );
	}

int PointsInPlaneView::ZoomCommand::id() const
	{
	return 756767601;
	}

bool PointsInPlaneView::ZoomCommand::mergeWith( const QUndoCommand * other )
	{
	auto * otherZoom = dynamic_cast<const PointsInPlaneView::ZoomCommand *>( other );
	newScale = otherZoom->newScale;
	return true;
	}



