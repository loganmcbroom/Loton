#pragma once

#include "LotonNodeData.hpp"

class WipeNodeData : public LotonNodeData
{
public:
	bool wipe() const override { return true; }
	NodeDataType type() const override { return Type(); }
	static NodeDataType Type() { return {"Wipe", "Wipe"}; }
};
