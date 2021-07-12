#pragma once

#include <nodes/FlowScene>

class LotonFlowScene : public QtNodes::FlowScene
{
public:
	LotonFlowScene( std::shared_ptr<QtNodes::DataModelRegistry> registry, QObject * parent = nullptr );
	void dropEvent( QGraphicsSceneDragDropEvent * event ) override;
	void dragMoveEvent( QGraphicsSceneDragDropEvent * event ) override;

	//void createNode( const QString & nodeName );
};

