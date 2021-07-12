#include "Interpolator.hpp"

#include <QDebug>
#include <QJsonObject>


InterpolatorModel::InterpolatorModel( int initial )
	: LotonComboBoxModel( initial, {"Linear", "Nearest", "Smoothstep", "Smootherstep", "Sine", "Square Root" } )
	{
	}

InterpolatorType InterpolatorModel::getInterpolatorType() const
	{
	return static_cast<InterpolatorType>( selection() );
	}

void InterpolatorModel::setInterpolatorType( InterpolatorType type )
	{
	setSelection( static_cast<int>( type ) );
	}

flan::Interpolator InterpolatorModel::getInterpolator()
	{
	switch( getInterpolatorType() )
		{
		case linear:		return flan::Interpolators::linear;
		case nearest:		return flan::Interpolators::nearest;
		case smoothstep:	return flan::Interpolators::smoothstep;
		case smootherstep:	return flan::Interpolators::smootherstep;
		case sine:			return flan::Interpolators::sine;
		case squareroot:	return flan::Interpolators::sqrt;
		default:
			qDebug() << "Unknown interpolator selected, defaulting to linear";
			return flan::Interpolators::linear;
		}
	}

