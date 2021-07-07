#include "AudioFadesModel.hpp"

#include <QLabel>
#include <QBoxLayout>

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/NumberData.hpp"
#include "NodeDataTypes/InterpolatorData.hpp"

#include "Widgets/NumberSlider.hpp"

using namespace flan;

AudioFadesModel::AudioFadesModel()
	: AudioProcessModel()
	, startLengthModel( new NumberSliderModel( .05f, 0, NumberSlider::infinity ) )
	, endLengthModel( new NumberSliderModel( .05f, 0, NumberSlider::infinity ) )
	, interpModel( new InterpolatorModel( InterpolatorType::squareroot ) )
	{
	auto sliderSetup = [this]( NumberSliderModel * m, int port )
		{
		QObject::connect( m, &NumberSliderModel::stateChanged,
						this, &AudioFadesModel::updateData );

		auto v = new NumberSliderView( m );
		mainLayout->addWidget( v );
		v->setMinimumSize( 64, 20 );
		setToolTipToPort( v, port );
		};

	sliderSetup( startLengthModel.get(), 1 );
	sliderSetup( endLengthModel.get(), 2 );

	QObject::connect( interpModel.get(), &InterpolatorModel::stateChanged,
						this, &AudioFadesModel::updateData );
	auto v = new LotonComboBoxView( interpModel.get() );
	mainLayout->addWidget( v );
	}

AudioFadesModel::~AudioFadesModel() = default;

QString AudioFadesModel::caption() const { return "Fades"; }
QString AudioFadesModel::name() const { return "Audio Fades"; }

bool AudioFadesModel::process()
	{
	if( ! ins[0] ) return false;

	auto in = std::dynamic_pointer_cast<AudioData>( ins[0] )->audio;
	auto s = tryLockingInput<NumberData>( ins[1], startLengthModel->getSliderPosition() );
	auto e = tryLockingInput<NumberData>( ins[2], endLengthModel->getSliderPosition() );
	auto interp = tryLockingInput<InterpolatorData>( ins[2], interpModel->getInterpolator() );

	setFunctor( [in, s, e, interp, c = canceller]()
		{
		return std::make_shared<AudioData>( in.fade( s->f, e->f, interp->f, *c ) );
		} );

	return true;
	}

QString AudioFadesModel::portCaption( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return "Audio";
			case 1: return "Start";
			case 2: return "End";
			case 3: return "Interp";
			}
	else if( type == PortType::Out )
		switch( index )
			{
			case 0: return "Audio";
			}

	return "";
	}
unsigned int AudioFadesModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 4;
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType AudioFadesModel::dataType( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return AudioData::Type();
			case 1: return NumberData::Type();
			case 2: return NumberData::Type();
			case 3: return InterpolatorData::Type();
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

ControllerPairs AudioFadesModel::makeInputControllers()
	{
	auto s = new NumberSliderView( startLengthModel.get() );
	auto e = new NumberSliderView( endLengthModel.get() );
	auto interp = new LotonComboBoxView( interpModel.get() );
	s->setFixedHeight( 30 );
	e->setFixedHeight( 30 );
	interp->setFixedHeight( 30 );
	return {
		{ portCaption( PortType::In, 1 ), s },
		{ portCaption( PortType::In, 2 ), e },
		{ portCaption( PortType::In, 3 ), interp }
		};
	}

QJsonObject AudioFadesModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["start"]	= startLengthModel->save();
	modelJson["end"]	= endLengthModel->save();
	modelJson["interp"] = interpModel->save();
	return modelJson;
	}

void AudioFadesModel::restore( QJsonObject const & p )
	{
	startLengthModel->restore( p["start"].toObject() );
	endLengthModel->restore( p["end"].toObject() );
	interpModel->restore( p["interp"].toObject() );
	}



