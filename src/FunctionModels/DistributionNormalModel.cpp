#include "DistributionNormalModel.hpp"

#include <random>

#include "NodeDataTypes/FunctionData.hpp"
#include "NodeDataTypes/NumberData.hpp"
#include "Widgets/NumberSlider.hpp"

DistributionNormalModel::DistributionNormalModel()
	: DistributionModel()
	{
	initialize();
	}

struct Identity_Random_Engine
	{
	//using result_type = size_t;
	size_t min()		{ return t; }
	size_t max()		{ return t; }
	size_t operator()() { return t; }
	size_t t;
	};

std::shared_ptr<Func1x1Data> DistributionNormalModel::sliderFunction( const std::vector<float> & v )
	{
	auto dist = [v]( unsigned int seed )
		{
		if( v[1] == 0 ) return v[0];
		std::minstd_rand gen{ seed };
		std::normal_distribution<> d{ v[0], v[1] };
		return float( d( gen ) );
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

void DistributionNormalModel::generateGraphFunction( const std::vector<float> & v )
	{
	graphFunction = std::make_shared<Func1x1Data>( [v]( float t )
		{
		if( v[1] > 0.01 )
			{
			static const float inv_sqrt_2pi = 0.3989422804014327f;
			const float a = ( t - v[0] ) / v[1];
			return inv_sqrt_2pi / v[1] * std::exp(-0.5f * a * a);
			}
		else
			{
			return std::abs( t - v[0] ) < 0.05 ? 1.0f : 0.0f;
			}
		});
	}

float DistributionNormalModel::sliderLowerBound( size_t n ) const
	{
	return std::vector({ -NumberSlider::infinity, 0 })[n];
	}
