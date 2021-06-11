#include "AudioGrainSelectModel.hpp"

#include <QLabel>
#include <QBoxLayout>

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/NumberData.hpp"
#include "NodeDataTypes/FunctionData.hpp"

#include "Widgets/NumberSlider.hpp"

using namespace flan;

AudioGrainSelectModel::AudioGrainSelectModel()
	: AudioFeedbackProcessModel()
	, lengthModel( new NumberSliderModel( 1, 0, NumberSlider::infinity ) )
	, grainsPerSecondModel( new NumberSliderModel( 5, 0, NumberSlider::infinity ) )
	, scatterModel( new NumberSliderModel( 0, 0, NumberSlider::infinity ) )
	, selectionModel( new NumberSliderModel( 0, 0, NumberSlider::infinity ) )
	, grainLengthModel( new NumberSliderModel( .1f, 0, NumberSlider::infinity ) )
	, fadeLengthModel( new NumberSliderModel( .05f, 0, NumberSlider::infinity ) )
	{
	auto sliderSetup = [this]( NumberSliderModel * m )
		{
		QObject::connect( m, &NumberSliderModel::stateChanged, this, &AudioGrainSelectModel::updateData );
		auto v = new NumberSliderView( m );
		mainLayout->addWidget( v );
		v->setMinimumSize( 64, 20 );
		};

	sliderSetup( lengthModel.get() );
	sliderSetup( grainsPerSecondModel.get() );
	sliderSetup( scatterModel.get() );
	sliderSetup( selectionModel.get() );
	sliderSetup( grainLengthModel.get() );
	sliderSetup( fadeLengthModel.get() );
	}

AudioGrainSelectModel::~AudioGrainSelectModel() = default;

QString AudioGrainSelectModel::caption() const { return "Grain Select"; }
QString AudioGrainSelectModel::name() const { return "Audio Grain Select"; }

bool AudioGrainSelectModel::process()
	{
	if( ! ins[0] ) return false;

	auto in = std::dynamic_pointer_cast<AudioData>( ins[0] );
	auto length				= tryLockingInput<NumberData>( ins[1], lengthModel->getSliderPosition() );
	auto grainsPerSecond	= tryLockingInput<Func1x1Data>( ins[2], grainsPerSecondModel->getSliderPosition() );
	auto scatter			= tryLockingInput<Func1x1Data>( ins[3], scatterModel->getSliderPosition() );
	auto selection			= tryLockingInput<Func1x1Data>( ins[4], selectionModel->getSliderPosition() );
	auto grainLength		= tryLockingInput<Func1x1Data>( ins[5], grainLengthModel->getSliderPosition() );
	auto fadeLength			= tryLockingInput<Func1x1Data>( ins[6], fadeLengthModel->getSliderPosition() );

	if( isFeedbackEnabled() )
		processWithFeedback( [in, length, grainsPerSecond, scatter, selection, grainLength, fadeLength, c = canceller]( flan::Audio::Mod m ){
			return in->audio.grainSelect( length->f, grainsPerSecond->f, scatter->f, selection->f, grainLength->f, fadeLength->f, m, *c );
			} );
	else
		setFunctor( [in, length, grainsPerSecond, scatter, selection, grainLength, fadeLength, c = canceller]()
			{
			return std::make_shared<AudioData>( in->audio.grainSelect( length->f, grainsPerSecond->f, scatter->f, selection->f, grainLength->f, fadeLength->f, nullptr, *c ) );
			} );

	return true;
	}

QString AudioGrainSelectModel::portCaption( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return "Audio";
			case 1: return "Length";
			case 2: return "Grain Rate";
			case 3: return "Scatter";
			case 4: return "Selection";
			case 5: return "Grain Length";
			case 6: return "Fade Length";
			case 7: return "Feedback";
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
unsigned int AudioGrainSelectModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 8;
		case PortType::Out: return 3;
		default: return 0;
		}
	}
NodeDataType AudioGrainSelectModel::dataType( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return AudioData::Type();
			case 1: return NumberData::Type();
			case 2: return Func1x1Data::Type();
			case 3: return Func1x1Data::Type();
			case 4: return Func1x1Data::Type();
			case 5: return Func1x1Data::Type();
			case 6: return Func1x1Data::Type();
			case 7: return AudioData::Type();
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

ControllerPairs AudioGrainSelectModel::makeInputControllers()
	{
	std::vector<NumberSliderView *> views;
	views.push_back( new NumberSliderView( lengthModel.get() ) );
	views.push_back( new NumberSliderView( grainsPerSecondModel.get() ) );
	views.push_back( new NumberSliderView( scatterModel.get() ) );
	views.push_back( new NumberSliderView( selectionModel.get() ) );
	views.push_back( new NumberSliderView( grainLengthModel.get() ) );
	views.push_back( new NumberSliderView( fadeLengthModel.get() ) );

	for( auto v : views ) v->setFixedHeight( 30 );

	return {
		{ portCaption( PortType::In, 1 ), views[0] },
		{ portCaption( PortType::In, 2 ), views[1] },
		{ portCaption( PortType::In, 3 ), views[2] },
		{ portCaption( PortType::In, 4 ), views[3] },
		{ portCaption( PortType::In, 5 ), views[4] },
		{ portCaption( PortType::In, 6 ), views[5] }
		};
	}

QJsonObject AudioGrainSelectModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson[portCaption( PortType::In, 1 )] = lengthModel->save();
	modelJson[portCaption( PortType::In, 2 )] = grainsPerSecondModel->save();
	modelJson[portCaption( PortType::In, 3 )] = scatterModel->save();
	modelJson[portCaption( PortType::In, 4 )] = selectionModel->save();
	modelJson[portCaption( PortType::In, 5 )] = grainLengthModel->save();
	modelJson[portCaption( PortType::In, 6 )] = fadeLengthModel->save();
	return modelJson;
	}

void AudioGrainSelectModel::restore( QJsonObject const & p )
	{
	lengthModel			->restore( p[portCaption( PortType::In, 1 )].toObject() );
	grainsPerSecondModel->restore( p[portCaption( PortType::In, 2 )].toObject() );
	scatterModel		->restore( p[portCaption( PortType::In, 3 )].toObject() );
	selectionModel		->restore( p[portCaption( PortType::In, 4 )].toObject() );
	grainLengthModel	->restore( p[portCaption( PortType::In, 5 )].toObject() );
	fadeLengthModel		->restore( p[portCaption( PortType::In, 6 )].toObject() );
	}



