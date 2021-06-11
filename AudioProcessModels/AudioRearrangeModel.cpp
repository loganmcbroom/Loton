#include "AudioRearrangeModel.hpp"

#include <QLabel>
#include <QBoxLayout>

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/NumberData.hpp"
#include "NodeDataTypes/FunctionData.hpp"

#include "Widgets/NumberSlider.hpp"

using namespace flan;

AudioRearrangeModel::AudioRearrangeModel()
	: AudioProcessModel()
	, sliceLengthModel( new NumberSliderModel( 1, 0.001, NumberSlider::infinity ) )
	, fadeLengthModel( new NumberSliderModel( 0.05, 0, NumberSlider::infinity ) )
	{
	auto sliderSetup = [this]( NumberSliderModel * m )
		{
		QObject::connect( m, &NumberSliderModel::stateChanged, this, &AudioRearrangeModel::updateData );
		auto v = new NumberSliderView( m );
		mainLayout->addWidget( v );
		v->setMinimumSize( 64, 20 );
		};

	sliderSetup( sliceLengthModel.get() );
	sliderSetup( fadeLengthModel.get() );
	}

AudioRearrangeModel::~AudioRearrangeModel() = default;

QString AudioRearrangeModel::caption() const { return "Rearrange"; }
QString AudioRearrangeModel::name() const { return "Audio Rearrange"; }

bool AudioRearrangeModel::process()
	{
	if( ! ins[0] ) return false;

	auto in = std::dynamic_pointer_cast<AudioData>( ins[0] )->audio;
	auto sliceLength	= tryLockingInput<NumberData>( ins[1], sliceLengthModel->getSliderPosition() );
	auto fadeLength		= tryLockingInput<NumberData>( ins[2], fadeLengthModel->getSliderPosition() );

	setFunctor( [in, sliceLength, fadeLength, c = canceller]()
		{
		return std::make_shared<AudioData>( in.rearrange( sliceLength->f, fadeLength->f, *c ) );
		} );

	return true;
	}

QString AudioRearrangeModel::portCaption( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return "Audio";
			case 1: return "Slice Length";
			case 2: return "Fade Length";
			}
	else if( type == PortType::Out )
		switch( index )
			{
			case 0: return "Audio";
			}

	return "";
	}
unsigned int AudioRearrangeModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 3;
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType AudioRearrangeModel::dataType( PortType type, PortIndex index ) const
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

ControllerPairs AudioRearrangeModel::makeInputControllers()
	{
	auto s = new NumberSliderView( sliceLengthModel.get() );
	auto f = new NumberSliderView( fadeLengthModel.get() );

	return {
		{ portCaption( PortType::In, 1 ), s },
		{ portCaption( PortType::In, 2 ), f }
		};
	}

QJsonObject AudioRearrangeModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson[portCaption( PortType::In, 1 )] = sliceLengthModel->save();
	modelJson[portCaption( PortType::In, 2 )] = fadeLengthModel->save();
	return modelJson;
	}

void AudioRearrangeModel::restore( QJsonObject const & p )
	{
	sliceLengthModel->restore( p[portCaption( PortType::In, 1 )].toObject() );
	fadeLengthModel	->restore( p[portCaption( PortType::In, 2 )].toObject() );
	}



