#pragma once

#include <nodes/FlowScene>

class XFlowScene : public QtNodes::FlowScene
{
public:
	XFlowScene( std::shared_ptr<QtNodes::DataModelRegistry> registry, QObject * parent = nullptr );
	void dropEvent( QGraphicsSceneDragDropEvent * event ) override;
	void dragMoveEvent( QGraphicsSceneDragDropEvent * event ) override;
};

