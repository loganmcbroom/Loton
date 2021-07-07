#include "AudioIterateModel.hpp"

#include <QLabel>
#include <QBoxLayout>

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/NumberData.hpp"

#include "Widgets/NumberSlider.hpp"
#include "Widgets/OnOffButton.hpp"
#include "FeedbackNodeHandlers/AudioFeedbackWorker.hpp"

using namespace flan;

AudioIterateModel::AudioIterateModel()
	: AudioFeedbackProcessModel()
	, numIterationsModel( new NumberSliderModel( 1, 0, NumberSlider::infinity ) )
	, recursionModel( new OnOffButtonModel( false ) )
	{
	QObject::connect( numIterationsModel.get(), &NumberSliderModel::stateChanged, this, &AudioIterateModel::updateData );
	QObject::connect( recursionModel.get(), &OnOffButtonModel::stateChanged, this, &AudioIterateModel::updateData );

	auto sliderView = new NumberSliderView( numIterationsModel.get() );
	mainLayout->addWidget( sliderView );
	sliderView->setMinimumSize( 64, 20 );
	setToolTipToPort( sliderView, 1 );

	numIterationsModel->setFilter( []( float x ){ return std::round( x ); } );
	}

AudioIterateModel::~AudioIterateModel() = default;

QString AudioIterateModel::caption() const { return "Iterate"; }
QString AudioIterateModel::name() const { return "Audio Iterate"; }

bool AudioIterateModel::process()
	{
	if( ! ins[0] ) return false;

	auto in = std::dynamic_pointer_cast<AudioData>( ins[0] );
	auto n = tryLockingInput<NumberData>( ins[1], numIterationsModel->getSliderPosition() );
	bool recursion = recursionModel->getButtonPosition();

	if( isFeedbackEnabled() )
		processWithFeedback( [in, n, recursion, c = canceller]( flan::Audio::Mod m )
			{
			return in->audio.iterate( n->f, m, recursion, *c );
			} );
	else
		setFunctor( [in, n, recursion, c = canceller]()
			{
			return std::shared_ptr<NodeData>( new AudioData( in->audio.iterate( n->f, nullptr, recursion, *c ) ) );
			} );

	return true;
	}

QString AudioIterateModel::portCaption( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return "Audio";
			case 1: return "N";
			case 2: return "Feedback";
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
unsigned int AudioIterateModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 3;
		case PortType::Out: return 3;
		default: return 0;
		}
	}
NodeDataType AudioIterateModel::dataType( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return AudioData::Type();
			case 1: return NumberData::Type();
			case 2: return AudioData::Type();
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

ControllerPairs AudioIterateModel::makeInputControllers()
	{
	auto slider = new NumberSliderView( numIterationsModel.get() );
	auto recButton = new OnOffButtonView( recursionModel.get() );
	slider->setFixedHeight( 30 );
	recButton->setFixedHeight( 30 );
	return {
		{ portCaption( PortType::In, 1 ), slider },
		{ "Recursion", recButton }
		};
	}

QJsonObject AudioIterateModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["slider"] = numIterationsModel->save();
	modelJson["recButton"] = recursionModel->save();
	return modelJson;
	}

void AudioIterateModel::restore( QJsonObject const & p )
	{
	numIterationsModel->restore( p["slider"].toObject() );
	recursionModel->restore( p["recButton"].toObject() );
	}


