#include "AudioLengthModel.hpp"

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/NumberData.hpp"

using namespace flan;

AudioLengthModel::AudioLengthModel()
	: AudioInfoProcessModel()
	{
	}

bool AudioLengthModel::process()
	{
	if( ! ins[0] ) return false;
	auto in = std::dynamic_pointer_cast<AudioData>( ins[0] )->audio;
	setFunctor( [in]()
		{
		return std::shared_ptr<NodeData>( new NumberData( in.getLength() ) );
		} );
	return true;
	}

QString AudioLengthModel::caption() const { return "Length"; }

QString AudioLengthModel::name() const { return "Audio Length"; }

QString AudioLengthModel::portCaption( PortType type, PortIndex ) const
	{
	if( type == PortType::In )
		return "Audio";
	else
		return "Length";
	}

unsigned int AudioLengthModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 1;
		case PortType::Out: return 1;
		default: return 0;
		}
	}

NodeDataType AudioLengthModel::dataType( PortType type, PortIndex index ) const
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
