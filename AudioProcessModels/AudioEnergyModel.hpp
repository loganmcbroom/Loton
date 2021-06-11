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
	};