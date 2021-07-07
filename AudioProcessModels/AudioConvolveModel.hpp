#pragma once

#include "AudioProcessModel.hpp"

struct AudioConvolveModel : public AudioProcessModel
	{
	AudioConvolveModel();

	bool process() override final;

	QString caption() const override;
	QString name() const override;
	QString portCaption(PortType, PortIndex) const override final;
	unsigned int nPorts( PortType type ) const override final;
	NodeDataType dataType( PortType type, PortIndex index ) const override final;
	std::vector<PortIndex> portsRequired() const override { return { 0, 1 }; }
	QString description() const override
		{
		return R"(Regular signal convolution.

IR - Audio:
	The other signal to convolve.
		)";
		}
	};
