#pragma once

#include <nodes/NodeDataModel>

#include <flan/Audio.h>

using QtNodes::NodeDataType;

class AudioData : public QtNodes::NodeData
{
public:
	AudioData() : audio() {}
	AudioData( const flan::Audio & _audio ) : audio( _audio ) {}

    NodeDataType type() const override { return Type(); }
	static NodeDataType Type() { return {"audio", "Audio"}; }

	flan::Audio audio;
};
