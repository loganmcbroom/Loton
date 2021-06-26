#pragma once

#include "LotonNodeData.hpp"

#include <flan/PVOC.h>

class PVOCData : public LotonNodeData
{
public:
	PVOCData() : pvoc() {}
	PVOCData( const flan::PVOC & _flan ) : pvoc( _flan ) {}

    NodeDataType type() const override { return Type() ; }
	static NodeDataType Type() { return {"flan", "PVOC"}; }

	flan::PVOC pvoc;
};
