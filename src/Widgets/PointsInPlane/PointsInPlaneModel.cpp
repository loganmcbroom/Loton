#include "PointsInPlaneModel.hpp"

#include <QJsonObject>
#include <QJsonArray>
#include <QGraphicsSceneMouseEvent>

#include "PointInPlane.hpp"

PointsInPlaneModel::PointsInPlaneModel()
	: controller( this )
	{
	}

PointsInPlaneModel::~PointsInPlaneModel() = default;

void PointsInPlaneModel::createPointAt( QPointF p )
	{
	auto command = new PointAddCommand( *this, p );
	LotonController::manager().undoStack->push( command );
	}

void PointsInPlaneModel::removePoint( PointInPlane * p )
	{
	LotonController::manager().undoStack->push( new PointRemoveCommand( *this, *p ) );
	}

QUuid PointsInPlaneModel::id() const
	{
	return controller.id();
	}

QJsonObject PointsInPlaneModel::save() const
	{
	return controller.save();
	}

void PointsInPlaneModel::restore( const QJsonObject & j )
	{
	controller.restore( j );
	}

std::vector<PointInPlane *> PointsInPlaneModel::points() const
	{
	std::vector<PointInPlane *> pointVec;

	auto itemVec = items();
	for( auto * i : itemVec )
		{
		if( i->type() == PointInPlane::Type )
			pointVec.push_back( qgraphicsitem_cast<PointInPlane *>( i ) );
		}

	return pointVec;
	}

std::vector<QPointF> PointsInPlaneModel::pointPositions() const
	{
	auto ps = points();
	std::vector<QPointF> poss;
	for( auto & p : ps )
		poss.emplace_back( p->pos() );
	return poss;
	}

bool PointsInPlaneModel::xThenYSortCompare( QPointF a, QPointF b )
	{
	return a.x() != b.x() ? a.x() < b.x() : a.y() > b.y();
	}

//==================================================================================
// Controller

QVariant PointsInPlaneModel::ControllerProxy::state() const
	{
	auto ps = scene->points();
	QList<QVariant> vs;
	for( auto & i : ps )
		{
		// Must be split, QPoint in QVariant can't convert to QJsonValue
		vs.push_back( i->pos().x() );
		vs.push_back( i->pos().y() );
		vs.push_back( i->id() );
		}

	return vs;
	}

void PointsInPlaneModel::ControllerProxy::setState( const QVariant & s )
	{
	scene->clear();
	QList<QVariant> vs = s.toList();
	for( int i = 0; i < vs.size(); i += 3 )
		{
		QPointF point( vs[i].toFloat(), vs[i+1].toFloat() );
		auto * item = new PointInPlane( point, vs[i+2].toUuid() );
		connect( item, &LotonController::stateChanged,
			 scene, &PointsInPlaneModel::stateChanged );
		scene->addItem( item );
		}
	}

//==================================================================================
// PointAddCommand


PointsInPlaneModel::PointAddCommand::PointAddCommand( PointsInPlaneModel & model, QPointF p )
	: QUndoCommand( "Point Added" )
	, pos( p )
	, sceneId( model.controller.id() )
	, itemId( QUuid::createUuid() )
	{}

void PointsInPlaneModel::PointAddCommand::undo()
	{
	auto scene = dynamic_cast<PointsInPlaneModel::ControllerProxy *>(
		LotonController::getControllerFromManager( sceneId ) )->scene;

	auto item = dynamic_cast<PointInPlane *>(
		LotonController::getControllerFromManager( itemId ) );

	scene->removeItem( item );
	delete item;

	emit scene->stateChanged();
	}

void PointsInPlaneModel::PointAddCommand::redo()
	{
	auto scene = dynamic_cast<PointsInPlaneModel::ControllerProxy * >(
		LotonController::getControllerFromManager( sceneId ) )->scene;

	auto * point = new PointInPlane( pos, itemId );
	connect( point, &LotonController::stateChanged,
			 scene, &PointsInPlaneModel::stateChanged );
	scene->addItem( point );

	emit scene->stateChanged();
	}

//==================================================================================
// PointRemoveCommand

PointsInPlaneModel::PointRemoveCommand::PointRemoveCommand( PointsInPlaneModel & scene, PointInPlane & point )
	: QUndoCommand( "Point Removed" )
	, pos( point.pos() )
	, sceneId( scene.controller.id() )
	, itemId( point.id() )
	{
	}

void PointsInPlaneModel::PointRemoveCommand::undo()
	{
	auto scene = dynamic_cast<PointsInPlaneModel::ControllerProxy * >(
		LotonController::getControllerFromManager( sceneId ) )->scene;

	auto * point = new PointInPlane( pos, itemId );
	connect( point, &LotonController::stateChanged,
			 scene, &PointsInPlaneModel::stateChanged );
	scene->addItem( point );

	emit scene->stateChanged();
	}

void PointsInPlaneModel::PointRemoveCommand::redo()
	{
	auto scene = dynamic_cast<PointsInPlaneModel::ControllerProxy *>(
		LotonController::getControllerFromManager( sceneId ) )->scene;
	auto item = dynamic_cast<PointInPlane *>(
		LotonController::getControllerFromManager( itemId ) );

	scene->removeItem( item );
	delete item;

	emit scene->stateChanged();
	}


