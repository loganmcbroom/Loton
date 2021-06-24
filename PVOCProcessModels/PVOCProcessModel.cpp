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
		if( out && ! std::static_pointer_cast<LotonNodeData>( out )->wipe() )
			playerModel->setPVOC( std::static_pointer_cast<PVOCData>( out ) );
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




