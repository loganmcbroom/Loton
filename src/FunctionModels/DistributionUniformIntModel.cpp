#include "DistributionUniformIntModel.hpp"

#include <utility>

#include "NodeDataTypes/FunctionData.hpp"
#include "Widgets/NumberSlider.hpp"

DistributionUniformIntModel::DistributionUniformIntModel()
	: DistributionModel()
	{
	initialize();
	for( auto i : sliders )
		i->setFilter( []( float t ){ return std::round( t ); } );
	}

std::shared_ptr<Func1x1Data> DistributionUniformIntModel::sliderFunction( const std::vector<float> & v )
	{
	auto dist = [v]( unsigned int seed )
		{
		std::minstd_rand gen{ seed };
		if( v[0] <= v[1] )
			{
			std::uniform_int_distribution<> d{ int( v[0] ), int( v[1] ) };
			return d( gen );
			}
		else
			{
			std::uniform_int_distribution<> d{ int( v[1] ), int( v[0] ) };
			return d( gen );
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

void DistributionUniformIntModel::generateGraphFunction( const std::vector<float> & v )
	{
	graphFunction = std::make_shared<Func1x1Data>( [v]( float t )
		{
		if( v[0] < t && t < v[1] ) return 1;
		if( v[1] < t && t < v[0] ) return 1;
		return 0;
		});
	}


