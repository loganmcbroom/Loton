#include "AudioConvertToPVOCModel.hpp"

#include <QLabel>
#include <QBoxLayout>

#include "Settings.hpp"
#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/PVOCData.hpp"
#include "NodeDataTypes/NumberData.hpp"
#include "Widgets/NumberSlider.hpp"

#include "Converters/SoundConverters.hpp"

using namespace flan;

AudioConvertToPVOCModel::AudioConvertToPVOCModel()
	: PVOCProcessModel()
	, windowSizeModel( new NumberSliderModel( Settings::PVOCWindowSize(), 1, NumberSlider::infinity ) )
	, hopSizeModel( new NumberSliderModel( Settings::PVOCHopSize(), 1, NumberSlider::infinity ) )
	, fftSizeModel( new NumberSliderModel( Settings::PVOCFFTSize(), 0, 24 ) )
	, converter( new AudioToPVOCConverter() )
	{
	auto addSlider = [this]( NumberSliderModel * m, int port )
		{
		m->setFilter( []( float x ) { return std::round( x ); } );
		QObject::connect( m, &NumberSliderModel::stateChanged, this, &AudioConvertToPVOCModel::updateData );
		auto v = new NumberSliderView( m );
		mainLayout->addWidget( v );
		v->setMinimumSize( 64, 20 );
		setToolTipToPort( v, port );
		};

	addSlider( windowSizeModel.get(), 1 );
	addSlider( hopSizeModel.get(), 2 );
	addSlider( fftSizeModel.get(), 3 );

	QObject::connect( converter.get(), &AudioToPVOCConverter::finished, this, [this]( std::shared_ptr<NodeData> data )
		{
		out = data;
		emit computingFinished();
		emit dataUpdated( 0 );
		} );
	}

AudioConvertToPVOCModel::~AudioConvertToPVOCModel() = default;

QString AudioConvertToPVOCModel::caption() const { return "Convert To PVOC"; }
QString AudioConvertToPVOCModel::name() const { return "Audio Convert To PVOC"; }

bool AudioConvertToPVOCModel::process()
	{
	if( ! ins[0] ) return false;

	auto in = std::dynamic_pointer_cast<AudioData>( ins[0] );
	auto windowSize = windowSizeModel->getSliderPosition();
	auto hopSize = hopSizeModel->getSliderPosition();
	auto fftSize = fftSizeModel->getSliderPosition();

	converter->convertWithCanceller( in, windowSize, hopSize, fftSize, canceller );
	emit computingStarted();



	return true;
	}

QString AudioConvertToPVOCModel::portCaption( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return "Audio";
			case 1: return "Window";
			case 2: return "Hop";
			case 3: return "FFT";
			}
	else if( type == PortType::Out )
		switch( index )
			{
			case 0: return "PVOC";
			}

	return "";
	}
unsigned int AudioConvertToPVOCModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 4;
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType AudioConvertToPVOCModel::dataType( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return AudioData::Type();
			case 1: return NumberData::Type();
			case 2: return NumberData::Type();
			case 3: return NumberData::Type();
			default: return {"",""};
			}
	else if( type == PortType::Out )
		switch( index )
			{
			case 0:  return PVOCData::Type();
			default: return {"",""};
			}
	else return {"",""};
	}

ControllerPairs AudioConvertToPVOCModel::makeInputControllers()
	{
	auto windowSize	= new NumberSliderView( windowSizeModel.get() );
	auto hopSize	= new NumberSliderView( hopSizeModel.get() );
	auto fftSize	= new NumberSliderView( fftSizeModel.get() );

	windowSize->setFixedHeight( 30 );
	hopSize->setFixedHeight( 30 );
	fftSize->setFixedHeight( 30 );

	return {
		{ "Window Size", windowSize },
		{ "Hop Size", hopSize },
		{ "FFT Size = 2^N", fftSize }
		};
	}

QJsonObject AudioConvertToPVOCModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["windowSize"]	= windowSizeModel->save();
	modelJson["hopSize"]	= hopSizeModel->save();
	modelJson["fftSize"]	= fftSizeModel->save();
	return modelJson;
	}

void AudioConvertToPVOCModel::restore( QJsonObject const & p )
	{
	windowSizeModel->restore( p["windowSize"].toObject() );
	hopSizeModel->restore( p["hopSize"].toObject() );
	fftSizeModel->restore( p["fftSize"].toObject() );
	}



