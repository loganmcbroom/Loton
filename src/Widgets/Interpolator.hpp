#pragma once

//Combo box containing the flan interpolators

#include <flan/Utility/Interpolator.h>

#include "Widgets/ComboBox.hpp"

enum InterpolatorType : int
{
	linear,
	nearest,
	smoothstep,
	smootherstep,
	sine,
	squareroot,
};

class InterpolatorModel : public LotonComboBoxModel
{
public:
	InterpolatorModel( int initial = 0 );

	InterpolatorType getInterpolatorType() const;
	void setInterpolatorType( InterpolatorType );

	flan::Interpolator getInterpolator();
};
