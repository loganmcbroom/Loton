#pragma once

#include "FunctionGeneratorNodeModel.hpp"

class Func1x1ExponentialModel : public FunctionGeneratorNodeModel
{ Q_OBJECT
public:
	Func1x1ExponentialModel();

	QString caption() const override { return "Exponential"; }
	QString name() const override { return "Func1x1 Exponential"; }
	size_t numSliders() const override { return 4; }
	QString sliderCaption( size_t n ) const override { return std::vector({ "A", "k", "s", "B" })[n]; }
	float defaultSliderValue( size_t n ) const override { return std::vector({ 1, 1, 0, 0 })[n]; }
	QString graphTitle() const override { return "Ae<sup>kx-s</sup> + B"; }
	std::shared_ptr<Func1x1Data> sliderFunction( const std::vector<float> & sliderValues ) override;
};

