#pragma once

#include "LotonNodeModel.hpp"

class LotonNodeData;
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

	virtual void updateOutput() = 0;

	QString portCaption( PortType, PortIndex ) const override { return ""; }
	bool portCaptionVisible( PortType, PortIndex ) const override { return true; }
	unsigned int nPorts( PortType ) const override { return 1; }
	void inputsUpdated( std::shared_ptr<NodeData>, PortIndex ) override final;
	void wipeOutputs( PortIndex ) override;
	std::shared_ptr<NodeData> outData( PortIndex = 0 ) override final;
	std::vector<PortIndex> portsRequired() const override { return {}; }

protected:
	std::weak_ptr<NumberData> in;
	std::shared_ptr<LotonNodeData> out;
	std::shared_ptr<LotonNodeData> uncomposedOut; // This works for now, if full composition returns we need to check in type instead

signals:
	void dataUpdatedDynamic();
};




class Function1x1NodeModel : public FunctionNodeModel
{ Q_OBJECT
public:
	Function1x1NodeModel();
	virtual ~Function1x1NodeModel() override;

	void updateOutput() override;

protected:
	virtual QString caption() const override = 0;
	virtual QString name() const override = 0;
	virtual std::shared_ptr<Func1x1Data> function() = 0;
	NodeDataType dataType( PortType, PortIndex ) const override;
};

class Function2x1NodeModel : public FunctionNodeModel
{ Q_OBJECT
public:
	Function2x1NodeModel();
	virtual ~Function2x1NodeModel() override;
	void updateOutput() override;

protected:
	virtual QString caption() const override = 0;
	virtual QString name() const override = 0;
	virtual std::shared_ptr<Func2x1Data> function() = 0;
	NodeDataType dataType( PortType, PortIndex ) const override;
};

class Function2x2NodeModel : public FunctionNodeModel
{ Q_OBJECT
public:
	Function2x2NodeModel();
	virtual ~Function2x2NodeModel() override;
	void updateOutput() override;

protected:
	virtual QString caption() const override = 0;
	virtual QString name() const override = 0;
	virtual std::shared_ptr<Func2x2Data> function() = 0;
	NodeDataType dataType( PortType, PortIndex ) const override;
};
