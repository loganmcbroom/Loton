#include "AudioModifyVolumeModel.hpp"

#include <QLabel>
#include <QBoxLayout>

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/FunctionData.hpp"

#include "Widgets/NumberSlider.hpp"

using namespace flan;

AudioModifyVolumeModel::AudioModifyVolumeModel()
	: AudioProcessModel()
	, sliderModel( new NumberSliderModel( 0.0 ) )
	{
	QObject::connect( sliderModel.get(), &NumberSliderModel::stateChanged,
					this, &AudioModifyVolumeModel::updateData );

	auto sliderView = new NumberSliderView( sliderModel.get() );
	mainLayout->addWidget( sliderView );
	sliderView->setMinimumSize( 64, 20 );
	setToolTipToPort( sliderView, 1 );
	}

bool AudioModifyVolumeModel::process()
	{
	if( ! ins[0] ) return false;

	auto in = std::dynamic_pointer_cast<AudioData>( ins[0] )->audio;
	auto gain = ins[1]?
		std::dynamic_pointer_cast<Func1x1Data>( ins[1] ) :
		std::make_shared<Func1x1Data>( sliderModel->getSliderPosition() );

	setFunctor( [in, gain, c = canceller]()
		{
		return std::shared_ptr<NodeData>( new AudioData( in.modifyVolume( gain->f, *c ) ) );
		});

	return true;
	}

QString AudioModifyVolumeModel::caption() const { return "Modify Volume"; }
QString AudioModifyVolumeModel::name() const { return "Audio Modify Volume"; }
QString AudioModifyVolumeModel::portCaption( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return "Audio";
			case 1: return "Factor";
			}
	else if( type == PortType::Out )
		return "Audio";

	return "";
	}
unsigned int AudioModifyVolumeModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 2;
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType AudioModifyVolumeModel::dataType( PortType type, PortIndex index ) const
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

ControllerPairs AudioModifyVolumeModel::makeInputControllers()
	{
	auto slider = new NumberSliderView( sliderModel.get() );
	slider->setFixedHeight( 30 );
	return {{ portCaption( PortType::In, 1 ), slider }};
	}

QJsonObject AudioModifyVolumeModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["slider"] = sliderModel->save();
	return modelJson;
	}

void AudioModifyVolumeModel::restore( QJsonObject const & p )
	{
	sliderModel->restore( p["slider"].toObject() );
	}

