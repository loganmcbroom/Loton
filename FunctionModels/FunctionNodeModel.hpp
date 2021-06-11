#pragma once

#include "LotonNodeModel.hpp"

class NumberData;
class Func1x1Data;
class Func2x1Data;
class Func2x2Data;

/*
 * Base class for all function nodes
 */

class FunctionNodeModel : public LotonNodeModel
{ Q_OBJECT
public:
	FunctionNodeModel();
	virtual ~FunctionNodeModel() override;

	void updateOutput();

signals:
	void dataUpdatedDynamic();

protected:
	virtual QString caption() const override = 0;
	virtual QString name() const override = 0;
	virtual std::shared_ptr<Func1x1Data> function() = 0;

	QString portCaption( PortType, PortIndex ) const override { return ""; }
	bool portCaptionVisible( PortType, PortIndex ) const override { return true; }
	unsigned int nPorts( PortType t ) const override { return 1; }

	void setInData( std::shared_ptr<NodeData>, PortIndex ) override;
	std::shared_ptr<NodeData> outData( PortIndex = 0 ) override;
	NodeDataType dataType( PortType, PortIndex ) const override;
	//ControllerPairs makeInputControllers() override;
	//QWidget * makeHeaderWidget() override;
	//QJsonObject save() const override;
	//void restore( QJsonObject const & p ) override;

	std::weak_ptr<NumberData> in;
	std::shared_ptr<Func1x1Data> out;
	std::shared_ptr<Func1x1Data> uncomposedOut; // This works for now, if full composition returns we need to check in type instead
};

class Function2x1NodeModel : public LotonNodeModel
{ Q_OBJECT
public:
	Function2x1NodeModel();
	virtual ~Function2x1NodeModel() override;

	void updateOutput();

signals:
	void dataUpdatedDynamic();

protected:
	virtual QString caption() const override = 0;
	virtual QString name() const override = 0;
	virtual std::shared_ptr<Func2x1Data> function() = 0;

	QString portCaption( PortType, PortIndex ) const override { return ""; }
	bool portCaptionVisible( PortType, PortIndex ) const override { return true; }
	unsigned int nPorts( PortType t ) const override { return 1; }

	void setInData( std::shared_ptr<NodeData>, PortIndex ) override;
	std::shared_ptr<NodeData> outData( PortIndex = 0 ) override;
	NodeDataType dataType( PortType, PortIndex ) const override;

	std::weak_ptr<NumberData> in;
	std::shared_ptr<Func2x1Data> out;
	std::shared_ptr<Func2x1Data> uncomposedOut;
};

class Function2x2NodeModel : public LotonNodeModel
{ Q_OBJECT
public:
	Function2x2NodeModel();
	virtual ~Function2x2NodeModel() override;

	void updateOutput();

signals:
	void dataUpdatedDynamic();

protected:
	virtual QString caption() const override = 0;
	virtual QString name() const override = 0;
	virtual std::shared_ptr<Func2x2Data> function() = 0;

	QString portCaption( PortType, PortIndex ) const override { return ""; }
	bool portCaptionVisible( PortType, PortIndex ) const override { return true; }
	unsigned int nPorts( PortType t ) const override { return 1; }

	void setInData( std::shared_ptr<NodeData>, PortIndex ) override;
	std::shared_ptr<NodeData> outData( PortIndex = 0 ) override;
	NodeDataType dataType( PortType, PortIndex ) const override;

	std::weak_ptr<NumberData> in;
	std::shared_ptr<Func2x2Data> out;
	std::shared_ptr<Func2x2Data> uncomposedOut;
};
