#pragma once

#include "AudioInfoProcessModel.hpp"

namespace flan { class Audio; };
class AudioData;

struct AudioEnergyModel : public AudioInfoProcessModel
	{
	AudioEnergyModel();

	bool process() override final;

	QString caption() const override;
	QString name() const override;
	QString portCaption(PortType, PortIndex) const override;
	unsigned int nPorts( PortType type ) const override;
	NodeDataType dataType( PortType type, PortIndex index ) const override;
	std::vector<PortIndex> portsRequired() const override { return { 0 }; }
	QString description() const override
		{
		return R"(Find the total energy in the input. Strictly, the sum of the square of each sample.
		)";
		}
	};
