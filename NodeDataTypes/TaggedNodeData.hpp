#pragma once

#include <QUuid>

#include <nodes/NodeDataModel>

class TaggedNodeData : public QtNodes::NodeData
{
public:
	TaggedNodeData();
	const QUuid tag;
};

