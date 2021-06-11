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
	};
