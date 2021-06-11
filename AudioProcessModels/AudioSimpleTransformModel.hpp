#pragma once

#include "AudioProcessModel.hpp"

/*
 * Base class for audio transforms taking no parameters
 */

namespace flan { class Audio; };
class AudioData;

struct AudioSimpleTransformModel : public AudioProcessModel
	{
	AudioSimpleTransformModel();
	virtual ~AudioSimpleTransformModel() override {}

	virtual std::function< std::shared_ptr<AudioData> () > getMethod( flan::Audio ) const = 0;

	bool process() override final;
	QString portCaption(PortType, PortIndex) const override;
	unsigned int nPorts( PortType type ) const override;
	NodeDataType dataType( PortType type, PortIndex index ) const override;
	};

