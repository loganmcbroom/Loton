#include "PVOCProcessModel.hpp"

#include <QBoxLayout>

#include "Widgets/PVOCPlayer.hpp"
#include "NodeDataTypes/PVOCData.hpp"

PVOCProcessModel::PVOCProcessModel()
	: FlanProcessModel()
	, flanLayout( new QVBoxLayout )
	, playerModel( new PVOCPlayerModel )
	{
	mainLayout->addLayout( flanLayout );
	flanLayout->setContentsMargins( 0, 5, 0, 0 );

	auto playerView = new PVOCPlayerView( playerModel.get() );
	playerView->setFixedSize( 128, 30 );
	flanLayout->addWidget( playerView );

	QObject::connect( this, &PVOCProcessModel::dataUpdated, [this]()
		{
		playerModel->setPVOC( std::dynamic_pointer_cast<PVOCData>( outData( 0 ) ) );
		});
	}

PVOCProcessModel::~PVOCProcessModel()
	{
	}

QWidget * PVOCProcessModel::makeHeaderWidget()
	{
	auto player = new PVOCPlayerView( playerModel.get() );
	player->setFixedHeight( 170 );
	return player;
	}




