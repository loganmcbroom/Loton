#include "AudioConvolveModel.hpp"

#include <QLabel>
#include <QBoxLayout>

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/NumberData.hpp"

using namespace flan;

AudioConvolveModel::AudioConvolveModel()
	: AudioProcessModel()
	{
	}

QString AudioConvolveModel::caption() const { return "Convolve"; }
QString AudioConvolveModel::name() const { return "Audio Convolve"; }

bool AudioConvolveModel::process()
	{
	if( ! ins[0] || !ins[1] ) return false;

	auto in = std::dynamic_pointer_cast<AudioData>( ins[0] )->audio;
	auto ir = std::dynamic_pointer_cast<AudioData>( ins[1] )->audio;

	setFunctor( [in, ir, c = canceller]()
		{
		return std::make_shared<AudioData>( in.convolve( ir, *c ) );
		} );

	return true;
	}

QString AudioConvolveModel::portCaption( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return "Audio";
			case 1: return "IR";
			}
	else if( type == PortType::Out )
		switch( index )
			{
			case 0: return "Audio";
			}

	return "";
	}
unsigned int AudioConvolveModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 2;
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType AudioConvolveModel::dataType( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return AudioData::Type();
			case 1: return AudioData::Type();
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
