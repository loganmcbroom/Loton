#pragma once

#include "PVOCProcessModel.hpp"

class NumberSliderModel;

struct PVOCGetFrameModel : public PVOCProcessModel
	{
	PVOCGetFrameModel();
	~PVOCGetFrameModel() override;

	bool process() override final;

	QString caption() const override;
	QString name() const override;
	QString portCaption(PortType, PortIndex) const override final;
	unsigned int nPorts( PortType type ) const override final;
	NodeDataType dataType( PortType type, PortIndex index ) const override final;
	ControllerPairs makeInputControllers() override final;
	QJsonObject save() const override final;
	void restore( QJsonObject const & p ) override final;
	std::vector<PortIndex> portsRequired() const override { return { 0 }; }
	QString description() const override
		{
		return R"(Returns a single frame from the input.

Time - Number:
	The time at which the frame should be taken.
		)";
		}

	std::unique_ptr<NumberSliderModel> frameTimeModel;
	};
