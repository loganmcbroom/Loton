#pragma once

#include <nodes/NodeDataModel>

#include <flan/Function.h>

using QtNodes::NodeDataType;

class InterpolatorData : public QtNodes::NodeData
{
public:
	InterpolatorData() : f( flan::Interpolators::linear ) {}
	InterpolatorData( const flan::Interpolator & _f ) : f( _f ) {}

	NodeDataType type() const override { return Type(); }
	static NodeDataType Type() { return {"Interpolator", "Interpolator"}; }

	flan::Interpolator f;
};
