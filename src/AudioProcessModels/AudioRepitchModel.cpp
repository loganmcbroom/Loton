#include "AudioRepitchModel.hpp"

#include <QLabel>
#include <QBoxLayout>

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/FunctionData.hpp"
#include "NodeDataTypes/NumberData.hpp"

#include "Widgets/NumberSlider.hpp"
#include "Widgets/ComboBox.hpp"

using namespace flan;


AudioRepitchModel::AudioRepitchModel()
	: AudioProcessModel()
	, factorModel( new NumberSliderModel( 1, 0.01f, NumberSlider::infinity ) )
	, granularityModel( new NumberSliderModel( 64, 1, NumberSlider::infinity ) )
	, qualityModel( new LotonComboBoxModel( 0, { "Sinc", "Fast", "Bad" } ) )
	{

	granularityModel->setFilter( []( float x ){ return std::round( x ); } );

	QObject::connect( factorModel.get(), &NumberSliderModel::stateChanged,
					this, &AudioRepitchModel::updateData );
	QObject::connect( granularityModel.get(), &NumberSliderModel::stateChanged,
					this, &AudioRepitchModel::updateData );
	QObject::connect( qualityModel.get(), &LotonComboBoxModel::stateChanged,
					this, &AudioRepitchModel::updateData );

	auto sliderView = new NumberSliderView( factorModel.get() );
	mainLayout->addWidget( sliderView );
	sliderView->setMinimumSize( 64, 20 );
	}

AudioRepitchModel::~AudioRepitchModel() = default;

bool AudioRepitchModel::process()
	{
	if( ! ins[0] ) return false;

	auto in = std::dynamic_pointer_cast<AudioData>( ins[0] )->audio;
	auto factor = tryLockingInput<Func1x1Data>( ins[1], factorModel->getSliderPosition() );
	auto granularity = granularityModel->getSliderPosition();
	auto quality = qualityModel->selection();

	setFunctor( [in, factor, granularity, quality, c = canceller]()
		{
		return std::shared_ptr<NodeData>( new AudioData( in.repitch(
			factor->f, granularity * in.frameToTime(), quality, *c ) ) );
		});

	return true;
	}


QString AudioRepitchModel::caption() const { return "Repitch"; }
QString AudioRepitchModel::name() const { return "Audio Repitch"; }
QString AudioRepitchModel::portCaption( PortType type, PortIndex index ) const
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
unsigned int AudioRepitchModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 2;
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType AudioRepitchModel::dataType( PortType type, PortIndex index ) const
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

ControllerPairs AudioRepitchModel::makeInputControllers()
	{
	auto factor = new NumberSliderView( factorModel.get() );
	auto granularity = new NumberSliderView( granularityModel.get() );
	auto quality = new LotonComboBoxView( qualityModel.get() );
	factor->setFixedHeight( 30 );
	granularity->setFixedHeight( 30 );
	quality->setFixedHeight( 30 );
	return {
		{ portCaption( PortType::In, 1 ), factor },
		{ "Granularity", granularity },
		{ "Quality", quality }
		};
	}

QJsonObject AudioRepitchModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["factor"] = factorModel->save();
	modelJson["granularity"] = granularityModel->save();
	modelJson["quality"] = qualityModel->save();
	return modelJson;
	}

void AudioRepitchModel::restore( QJsonObject const & p )
	{
	factorModel->restore( p["factor"].toObject() );
	granularityModel->restore( p["granularity"].toObject() );
	qualityModel->restore( p["quality"].toObject() );
	}



