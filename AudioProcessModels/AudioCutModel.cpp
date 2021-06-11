#include "AudioCutModel.hpp"

#include <QLabel>
#include <QBoxLayout>

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/NumberData.hpp"

#include "Widgets/NumberSlider.hpp"

using namespace flan;

AudioCutModel::AudioCutModel()
	: AudioProcessModel()
	, leftSliderModel( new NumberSliderModel( 0, 0, NumberSlider::infinity ) )
	, rightSliderModel( new NumberSliderModel( 0, 0, NumberSlider::infinity ) )
	{
	auto sliderSetup = [this]( NumberSliderModel * sliderModel )
		{
		QObject::connect( sliderModel, &NumberSliderModel::stateChanged,
						this, &AudioCutModel::updateData );
		auto sliderView = new NumberSliderView( sliderModel );
		mainLayout->addWidget( sliderView );
		sliderView->setMinimumSize( 64, 20 );
		};

	sliderSetup( leftSliderModel.get() );
	sliderSetup( rightSliderModel.get() );
	}

AudioCutModel::~AudioCutModel() = default;

bool AudioCutModel::process()
	{
	if( ! ins[0] ) return false;

	auto in = std::dynamic_pointer_cast<AudioData>( ins[0] )->audio;
	float leftBound = ins[1]?
		std::dynamic_pointer_cast<NumberData>( ins[1] )->f :
		float( leftSliderModel->getSliderPosition() );

	float rightBound = ins[2]?
		std::dynamic_pointer_cast<NumberData>( ins[2] )->f :
		float( rightSliderModel->getSliderPosition() );

	setFunctor( [in, leftBound, rightBound, c = canceller]()
		{
		return std::shared_ptr<NodeData>( new AudioData( in.cut( leftBound, rightBound, *c ) ) );
		});

	return true;
	}

QString AudioCutModel::caption() const { return "Cut"; }
QString AudioCutModel::name() const { return "Audio Cut"; }

QString AudioCutModel::portCaption( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return "Audio";
			case 1: return "Start";
			case 2: return "End";
			}
	else if( type == PortType::Out )
		switch( index )
			{
			case 0: return "Audio";
			}

	return "";
	}
unsigned int AudioCutModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 3;
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType AudioCutModel::dataType( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return AudioData::Type();
			case 1: return NumberData::Type();
			case 2: return NumberData::Type();
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

ControllerPairs AudioCutModel::makeInputControllers()
	{
	auto leftSlider = new NumberSliderView( leftSliderModel.get() );
	auto rightSlider = new NumberSliderView( rightSliderModel.get() );
	leftSlider->setFixedHeight( 30 );
	rightSlider->setFixedHeight( 30 );
	return
		{
		{ portCaption( PortType::In, 1 ), leftSlider },
		{ portCaption( PortType::In, 2 ), rightSlider }
		};
	}

QJsonObject AudioCutModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["start"] = leftSliderModel->save();
	modelJson["end"] = rightSliderModel->save();
	return modelJson;
	}

void AudioCutModel::restore( QJsonObject const & p )
	{
	leftSliderModel->restore( p["start"].toObject() );
	rightSliderModel->restore( p["end"].toObject() );
	}



