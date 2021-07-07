#pragma once

#include "PVOCProcessModel.hpp"

class NumberSliderModel;

struct PVOCSubtractAmplitudesModel : public PVOCProcessModel
	{
	PVOCSubtractAmplitudesModel();

	bool process() override final;

	QString caption() const override;
	QString name() const override;
	QString portCaption(PortType, PortIndex) const override final;
	unsigned int nPorts( PortType type ) const override final;
	NodeDataType dataType( PortType type, PortIndex index ) const override final;
	ControllerPairs makeInputControllers() override final;
	QJsonObject save() const override final;
	void restore( QJsonObject const & p ) override final;
	std::vector<PortIndex> portsRequired() const override { return { 0, 1 }; }
	QString description() const override
		{
		return R"(Subtracts the amplitudes of bins in the second input with those in the first.

Other - PVOC:
	The source PVOC from which to draw amplitudes to subtract.

Amount - 2->1:
	A scalar on the subtraction amount, with zero having no effect. This isn't clamped to [0,1].
		)";
		}

	std::unique_ptr<NumberSliderModel> sliderModel;
	};
