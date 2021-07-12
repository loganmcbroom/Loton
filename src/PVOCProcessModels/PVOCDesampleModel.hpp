#pragma once

#include "PVOCProcessModel.hpp"

class NumberSliderModel;
class InterpolatorModel;

class PVOCDesampleModel : public PVOCProcessModel
{
public:
	PVOCDesampleModel();

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
		return R"(This reduces time clarity by removing a portion of the data and replacing it with an interpolation of the surrounding data.

Factor - 2->1:
	The amount of data to discard. For example, a factor of two will discard every other data point.
		)";
		}

	QJsonObject save() const override;
	void restore( QJsonObject const & p ) override;

	std::unique_ptr<NumberSliderModel> factorModel;
	std::unique_ptr<InterpolatorModel> interpModel;
};
