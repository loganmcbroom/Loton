#include "LotonController.hpp"

#include <QJsonObject>
#include <QUndoView>

LotonController::LotonController( QVariant initialState, QUuid id )
	: QObject()
	, _id( id.isNull()? QUuid::createUuid() : id )
	, _previousState( initialState )
	{
	manager().registry.insert( { _id, this } );
	QObject::connect( this, &LotonController::stateChanged,
					  this, &LotonController::stateChangedDynamic );
	}

LotonController::~LotonController()
	{
	manager().registry.erase( _id );
	}

void LotonController::setId( QUuid id )
	{
	manager().registry.erase( _id );
	manager().registry.insert( { id, this } );
	_id = id;
	emit idChanged();
	}

QJsonObject LotonController::save() const
	{
	QJsonObject j;
	j["id"] = id().toString();
	j["state"] = QJsonValue::fromVariant( state() );
	return j;
	}

void LotonController::restore( const QJsonObject & j )
	{
	setId( j["id"].toString() );
	auto stateToRestore = j["state"].toVariant();
	setState( stateToRestore );
	_previousState = stateToRestore;
	emit stateChanged();
	}

void LotonController::pushStateChange( const QString & msg, LotonCommand * command )
	{
	auto currentState = state();
	manager().undoStack->push( command? command : new LotonCommand( *this, currentState, msg ) );
	_previousState = currentState;
	emit stateChanged();
	}

void LotonController::setInitialState()
	{
	_previousState = state();
	}

LotonController * LotonController::getControllerFromManager( QUuid id )
	{
	auto model = manager().registry[id];
	Q_ASSERT_X( model != nullptr, "LotonController::getController", "Undo stack corrupted" );
	return model;
	}

//=======================================================================================
// Manager

LotonController::ControllerManager & LotonController::manager()
	{
	static LotonController::ControllerManager instance;
	return instance;
	}

LotonController::ControllerManager::ControllerManager()
	: registry()
	, undoStack( new QUndoStack )
	, undoView( new QUndoView( undoStack.get() ) )
	{
#ifndef NDEBUG
	undoView->setWindowTitle( "Command List" );
	undoView->show();
	undoView->setAttribute( Qt::WA_QuitOnClose, false );
#endif
	}

//=======================================================================================
// Commands

LotonCommand::LotonCommand( LotonController & model, const QVariant & state, const QString & msg )
	: QUndoCommand( msg )
	, _id( model.id() )
	, _state( state ) //State is passed in to avoid double calling state()
	, _previousState( model.previousState() )
	, pushRedoStopper( true )
	{
	}

void LotonCommand::undo()
	{
	auto * controller = LotonController::getControllerFromManager( _id );
	controller->setState( _previousState );
	controller->_previousState = _previousState;
	emit controller->stateChanged();
	}

void LotonCommand::redo()
	{
	// QUndoStack calls redo when commands are pushed, regardless of how stupid
	//     forcing that design on everyone is. This hacky shit keeps that
	//     initial redo from happening.
	if( pushRedoStopper )
		{
		pushRedoStopper = false;
		return;
		}
	auto * controller = LotonController::getControllerFromManager( _id );
	controller->setState( _state );
	controller->_previousState = _state;
	emit controller->stateChanged();
	}

