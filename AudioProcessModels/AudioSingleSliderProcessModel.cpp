#include "AudioSingleSliderProcessModel.hpp"

#include <QLabel>
#include <QBoxLayout>

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/FunctionData.hpp"

#include "Widgets/NumberSlider.hpp"

using namespace flan;

AudioSingleSliderProcessModel::AudioSingleSliderProcessModel( double sliderLow, double sliderHigh, double sliderCenter )
	: AudioProcessModel()
	, sliderModel( new NumberSliderModel( sliderCenter, sliderLow, sliderHigh ) )
	{
	QObject::connect( sliderModel.get(), &NumberSliderModel::stateChanged,
					this, &AudioSingleSliderProcessModel::updateData );

	auto sliderView = new NumberSliderView( sliderModel.get() );
	mainLayout->addWidget( sliderView );
	sliderView->setMinimumSize( 64, 20 );
	sliderView->setToolTip( "Factor" );
	}

AudioSingleSliderProcessModel::~AudioSingleSliderProcessModel() = default;

bool AudioSingleSliderProcessModel::process()
	{
	if( ! ins[0] ) return false;

	auto in = std::dynamic_pointer_cast<AudioData>( ins[0] )->audio;
	auto shift = ins[1]?
		std::dynamic_pointer_cast<Func1x1Data>( ins[1] ) :
		std::make_shared<Func1x1Data>( sliderModel->getSliderPosition() );

	setFunctor( getMethod( in, shift ) );

	return true;
	}

QString AudioSingleSliderProcessModel::portCaption( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return "Audio";
			case 1: return "Factor";
			}
	else if( type == PortType::Out )
		switch( index )
			{
			case 0: return "Audio";
			}

	return "";
	}
unsigned int AudioSingleSliderProcessModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 2;
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType AudioSingleSliderProcessModel::dataType( PortType type, PortIndex index ) const
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

ControllerPairs AudioSingleSliderProcessModel::makeInputControllers()
	{
	auto slider = new NumberSliderView( sliderModel.get() );
	slider->setFixedHeight( 30 );
	return {{ portCaption( PortType::In, 1 ), slider }};
	}

QJsonObject AudioSingleSliderProcessModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["slider"] = sliderModel->save();
	return modelJson;
	}

void AudioSingleSliderProcessModel::restore( QJsonObject const & p )
	{
	sliderModel->restore( p["slider"].toObject() );
	}



