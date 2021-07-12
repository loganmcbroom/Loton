#include "PVOCConvertToAudioModel.hpp"

#include <QLabel>
#include <QBoxLayout>

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/PVOCData.hpp"
#include "Widgets/StringDisplay.hpp"

#include "Converters/SoundConverters.hpp"

using namespace flan;

PVOCConvertToAudioModel::PVOCConvertToAudioModel()
	: AudioProcessModel()
	//, windowSizeModel( new StringDisplayModel( "-" ) )
	//, overlapsModel( new StringDisplayModel( "-" ) )
	, converter( new PVOCToAudioConverter )
	{
//	auto stringSetup = [this]( StringDisplayModel * m )
//		{
//		//auto v = new StringDisplayView( m );
//		//mainLayout->addWidget( v );
//		//v->setMinimumSize( 64, 20 );
//		};

//	stringSetup( windowSizeModel.get() );
//	stringSetup( overlapsModel.get() );

	mainWidget->setMinimumWidth( 64 );

	QObject::connect( converter.get(), &PVOCToAudioConverter::finished, this, [this]( std::shared_ptr<NodeData> data )
		{
		out = data;
		emit computingFinished();
		emit dataUpdated( 0 );
		} );
	}

PVOCConvertToAudioModel::~PVOCConvertToAudioModel() = default;

QString PVOCConvertToAudioModel::caption() const { return "Convert To Audio"; }
QString PVOCConvertToAudioModel::name() const { return "PVOC Convert To Audio"; }

bool PVOCConvertToAudioModel::process()
	{
	if( ! ins[0] ) return false;

	auto in = std::dynamic_pointer_cast<PVOCData>( ins[0] );

	//windowSizeModel->setString( QString::number( in->flan.getWindowSize() ) );
	//overlapsModel->setString( QString::number( in->flan.getOverlaps() / 2 ) );

	converter->convertWithCanceller( in, 0, 0, 0, canceller );
	emit computingStarted();

	return true;
	}

QString PVOCConvertToAudioModel::portCaption( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return "PVOC";
			}
	else if( type == PortType::Out )
		switch( index )
			{
			case 0: return "Audio";
			}

	return "";
	}
unsigned int PVOCConvertToAudioModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 1;
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType PVOCConvertToAudioModel::dataType( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		switch( index )
			{
			case 0: return PVOCData::Type();
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

ControllerPairs PVOCConvertToAudioModel::makeInputControllers()
	{
	//auto windowSize = new StringDisplayView( windowSizeModel.get() );
	//auto overlaps = new StringDisplayView( overlapsModel.get() );
	//windowSize->setFixedHeight( 30 );
	//overlaps->setFixedHeight( 30 );
	return {};
//		{
//		{ "Window Size", windowSize },
//		{ "Overlaps", overlaps }
//		};
	}

void PVOCConvertToAudioModel::inputConnectionDeleted( PortIndex i )
	{
	FlanProcessModel::inputConnectionDeleted( i );
	//windowSizeModel->setString( "-" );
	//overlapsModel->setString( "-" );
	}
