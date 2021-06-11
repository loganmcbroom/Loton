#pragma once

#include "LotonNodeModel.hpp"

class PointsInPlaneModel;

class PointsInPlaneTestModel : public LotonNodeModel
{ Q_OBJECT
public:
	PointsInPlaneTestModel();
	~PointsInPlaneTestModel() override;

	virtual QString caption() const override { return "Points In Plane"; }
	virtual QString name() const override { return "Points In Plane"; }

	void setInData( std::shared_ptr<NodeData>, PortIndex ) override {}
	std::shared_ptr<NodeData> outData( PortIndex = 0 ) override { return nullptr; }
	NodeDataType dataType( PortType, PortIndex ) const override { return {"", ""}; }
	unsigned int nPorts( PortType ) const override { return 0; }

	//ControllerPairs makeInputControllers() override;
	QWidget * makeHeaderWidget() override;

	std::unique_ptr<PointsInPlaneModel> pipModel;
};
