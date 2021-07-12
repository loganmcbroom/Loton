#include "DistributionExponentialModel.hpp"

#include "NodeDataTypes/FunctionData.hpp"
#include "Widgets/NumberSlider.hpp"

DistributionExponentialModel::DistributionExponentialModel()
	: DistributionModel()
	{
	initialize();
	}

std::shared_ptr<Func1x1Data> DistributionExponentialModel::sliderFunction( const std::vector<float> & v )
	{
	auto dist = [v]( unsigned int seed )
		{
		std::minstd_rand gen{ seed };
		if( v[1] == 0 ) return v[0];
		if( v[1] > 0 )
			{
			std::exponential_distribution<> d{ v[1] };
			return float( d( gen ) ) + v[0];
			}
		else
			{
			std::exponential_distribution<> d{ -v[1] };
			return float( -d( gen ) ) + v[0];
			}
		};

	if( in.lock() ) return std::make_shared<Func1x1Data>( [dist]( float t )
		{
		return dist( std::hash<float>()( t ) );
		});
	else return std::make_shared<Func1x1Data>( [dist]( float )
		{
		return dist( std::random_device()() );
		});
	}

void DistributionExponentialModel::generateGraphFunction( const std::vector<float> & v )
	{
	graphFunction = std::make_shared<Func1x1Data>( [v]( float t )
		{
		if( v[1] == 0 ) return 0.0f;
		if( v[1] > 0 )
			{
			if( t < v[0] ) return 0.0f;
			return v[1] * std::exp( -v[1] * ( t - v[0] ) );
			}
		else
			{
			if( t > v[0] ) return 0.0f;
			return -v[1] * std::exp( -v[1] * ( t - v[0] ) );
			}

		});
	}
