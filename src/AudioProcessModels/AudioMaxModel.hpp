#pragma once

#include "AudioInfoProcessModel.hpp"

namespace flan { class Audio; };
class AudioData;

struct AudioMaxModel : public AudioInfoProcessModel
	{
	AudioMaxModel();

	bool process() override final;

	QString caption() const override;
	QString name() const override;
	QString portCaption(PortType, PortIndex) const override;
	unsigned int nPorts( PortType type ) const override;
	NodeDataType dataType( PortType type, PortIndex index ) const override;
	std::vector<PortIndex> portsRequired() const override { return { 0 }; }
	QString description() const override
		{
		return R"(This returns the size of the largest sample in the input.
		)";
		}
	};
