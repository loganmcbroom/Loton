#pragma once

#include "PVOCProcessModel.hpp"

class NumberSliderModel;
class InterpolatorModel;

class PVOCResonateModel : public PVOCProcessModel
{
public:
	PVOCResonateModel();

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
		return R"(Each bin has its frequency copied to subsequent output bins with decaying magnitude until a bin with magnitude greater than the current decayed magnitude is read from the input.
The frequency of the output at that point then changes to that of the louder bin.

Length - Number:
   How much time after the input should be generated. Because the decay is exponential, a set tail length is needed.

Decay - 2->1:
	This returns the decay amount per second at every time/frequency input.
	For example, a constant decay of .5 applied to an impulse will lose half it's magnitude every second.
		)";
		}

	QJsonObject save() const override;
	void restore( QJsonObject const & p ) override;

	std::unique_ptr<NumberSliderModel> lengthModel;
	std::unique_ptr<NumberSliderModel> resTimeModel;
};
