#include "Func1x1PolynomialModel.hpp"

#include "NodeDataTypes/FunctionData.hpp"

Func1x1PolynomialModel::Func1x1PolynomialModel()
	: FunctionGeneratorNodeModel()
	{
	initialize();
	}

std::shared_ptr<Func1x1Data>
Func1x1PolynomialModel::sliderFunction( const std::vector<float> & v )
	{
	return std::make_shared<Func1x1Data>( [v]( float t )
		{
		float accumulator = 0;
		for( int i = 0; i < v.size(); ++i )
			accumulator += float( v[i] ) * pow( t, i );
		return accumulator;
		});
	}

