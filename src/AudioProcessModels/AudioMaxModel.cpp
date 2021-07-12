#include "AudioMaxModel.hpp"

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/NumberData.hpp"

using namespace flan;

AudioMaxModel::AudioMaxModel()
	: AudioInfoProcessModel()
	{
	}

bool AudioMaxModel::process()
	{
	if( ! ins[0] ) return false;
	auto in = std::dynamic_pointer_cast<AudioData>( ins[0] )->audio;
	setFunctor( [in]()
		{
		return std::shared_ptr<NodeData>( new NumberData( in.getMaxSampleMagnitude() ) );
		} );
	return true;
	}

QString AudioMaxModel::caption() const { return "Max"; }

QString AudioMaxModel::name() const { return "Audio Max"; }

QString AudioMaxModel::portCaption( PortType type, PortIndex ) const
	{
	if( type == PortType::In )
		return "Audio";
	else
		return "Max";
	}

unsigned int AudioMaxModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 1;
		case PortType::Out: return 1;
		default: return 0;
		}
	}

NodeDataType AudioMaxModel::dataType( PortType type, PortIndex index ) const
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
			case 0:  return NumberData::Type();
			default: return {"",""};
			}
	else return {"",""};
	}
