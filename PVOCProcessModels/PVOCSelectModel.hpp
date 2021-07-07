#pragma once

#include "PVOCProcessModel.hpp"

class NumberSliderModel;
class InterpolatorModel;
class Func1x1Data;

struct PVOCSelectModel : public PVOCProcessModel
	{
	PVOCSelectModel();

	bool process() override final;

	QString caption() const override;
	QString name() const override;
	QString portCaption(PortType, PortIndex) const override final;
	unsigned int nPorts( PortType type ) const override final;
	NodeDataType dataType( PortType type, PortIndex index ) const override final;
	ControllerPairs makeInputControllers() override final;
	QJsonObject save() const override final;
	void restore( QJsonObject const & p ) override final;
	std::vector<PortIndex> portsRequired() const override { return { 0, 2 }; }
	QString description() const override
		{
		return R"(Every time/frequency point in the output PVOC will read an arbitrary point in the input, determined by Selector.
Selected non-integer coordinates will interpolate surrounding integer coordinates.

Length - Number:
	The output length.

Selector - 2->2:
	This function takes each time/frequency point in the output and returns the time/frequency that should be read from the input into that output.
		)";
		}

	std::unique_ptr<NumberSliderModel> lengthModel;
	std::unique_ptr<InterpolatorModel> interpModel;
	};
