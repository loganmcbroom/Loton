#include "AudioDelayModel.hpp"

#include <QLabel>
#include <QBoxLayout>

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/NumberData.hpp"

#include "Widgets/NumberSlider.hpp"
#include "Widgets/OnOffButton.hpp"

using namespace flan;

AudioDelayModel::AudioDelayModel()
	: AudioFeedbackProcessModel()
	, lengthModel	( new NumberSliderModel( 5, 0, NumberSlider::infinity ) )
	, delayModel	( new NumberSliderModel( 1, 0, NumberSlider::infinity ) )
	, decayAmtModel	( new NumberSliderModel( .5, 0, NumberSlider::infinity ) )
	, recursionModel( new OnOffButtonModel( false ) )
	{
	auto sliderSetup = [this]( NumberSliderModel * m )
		{
		QObject::connect( m, &NumberSliderModel::stateChanged,
			this, &AudioDelayModel::updateData );
		auto v = new NumberSliderView( m );
		mainLayout->addWidget( v );
		v->setMinimumSize( 64, 20 );
		};

	sliderSetup( lengthModel.get() );
	sliderSetup( delayModel.get() );
	sliderSetup( decayAmtModel.get() );

	QObject::connect( recursionModel.get(), &OnOffButtonModel::stateChanged, this, &AudioDelayModel::updateData );
	}

AudioDelayModel::~AudioDelayModel()
	{}

QString AudioDelayModel::caption() const { return "Delay"; }
QString AudioDelayModel::name() const { return "Audio Delay"; }

bool AudioDelayModel::process()
	{
	if( ! ins[0] ) return false;

	auto in = std::dynamic_pointer_cast<AudioData>( ins[0] );
	auto length = tryLockingInput<NumberData>( ins[1], lengthModel->getSliderPosition() );
	auto delay = tryLockingInput<NumberData>( ins[2], delayModel->getSliderPosition() );
	auto decayAmt = tryLockingInput<NumberData>( ins[3], decayAmtModel->getSliderPosition() );
	const bool rec = recursionModel->getButtonPosition();

	if( isFeedbackEnabled() )
		processWithFeedback( [in, length, delay, decayAmt, rec, c = canceller]( flan::Audio::Mod m ){
			return in->audio.delay( length->f, delay->f, decayAmt->f, m, *c );
			} );
	else
		setFunctor( [in, length, delay, decayAmt, c = canceller]()
			{
			return std::make_shared<AudioData>(
				in->audio.delay( length->f, delay->f, decayAmt->f, nullptr, *c ) );
			} );

	return true;
	}

QString AudioDelayModel::portCaption( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return "Audio";
			case 1: return "Length";
			case 2: return "Delay";
			case 3: return "Decay";
			case 4: return "Feedback";
			}
	else if( type == PortType::Out )
		switch( index )
			{
			case 0: return "Audio";
			case 1: return "FB";
			case 2: return "FB Time";
			}

	return "";
	}
unsigned int AudioDelayModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 5;
		case PortType::Out: return 3;
		default: return 0;
		}
	}
NodeDataType AudioDelayModel::dataType( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return AudioData::Type();
			case 1: return NumberData::Type();
			case 2: return NumberData::Type();
			case 3: return NumberData::Type();
			case 4: return AudioData::Type();
			default: return {"",""};
			}
	else if( type == PortType::Out )
		switch( index )
			{
			case 0:  return AudioData::Type();
			case 1:  return AudioData::Type();
			case 2:  return NumberData::Type();
			default: return {"",""};
			}
	else return {"",""};
	}

ControllerPairs AudioDelayModel::makeInputControllers()
	{
	auto delay	= new NumberSliderView( lengthModel.get() );
	auto N		= new NumberSliderView( delayModel.get() );
	auto decay	= new NumberSliderView( decayAmtModel.get() );
	auto rec	= new OnOffButtonView( recursionModel.get() );
	delay->setFixedHeight( 30 );
	N->setFixedHeight( 30 );
	decay->setFixedHeight( 30 );
	rec->setFixedHeight( 30 );
	return {
		{ portCaption( PortType::In, 1 ), delay },
		{ portCaption( PortType::In, 2 ), N },
		{ portCaption( PortType::In, 3 ), decay },
		{ "Recursion", rec }
		};
	}

QJsonObject AudioDelayModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["delay"]	= lengthModel->save();
	modelJson["N"]		= delayModel->save();
	modelJson["decay"]	= decayAmtModel->save();
	modelJson["Recursion"]	= recursionModel->save();
	return modelJson;
	}

void AudioDelayModel::restore( QJsonObject const & p )
	{
	lengthModel->restore( p["delay"].toObject() );
	delayModel->restore( p["N"].toObject() );
	decayAmtModel->restore( p["decay"].toObject() );
	decayAmtModel->restore( p["Recursion"].toObject() );
	}



