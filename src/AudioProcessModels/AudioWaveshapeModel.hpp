#pragma once

#include "AudioProcessModel.hpp"

class NumberSliderModel;

struct AudioWaveshapeModel : public AudioProcessModel
	{
	AudioWaveshapeModel();

	bool process() override;

	QString caption() const override;
	QString name() const override;
	QString portCaption(PortType, PortIndex) const override;
	unsigned int nPorts( PortType type ) const override;
	NodeDataType dataType( PortType type, PortIndex index ) const override;
	std::vector<PortIndex> portsRequired() const override { return { 0, 1 }; }
	QString description() const override
		{
		return R"(This applies the provided shaper as a function to each sample in the input.

Shaper - 1->1:
	Each sample in the input is passed through this.
	Samples will be values on [-1,1] under normal circumstances.
	For example, the function y = x would have no effect as a shaper.
		)";
		}
	};
