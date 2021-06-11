#pragma once

#include "FunctionGeneratorNodeModel.hpp"

class NumberSlider;

class Func1x1SineModel : public FunctionGeneratorNodeModel
{ Q_OBJECT
public:
	Func1x1SineModel();

	QString caption() const override { return "Sine"; }
	QString name() const override { return "Func1x1 Sine"; }
	size_t numSliders() const override { return 4; }
	QString sliderCaption( size_t n ) const override { return std::vector({ "A", "ω", "φ", "B" })[n]; }
	float defaultSliderValue( size_t n ) const override { return std::vector({ 1, 1, 0, 0 })[n]; }
	QString graphTitle() const override { return "Asin(ωx - φ) + B"; }
	std::shared_ptr<Func1x1Data> sliderFunction( const std::vector<float> & sliderValues ) override;
};

