#pragma once

#include "LotonNodeData.hpp"

#include <flan/Audio.h>

class AudioData : public LotonNodeData
{
public:
	AudioData() : audio() {}
	AudioData( const flan::Audio & _audio ) : audio( _audio ) {}

    NodeDataType type() const override { return Type(); }
	static NodeDataType Type() { return {"audio", "Audio"}; }

	flan::Audio audio;
};
