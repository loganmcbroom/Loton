#include "AudioInfoProcessModel.hpp"

#include <QBoxLayout>

#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/NumberData.hpp"
#include "Widgets/StringDisplay.hpp"

AudioInfoProcessModel::AudioInfoProcessModel()
	: FlanProcessModel()
	, infoLayout( new QVBoxLayout )
	, infoModel( new StringDisplayModel( "" ) )
	{
	mainLayout->addLayout( infoLayout );
	infoLayout->setContentsMargins( 0, 5, 0, 0 );

	auto StringView = new StringDisplayView( infoModel.get() );
	StringView->setFixedSize( 60, 30 );
	infoLayout->addWidget( StringView );

	QObject::connect( this, &AudioInfoProcessModel::dataUpdated, [this]()
		{
		if( outData( 0 ) )
			{
			infoModel->setString( QString::number(
				(double) std::dynamic_pointer_cast<NumberData>( outData( 0 ) )->f ) );
			}
		} );
	}

AudioInfoProcessModel::~AudioInfoProcessModel()
	{
	}

QWidget * AudioInfoProcessModel::makeHeaderWidget()
	{
	auto player = new StringDisplayView( infoModel.get() );
	player->setFixedHeight( 50 );
	return player;
	}

