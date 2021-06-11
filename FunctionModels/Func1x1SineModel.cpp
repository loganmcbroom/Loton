#include "Func1x1SineModel.hpp"

#include "NodeDataTypes/FunctionData.hpp"

Func1x1SineModel::Func1x1SineModel()
	: FunctionGeneratorNodeModel()
	{
	initialize();
	}

std::shared_ptr<Func1x1Data>
Func1x1SineModel::sliderFunction( const std::vector<float> & v )
	{
	return std::make_shared<Func1x1Data>( [v]( float t )
		{
		return v[0] * sin( v[1] * t - v[2] ) + v[3];
		});
	}
