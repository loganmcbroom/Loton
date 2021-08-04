#include "AudioCutModel.hpp"

#include <QLabel>
#include <QBoxLayout>

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/NumberData.hpp"

#include "Widgets/NumberSlider.hpp"
#include "Widgets/OnOffButton.hpp"

using namespace flan;

AudioCutModel::AudioCutModel()
	: AudioProcessModel()
	, leftSliderModel( new NumberSliderModel( 0, 0, NumberSlider::infinity ) )
	, rightSliderModel( new NumberSliderModel( 0, 0, NumberSlider::infinity ) )
	, lengthModeButtonModel( new OnOffButtonModel( false ) )
	{
	auto sliderSetup = [this]( NumberSliderModel * m, int port )
		{
		QObject::connect( m, &NumberSliderModel::stateChanged, this, &AudioCutModel::updateData );
		auto v = new NumberSliderView( m );
		mainLayout->addWidget( v );
		v->setMinimumSize( 64, 20 );
		setToolTipToPort( v, port );
		};

	sliderSetup( leftSliderModel.get(), 1 );
	sliderSetup( rightSliderModel.get(), 2 );

	QObject::connect( lengthModeButtonModel.get(), &NumberSliderModel::stateChanged, this, &AudioCutModel::updateData );
	auto v = new OnOffButtonView( lengthModeButtonModel.get() );
	mainLayout->addWidget( v );
	v->setMinimumSize( 64, 20 );
	setToolTipToPort( v, 3 );
	}

AudioCutModel::~AudioCutModel() = default;

bool AudioCutModel::process()
	{
	if( ! ins[0] ) return false;

	auto in = std::dynamic_pointer_cast<AudioData>( ins[0] );
	auto leftBound = tryLockingInput<NumberData>( ins[1], leftSliderModel->getSliderPosition() );
	auto rightBound = tryLockingInput<NumberData>( ins[2], rightSliderModel->getSliderPosition() );
	const bool lengthMode = lengthModeButtonModel->getButtonPosition();

	setFunctor( [in, leftBound, rightBound, lengthMode, c = canceller]()
		{
		return std::make_shared<AudioData>( in->audio.cut( leftBound->f, lengthMode? rightBound->f + leftBound->f : rightBound->f, *c ) );
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
			case 2: return "End/Length";
			case 3: return "Mode";
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
		{ portCaption( PortType::In, 2 ), rightSlider },
		{ portCaption( PortType::In, 3 ), new OnOffButtonView( lengthModeButtonModel.get() ) }
		};
	}

QJsonObject AudioCutModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["start"] = leftSliderModel->save();
	modelJson["end"] = rightSliderModel->save();
	modelJson["mode"] = lengthModeButtonModel->save();
	return modelJson;
	}

void AudioCutModel::restore( QJsonObject const & p )
	{
	leftSliderModel->restore( p["start"].toObject() );
	rightSliderModel->restore( p["end"].toObject() );
	lengthModeButtonModel->restore( p["mode"].toObject() );
	}



