#pragma once

#include <nodes/NodeDataModel>

#include <QtConcurrent>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataModel;
using QtNodes::NodeDataType;
using ControllerPairs = std::vector<std::pair<QString, QWidget *>>;

class LED;
class LotonController;
class LotonNodeData;

/*
 * Base class of all Loton nodes
 * Handles communication with nodeeditor and a bit more
 */

class LotonNodeModel : public NodeDataModel
{ Q_OBJECT
public:
	LotonNodeModel();
	virtual ~LotonNodeModel() override;

//	Interface
//	virtual QString caption() const override = 0;
//	virtual QString name() const override = 0;
//	virtual QString portCaption( PortType, PortIndex ) const override = 0;
//	virtual std::shared_ptr<NodeData> outData( PortIndex = 0 ) override = 0;
//	virtual NodeDataType dataType( PortType, PortIndex ) const override = 0;
//	virtual unsigned int nPorts( PortType ) const override = 0;
//	virtual QJsonObject save() const override;
//	virtual void restore( const QJsonObject & p ) override;
//	virtual void inputConnectionCreated( const QtNodes::Connection & ) override;
//	virtual void inputConnectionDeleted( const QtNodes::Connection & ) override;
//	virtual void outputConnectionCreated( const QtNodes::Connection & ) override;
//	virtual void outputConnectionDeleted( const QtNodes::Connection & ) override;
	virtual void wipeOutputs( PortIndex ) = 0;
	virtual void inputsUpdated( std::shared_ptr<NodeData>, PortIndex ) {}
	virtual ControllerPairs makeInputControllers() { return {}; }
	virtual QWidget * makeHeaderWidget() { return nullptr; }
	virtual std::vector<PortIndex> portsRequired() const = 0;
	virtual QString description() const { return ""; }

protected:
	void setInData( std::shared_ptr<NodeData>, PortIndex ) override final;
	bool captionVisible() const override;
	bool portCaptionVisible( PortType, PortIndex ) const override;
	QWidget * embeddedWidget() override { return _embeddedWidget; }
	void clicked() override final;
	std::shared_ptr<LotonNodeData> makeWipe() const;
	bool hasWipedInput() const;
	void setToolTipToPort( QWidget *, PortIndex );
	bool portRequired( PortIndex ) const override;

	//Utility used in derived classes
	template<typename T, typename S>
	static std::shared_ptr<T> tryLockingInput( std::shared_ptr<NodeData> input, S other )
		{
		return input?
			std::static_pointer_cast<T>( input ) :
			std::make_shared<T>( other );
		}

	virtual void setValidationState( QtNodes::NodeValidationState state, QString msg );
	QtNodes::NodeValidationState validationState() const override { return modelValidationState; }
	QString validationMessage() const override { return modelValidationError; }

	QWidget * mainWidget; // Widget used by derived classes
	QWidget * _embeddedWidget;
	QtNodes::NodeValidationState modelValidationState;
	QString modelValidationError;
	std::vector<std::shared_ptr<LotonNodeData>> ins;
	//std::vector<std::unique_ptr<LotonController>> controllers;
};

