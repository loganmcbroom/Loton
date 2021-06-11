#include "AudioEnergyModel.hpp"

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/NumberData.hpp"

using namespace flan;

AudioEnergyModel::AudioEnergyModel()
	: AudioInfoProcessModel()
	{
	}

bool AudioEnergyModel::process()
	{
	if( ! ins[0] ) return false;
	auto in = std::dynamic_pointer_cast<AudioData>( ins[0] )->audio;
	setFunctor( [in, c = canceller]()
		{
		return std::shared_ptr<NodeData>(
			new NumberData( in.getTotalEnergy( *c ) ) );
		} );
	return true;
	}

QString AudioEnergyModel::caption() const { return "Energy"; }

QString AudioEnergyModel::name() const { return "Audio Energy"; }

QString AudioEnergyModel::portCaption( PortType type, PortIndex ) const
	{
	if( type == PortType::In )
		return "Audio";
	else
		return "Energy";
	}

unsigned int AudioEnergyModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 1;
		case PortType::Out: return 1;
		default: return 0;
		}
	}

NodeDataType AudioEnergyModel::dataType( PortType type, PortIndex index ) const
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
