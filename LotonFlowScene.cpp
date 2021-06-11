#include "LotonFlowScene.hpp"

#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsView>
#include <nodes/Node>

#include "Widgets/ProcessExplorer.hpp"
#include "AudioProcessModels/AudioSourceDataModel.hpp"
#include "PVOCProcessModels/PVOCSourceDataModel.hpp"

using namespace QtNodes;

XFlowScene::XFlowScene( std::shared_ptr<QtNodes::DataModelRegistry> registry, QObject * parent )
	: FlowScene( registry, parent )
	{
	}

void XFlowScene::dragMoveEvent( QGraphicsSceneDragDropEvent * event )
	{
	if( event->mimeData()->hasUrls()
	 || event->mimeData()->hasFormat( "application/x-qabstractitemmodeldatalist" ) )
		event->acceptProposedAction();
	}

#include <QApplication>
#include <QClipboard>
void XFlowScene::dropEvent( QGraphicsSceneDragDropEvent * event )
	{
	// Filepaths to PVOC/Audio data
	if( event->mimeData()->hasUrls() )
		{
		auto urls = event->mimeData()->urls();
		QStringList filepaths;

		// Get local file paths from mime data
		for( auto & url : urls )
			{
			if( ! url.isLocalFile() ) continue;
			QString path = url.toLocalFile();
			filepaths.push_back( path );
			}

		for( int i = 0; i < filepaths.size(); ++i )
			{
			if( ! QFileInfo::exists( filepaths[i] ) )
				{
				qDebug() << "A file drapped into the flow scene doesn't exist.";
				continue;
				}

			std::unique_ptr<NodeDataModel> type;
			QString suffix = QFileInfo( filepaths[i] ).suffix();
			if( suffix == "flan" )
				type = registry().create( "PVOC Source" );
			else
				type = registry().create( "Audio Source" );

			if( type )
				{
				Node & node = createNode( std::move( type ) );
				undoStack->push( new NodeAddCommand( node ) );
				node.nodeGraphicsObject().setPos( event->scenePos() + i * QPointF( 0, node.nodeGeometry().height() + 25 ) );
				nodePlaced( node );
				if( suffix == "flan" )
					dynamic_cast<PVOCSourceDataModel *>( node.nodeDataModel() )->setFile( filepaths[i] );
				else
					dynamic_cast<AudioSourceDataModel *>( node.nodeDataModel() )->setFile( filepaths[i] );
				}
			else qDebug() << "Type not found in scene registry";
			}
		}
	// Name of node to create
	else if( event->mimeData()->hasFormat( "application/x-qabstractitemmodeldatalist" ) )
		{
		QByteArray encoded = event->mimeData()->data( "application/x-qabstractitemmodeldatalist" );
		QDataStream stream( &encoded, QIODevice::ReadOnly );

		int i = 0;
		while( ! stream.atEnd() )
			{
			int row, col;
			QMap<int,  QVariant> roleDataMap;
			stream >> row >> col >> roleDataMap;
			QString dataString = roleDataMap.value( Qt::UserRole ).toString();

			if( dataString != ProcessExplorer::skipText )
				{
				// Create node of type
				std::unique_ptr<NodeDataModel> type = registry().create( dataString );

				if( type )
					{
					Node & node = createNode( std::move( type ) );
					undoStack->push( new NodeAddCommand( node ) );
					node.nodeGraphicsObject().setPos( event->scenePos() +
						i * QPointF( 0, node.nodeGeometry().height() + 25 ) );
					nodePlaced( node );
					++i;
					}
				else qDebug() << "Type not found in scene registry: dataString" ;
				}
			}
		}
	else
		return;

	event->acceptProposedAction();
	}
