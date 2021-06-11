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
//	virtual void setInData( std::shared_ptr<NodeData>, PortIndex ) override = 0;
//	virtual std::shared_ptr<NodeData> outData( PortIndex = 0 ) override = 0;
//	virtual NodeDataType dataType( PortType, PortIndex ) const override = 0;
//	virtual unsigned int nPorts( PortType ) const override = 0;
//	virtual QJsonObject save() const override;
//	virtual void restore( const QJsonObject & p ) override;
//	virtual void inputConnectionCreated( const QtNodes::Connection & ) override;
//	virtual void inputConnectionDeleted( const QtNodes::Connection & ) override;
//	virtual void outputConnectionCreated( const QtNodes::Connection & ) override;
//	virtual void outputConnectionDeleted( const QtNodes::Connection & ) override;
	virtual ControllerPairs makeInputControllers() { return {}; }
	virtual QWidget * makeHeaderWidget() { return nullptr; }

protected:
	QWidget * mainWidget; //Widget used by derived classes

	bool captionVisible() const override;
	bool portCaptionVisible( PortType, PortIndex ) const override;
	QWidget * embeddedWidget() override { return _embeddedWidget; }
	virtual void clicked() override final;

	virtual void setValidationState( QtNodes::NodeValidationState state, QString msg );
	QtNodes::NodeValidationState validationState() const override { return modelValidationState; }
	QString validationMessage() const override { return modelValidationError; }

	QWidget * _embeddedWidget;
	QtNodes::NodeValidationState modelValidationState;
	QString modelValidationError;

	//std::vector<std::unique_ptr<LotonController>> controllers;
};

