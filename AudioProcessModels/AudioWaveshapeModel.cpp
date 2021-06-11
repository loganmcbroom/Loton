#include "AudioWaveshapeModel.hpp"

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/FunctionData.hpp"

using namespace flan;

AudioWaveshapeModel::AudioWaveshapeModel()
	: AudioProcessModel()
	{
	}

bool AudioWaveshapeModel::process()
	{
	if( ! ins[0] || ! ins[1] ) return false;

	auto in = std::dynamic_pointer_cast<AudioData>( ins[0] )->audio;
	auto shape = std::dynamic_pointer_cast<Func1x1Data>( ins[1] );

	setFunctor( [in, shape, c = canceller]()
		{
		return std::shared_ptr<NodeData>( new AudioData( in.waveshape( shape->f, *c ) ) );
		});

	return true;
	}


QString AudioWaveshapeModel::caption() const { return "Waveshape"; }
QString AudioWaveshapeModel::name() const { return "Audio Waveshape"; }
QString AudioWaveshapeModel::portCaption( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return "Audio";
			case 1: return "Shaper";
			}
	else if( type == PortType::Out )
		switch( index )
			{
			case 0: return "Audio";
			}

	return "";
	}
unsigned int AudioWaveshapeModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 2;
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType AudioWaveshapeModel::dataType( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return AudioData::Type();
			case 1: return Func1x1Data::Type();
			default: return {"",""};
			}
	else if( type == PortType::Out )
		switch( index )
			{
			case 0:  return AudioData::Type();
			default: return {"",""};
			}
	else return {"",""};
	}



