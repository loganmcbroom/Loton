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
	};
