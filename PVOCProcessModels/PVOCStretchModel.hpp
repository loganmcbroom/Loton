#pragma once

#include "PVOCProcessModel.hpp"

class NumberSliderModel;
class InterpolatorModel;
class LotonComboBoxModel;

class PVOCStretchModel : public PVOCProcessModel
{
public:
	PVOCStretchModel();

	bool process() override;

	QString caption() const override;
	QString name() const override;
	QString portCaption(PortType, PortIndex) const override;
	unsigned int nPorts( PortType type ) const override;
	NodeDataType dataType( PortType type, PortIndex index ) const override;
	ControllerPairs makeInputControllers() override;
	std::vector<PortIndex> portsRequired() const override { return { 0 }; }
	QString description() const override
		{
		return R"(This scales the input time at each input point.

Factor - 2->1:
	Takes time/frequency pairs and returns a time multiplier.
		)";
		}

	QJsonObject save() const override;
	void restore( QJsonObject const & p ) override;

	std::unique_ptr<NumberSliderModel> factorModel;
	std::unique_ptr<InterpolatorModel> interpModel;
};
