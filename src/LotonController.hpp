#pragma once

#include <unordered_map>

#include <QObject>
#include <Quuid>
#include <QUndoCommand>

#include <nodes/internal/QUuidStdHash.hpp>

/*
 * Base class for objects handling data
 *		Handles saving and restoring
 *		Handles undo stack
 * Subclasses hold primary state.
 * Interface forces subclasses to convert state into QVariant form
 *	which is used for saving/restoring/undo
 */

class QUndoView;
class LotonCommand;

class LotonController : public QObject
{ Q_OBJECT
public:
	struct ControllerManager
		{
		ControllerManager();
		std::unordered_map<QUuid, LotonController *> registry;
		std::unique_ptr<QUndoStack> undoStack;
		std::unique_ptr<QUndoView> undoView;
		};

	LotonController( QVariant initialState, QUuid id = QUuid() );
	virtual ~LotonController();

	QUuid id() const { return _id; }
	void setId( QUuid );

	QJsonObject save() const;
	void restore( const QJsonObject & );

	void pushStateChange( const QString & msg, LotonCommand * command = nullptr );

	virtual QVariant state() const = 0;
	virtual void setState( const QVariant & ) = 0;
	QVariant previousState() { return _previousState; }

	//For when initial state is too complex to pass in construction. Calls state().
	void setInitialState();

	static LotonController * getControllerFromManager( QUuid id );
	static ControllerManager & manager();

signals:
	void stateChanged();
	void stateChangedDynamic();
	void idChanged();

private:
	QUuid _id;
	QVariant _previousState;

	friend class LotonCommand;
};

class LotonCommand : public QUndoCommand
{
public:
	LotonCommand( LotonController & model, const QVariant & state, const QString & msg );

	void undo() override;
	void redo() override;

protected:
	QUuid _id;
	QVariant _state;
	QVariant _previousState;
	bool pushRedoStopper;
};


