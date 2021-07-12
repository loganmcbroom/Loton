#include "AudioTextureModel.hpp"

#include <QLabel>
#include <QBoxLayout>

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/NumberData.hpp"
#include "NodeDataTypes/FunctionData.hpp"

#include "Widgets/NumberSlider.hpp"
#include "Widgets/OnOffButton.hpp"

using namespace flan;

AudioTextureModel::AudioTextureModel()
	: AudioFeedbackProcessModel()
	, lengthModel( new NumberSliderModel( 1, 0, NumberSlider::infinity ) )
	, eventsPerSecondModel( new NumberSliderModel( 5, 0, NumberSlider::infinity ) )
	, scatterModel( new NumberSliderModel( 0, 0, NumberSlider::infinity ) )
	, recursionModel( new OnOffButtonModel( false ) )
	{
	auto sliderSetup = [this]( NumberSliderModel * m, int port )
		{
		QObject::connect( m, &NumberSliderModel::stateChanged, this, &AudioTextureModel::updateData );
		auto v = new NumberSliderView( m );
		mainLayout->addWidget( v );
		v->setMinimumSize( 64, 20 );
		setToolTipToPort( v, port );
		};

	sliderSetup( lengthModel.get(), 1 );
	sliderSetup( eventsPerSecondModel.get(), 2 );
	sliderSetup( scatterModel.get(), 3 );

	QObject::connect( recursionModel.get(), &OnOffButtonModel::stateChanged, this, &AudioTextureModel::updateData );
	}

AudioTextureModel::~AudioTextureModel() = default;

QString AudioTextureModel::caption() const { return "Texture"; }
QString AudioTextureModel::name() const { return "Audio Texture"; }

bool AudioTextureModel::process()
	{
	if( ! ins[0] ) return false;

	auto in = std::dynamic_pointer_cast<AudioData>( ins[0] );
	auto length				= tryLockingInput<NumberData>( ins[1], lengthModel->getSliderPosition() );
	auto eventsPerSecond	= tryLockingInput<Func1x1Data>( ins[2], eventsPerSecondModel->getSliderPosition() );
	auto scatter			= tryLockingInput<Func1x1Data>( ins[3], scatterModel->getSliderPosition() );
	bool rec				= recursionModel->getButtonPosition();

	if( isFeedbackEnabled() )
		processWithFeedback( [in, length, eventsPerSecond, scatter, rec, c = canceller]( flan::Audio::Mod m )
			{
			return in->audio.texture( length->f, eventsPerSecond->f, scatter->f, m, rec, *c );
			} );
	else
		setFunctor( [in, length, eventsPerSecond, scatter, rec, c = canceller]()
			{
			return std::make_shared<AudioData>( in->audio.texture( length->f, eventsPerSecond->f, scatter->f, nullptr, rec, *c ) );
			} );

	return true;
	}

QString AudioTextureModel::portCaption( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return "Audio";
			case 1: return "Length";
			case 2: return "Event Rate";
			case 3: return "Scatter";
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
unsigned int AudioTextureModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 5;
		case PortType::Out: return 3;
		default: return 0;
		}
	}
NodeDataType AudioTextureModel::dataType( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return AudioData::Type();
			case 1: return NumberData::Type();
			case 2: return Func1x1Data::Type();
			case 3: return Func1x1Data::Type();
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

ControllerPairs AudioTextureModel::makeInputControllers()
	{
	std::vector<QWidget *> views;
	views.push_back( new NumberSliderView( lengthModel.get() ) );
	views.push_back( new NumberSliderView( eventsPerSecondModel.get() ) );
	views.push_back( new NumberSliderView( scatterModel.get() ) );
	views.push_back( new OnOffButtonView( recursionModel.get() ) );

	for( auto v : views ) v->setFixedHeight( 30 );

	return {
		{ portCaption( PortType::In, 1 ), views[0] },
		{ portCaption( PortType::In, 2 ), views[1] },
		{ portCaption( PortType::In, 3 ), views[2] },
		{ "Recursion", views[3] }

		};
	}

QJsonObject AudioTextureModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson[portCaption( PortType::In, 1 )] = lengthModel->save();
	modelJson[portCaption( PortType::In, 2 )] = eventsPerSecondModel->save();
	modelJson[portCaption( PortType::In, 3 )] = scatterModel->save();
	modelJson["Recursion"] = recursionModel->save();
	return modelJson;
	}

void AudioTextureModel::restore( QJsonObject const & p )
	{
	lengthModel			->restore( p[portCaption( PortType::In, 1 )].toObject() );
	eventsPerSecondModel->restore( p[portCaption( PortType::In, 2 )].toObject() );
	scatterModel		->restore( p[portCaption( PortType::In, 3 )].toObject() );
	recursionModel		->restore( p["Recursion"].toObject() );
	}



