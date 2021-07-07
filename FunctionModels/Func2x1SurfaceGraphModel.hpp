#pragma once

#include "LotonNodeModel.hpp"

class SurfaceGraphModel;
class NumberSliderModel;
class Func2x1Data;

class Func2x1SurfaceGraphModel : public LotonNodeModel
{ Q_OBJECT
public:
	Func2x1SurfaceGraphModel();
	~Func2x1SurfaceGraphModel() override;

	void updateGraphModel();

	virtual QString caption() const override;
	virtual QString name() const override;
	virtual QString portCaption( PortType, PortIndex ) const override;
	virtual void inputsUpdated( std::shared_ptr<NodeData>, PortIndex ) override;
	virtual void wipeOutputs( PortIndex ) override;
	virtual std::shared_ptr<NodeData> outData( PortIndex = 0 ) override;
	virtual NodeDataType dataType( PortType, PortIndex ) const override;
	virtual unsigned int nPorts( PortType ) const override;
	virtual QJsonObject save() const override;
	virtual void restore( QJsonObject const & p ) override;
	virtual void inputConnectionDeleted( PortIndex ) override;
	virtual ControllerPairs makeInputControllers() override;
	virtual QWidget * makeHeaderWidget() override;
	std::vector<PortIndex> portsRequired() const override { return { 0 }; }
	QString description() const override
		{
		return R"(This provides a 3D graph of the input surface in the node view.

Input - 2->1:
	The surface to graph.
		)";
		}

	std::shared_ptr<NodeData> out;
	std::unique_ptr<SurfaceGraphModel> graphModel;
	std::unique_ptr<NumberSliderModel> timeMax, freqMax;
};


