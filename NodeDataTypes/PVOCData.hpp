#pragma once

#include "TaggedNodeData.hpp"

#include <flan/PVOC.h>

using QtNodes::NodeDataType;

class PVOCData : public TaggedNodeData
{
public:
	PVOCData() : flan() {}
	PVOCData( const flan::PVOC & _flan ) : flan( _flan ) {}

    NodeDataType type() const override { return Type() ; }
	static NodeDataType Type() { return {"flan", "PVOC"}; }

	flan::PVOC flan;
};
