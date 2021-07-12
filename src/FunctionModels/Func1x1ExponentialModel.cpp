#include "Func1x1ExponentialModel.hpp"

#include "NodeDataTypes/FunctionData.hpp"

Func1x1ExponentialModel::Func1x1ExponentialModel()
	: FunctionGeneratorNodeModel()
	{
	initialize();
	}

std::shared_ptr<Func1x1Data> Func1x1ExponentialModel::sliderFunction( const std::vector<float> & v )
	{
	return std::make_shared<Func1x1Data>( [v]( float t )
		{
		return v[0] * exp( v[1] * t - v[2] ) + v[3];
		});
	}





