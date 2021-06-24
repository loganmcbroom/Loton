#pragma once

#include <QUuid>

#include <nodes/NodeDataModel>

using QtNodes::NodeDataType;

class LotonNodeData : public QtNodes::NodeData
{
public:
	LotonNodeData();
	virtual bool wipe() const { return false; }
	//const QUuid tag;
};

