#include "AudioTextureSimpleModel.hpp"

#include <QLabel>
#include <QBoxLayout>

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/NumberData.hpp"
#include "NodeDataTypes/FunctionData.hpp"

#include "Widgets/NumberSlider.hpp"

using namespace flan;

AudioTextureSimpleModel::AudioTextureSimpleModel()
	: AudioProcessModel()
	, lengthModel( new NumberSliderModel( 1, 0, NumberSlider::infinity ) )
	, eventsPerSecondModel( new NumberSliderModel( 5, 0, NumberSlider::infinity ) )
	, scatterModel( new NumberSliderModel( 0, 0, NumberSlider::infinity ) )
	, repitchModel( new NumberSliderModel( 1, 0, NumberSlider::infinity ) )
	, gainModel( new NumberSliderModel( 1, 0, NumberSlider::infinity ) )
	, eventLengthModel( new NumberSliderModel( .1, 0, NumberSlider::infinity ) )
	, panModel( new NumberSliderModel( 0, -1, 1 ) )
	{
	auto sliderSetup = [this]( NumberSliderModel * m )
		{
		QObject::connect( m, &NumberSliderModel::stateChanged, this, &AudioTextureSimpleModel::updateData );
		auto v = new NumberSliderView( m );
		mainLayout->addWidget( v );
		v->setMinimumSize( 64, 20 );
		};

	sliderSetup( lengthModel.get() );
	sliderSetup( eventsPerSecondModel.get() );
	sliderSetup( scatterModel.get() );
	sliderSetup( repitchModel.get() );
	sliderSetup( gainModel.get() );
	sliderSetup( eventLengthModel.get() );
	sliderSetup( panModel.get() );
	}

AudioTextureSimpleModel::~AudioTextureSimpleModel() = default;

QString AudioTextureSimpleModel::caption() const { return "Texture Simple"; }
QString AudioTextureSimpleModel::name() const { return "Audio Texture Simple"; }

bool AudioTextureSimpleModel::process()
	{
	if( ! ins[0] ) return false;

	auto in = std::dynamic_pointer_cast<AudioData>( ins[0] )->audio;
	auto length				= tryLockingInput<NumberData>( ins[1], lengthModel->getSliderPosition() );
	auto eventsPerSecond	= tryLockingInput<Func1x1Data>( ins[2], eventsPerSecondModel->getSliderPosition() );
	auto scatter			= tryLockingInput<Func1x1Data>( ins[3], scatterModel->getSliderPosition() );
	auto repitch			= tryLockingInput<Func1x1Data>( ins[4], repitchModel->getSliderPosition() );
	auto gain				= tryLockingInput<Func1x1Data>( ins[5], gainModel->getSliderPosition() );
	auto eventLength		= tryLockingInput<Func1x1Data>( ins[6], eventLengthModel->getSliderPosition() );
	auto pan				= tryLockingInput<Func1x1Data>( ins[7], panModel->getSliderPosition() );

	setFunctor( [in, length, eventsPerSecond, scatter, repitch, gain, eventLength, pan, c = canceller]()
		{
		return std::make_shared<AudioData>( in.textureSimple( length->f, eventsPerSecond->f, scatter->f, repitch->f, gain->f, eventLength->f, pan->f, *c ) );
		} );

	return true;
	}

QString AudioTextureSimpleModel::portCaption( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return "Audio";
			case 1: return "Length";
			case 2: return "Event Rate";
			case 3: return "Scatter";
			case 4: return "Repitch";
			case 5: return "Gain";
			case 6: return "Event Length";
			case 7: return "Pan";
			}
	else if( type == PortType::Out )
		switch( index )
			{
			case 0: return "Audio";
			}

	return "";
	}
unsigned int AudioTextureSimpleModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 8;
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType AudioTextureSimpleModel::dataType( PortType type, PortIndex index ) const
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
			case 7: return Func1x1Data::Type();
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

ControllerPairs AudioTextureSimpleModel::makeInputControllers()
	{
	std::vector<NumberSliderView *> views;
	views.push_back( new NumberSliderView( lengthModel.get() ) );
	views.push_back( new NumberSliderView( eventsPerSecondModel.get() ) );
	views.push_back( new NumberSliderView( scatterModel.get() ) );
	views.push_back( new NumberSliderView( repitchModel.get() ) );
	views.push_back( new NumberSliderView( gainModel.get() ) );
	views.push_back( new NumberSliderView( eventLengthModel.get() ) );
	views.push_back( new NumberSliderView( panModel.get() ) );

	for( auto v : views ) v->setFixedHeight( 30 );

	return {
		{ portCaption( PortType::In, 1 ), views[0] },
		{ portCaption( PortType::In, 2 ), views[1] },
		{ portCaption( PortType::In, 3 ), views[2] },
		{ portCaption( PortType::In, 4 ), views[3] },
		{ portCaption( PortType::In, 5 ), views[4] },
		{ portCaption( PortType::In, 6 ), views[5] },
		{ portCaption( PortType::In, 7 ), views[6] }
		};
	}

QJsonObject AudioTextureSimpleModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson[portCaption( PortType::In, 1 )] = lengthModel->save();
	modelJson[portCaption( PortType::In, 2 )] = eventsPerSecondModel->save();
	modelJson[portCaption( PortType::In, 3 )] = scatterModel->save();
	modelJson[portCaption( PortType::In, 4 )] = repitchModel->save();
	modelJson[portCaption( PortType::In, 5 )] = gainModel->save();
	modelJson[portCaption( PortType::In, 6 )] = eventLengthModel->save();
	modelJson[portCaption( PortType::In, 7 )] = panModel->save();
	return modelJson;
	}

void AudioTextureSimpleModel::restore( QJsonObject const & p )
	{
	lengthModel			->restore( p[portCaption( PortType::In, 1 )].toObject() );
	eventsPerSecondModel->restore( p[portCaption( PortType::In, 2 )].toObject() );
	scatterModel		->restore( p[portCaption( PortType::In, 3 )].toObject() );
	repitchModel		->restore( p[portCaption( PortType::In, 4 )].toObject() );
	gainModel			->restore( p[portCaption( PortType::In, 5 )].toObject() );
	eventLengthModel	->restore( p[portCaption( PortType::In, 6 )].toObject() );
	panModel			->restore( p[portCaption( PortType::In, 7 )].toObject() );
	}



