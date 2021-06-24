#include "AudioProcessModel.hpp"

#include <QBoxLayout>

#include "Widgets/AudioPlayer.hpp"
#include "NodeDataTypes/AudioData.hpp"

AudioProcessModel::AudioProcessModel()
	: FlanProcessModel()
	, audioLayout( new QVBoxLayout )
	, playerModel( new AudioPlayerModel )
	{
	mainLayout->addLayout( audioLayout );
	audioLayout->setContentsMargins( 0, 5, 0, 0 );

	auto playerView = new AudioPlayerView( playerModel.get() );
	playerView->setFixedSize( 128, 30 );
	audioLayout->addWidget( playerView );

	QObject::connect( this, &AudioProcessModel::dataUpdated, [this]()
		{
		if( out && ! std::static_pointer_cast<LotonNodeData>( out )->wipe() )
			playerModel->setAudio( std::static_pointer_cast<AudioData>( out ) );
		});

	QObject::connect( this, &AudioProcessModel::computingStarted, playerModel.get(), &AudioPlayerModel::stop );
	}

AudioProcessModel::~AudioProcessModel()
	{
	}

QWidget * AudioProcessModel::makeHeaderWidget()
	{
	auto player = new AudioPlayerView( playerModel.get() );
	player->setFixedHeight( 130 );
	return player;
	}

