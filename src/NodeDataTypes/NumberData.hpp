#pragma once

#include "LotonNodeData.hpp"

class NumberData : public LotonNodeData
{
public:
	NumberData() : f( 0 ) {}
	NumberData( float _f ) : f( _f ) {}

	NodeDataType type() const override { return Type(); }
	static NodeDataType Type() { return {"Number", "Number"}; }

	float f;
};
