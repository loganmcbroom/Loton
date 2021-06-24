#pragma once

#include "LotonNodeData.hpp"

#include <flan/Function.h>

class InterpolatorData : public LotonNodeData
{
public:
	InterpolatorData() : f( flan::Interpolators::linear ) {}
	InterpolatorData( const flan::Interpolator & _f ) : f( _f ) {}

	NodeDataType type() const override { return Type(); }
	static NodeDataType Type() { return {"Interpolator", "Interpolator"}; }

	flan::Interpolator f;
};
