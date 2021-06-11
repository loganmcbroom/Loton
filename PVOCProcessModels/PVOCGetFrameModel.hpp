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

	std::unique_ptr<NumberSliderModel> frameTimeModel;
	};
