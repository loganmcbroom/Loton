#include "AudioSimpleTransformModel.hpp"

#include "NodeDataTypes/AudioData.hpp"

using namespace flan;

AudioSimpleTransformModel::AudioSimpleTransformModel()
	: AudioProcessModel()
	{
	}

bool AudioSimpleTransformModel::process()
	{
	if( ! ins[0] ) return false;
	auto in = std::dynamic_pointer_cast<AudioData>( ins[0] )->audio;
	setFunctor( getMethod( in ) );
	return true;
	}

QString AudioSimpleTransformModel::portCaption( PortType, PortIndex ) const
	{
	return "Audio";
	}

unsigned int AudioSimpleTransformModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 1;
		case PortType::Out: return 1;
		default: return 0;
		}
	}

NodeDataType AudioSimpleTransformModel::dataType( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return AudioData::Type();
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
