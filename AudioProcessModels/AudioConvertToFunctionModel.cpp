#include "AudioConvertToFunctionModel.hpp"

#include <QLabel>
#include <QBoxLayout>

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/FunctionData.hpp"
#include "NodeDataTypes/NumberData.hpp"
#include "Widgets/NumberSlider.hpp"

using namespace flan;

AudioConvertToFunctionModel::AudioConvertToFunctionModel()
	: AudioProcessModel()
	, granularityModel( new NumberSliderModel( 4096, 1, NumberSlider::infinity ) )
	{
	granularityModel->setFilter( []( float x ){ return std::round( x ); } );
	QObject::connect( granularityModel.get(), &NumberSliderModel::stateChanged,
			this, &AudioConvertToFunctionModel::updateData );
	}

AudioConvertToFunctionModel::~AudioConvertToFunctionModel() = default;

QString AudioConvertToFunctionModel::caption() const { return "Convert To Function"; }
QString AudioConvertToFunctionModel::name() const { return "Audio Convert To Function"; }

bool AudioConvertToFunctionModel::process()
	{
	if( ! ins[0] ) return false;

	auto in = std::dynamic_pointer_cast<AudioData>( ins[0] )->audio;
	auto granularity = granularityModel->getSliderPosition();

	setFunctor( [in, granularity, cancel = canceller]()
		{
		return std::make_shared<Func1x1Data>( in.convertToFunction( granularity, *cancel ) );
		} );

	return true;
	}

QString AudioConvertToFunctionModel::portCaption( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return "Audio";
			}
	else if( type == PortType::Out )
		switch( index )
			{
			case 0: return "Func1x1";
			}

	return "";
	}
unsigned int AudioConvertToFunctionModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 1;
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType AudioConvertToFunctionModel::dataType( PortType type, PortIndex index ) const
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
			case 0:  return Func1x1Data::Type();
			default: return {"",""};
			}
	else return {"",""};
	}

ControllerPairs AudioConvertToFunctionModel::makeInputControllers()
	{
	auto granularity = new NumberSliderView( granularityModel.get() );
	granularity->setFixedHeight( 30 );
	return {
		{ "Granularity", granularity }
		};
	}

QJsonObject AudioConvertToFunctionModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["granularity"] = granularityModel->save();
	return modelJson;
	}

void AudioConvertToFunctionModel::restore( QJsonObject const & p )
	{
	granularityModel->restore( p["granularity"].toObject() );
	}



