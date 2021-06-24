#pragma once

#include "LotonNodeData.hpp"

#include <flan/PVOC.h>

class PVOCData : public LotonNodeData
{
public:
	PVOCData() : flan() {}
	PVOCData( const flan::PVOC & _flan ) : flan( _flan ) {}

    NodeDataType type() const override { return Type() ; }
	static NodeDataType Type() { return {"flan", "PVOC"}; }

	flan::PVOC flan;
};
