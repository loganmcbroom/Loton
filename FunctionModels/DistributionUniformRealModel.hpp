#pragma once

#include "DistributionModel.hpp"

class DistributionUniformRealModel: public DistributionModel
{
public:
	DistributionUniformRealModel();

	QString caption() const override { return "Uniform Real Distribution"; }
	QString name() const override { return "Distributions - Uniform Real"; }
	size_t numSliders() const override { return 2; }
	QString sliderCaption( size_t n ) const override { return std::vector({ "a", "b" })[n]; }
	float defaultSliderValue( size_t n ) const override { return std::vector({ 0, 1 })[n]; }
	int evalFrequency() const override { return 1; }

	QString graphTitle() const override { return "P = 1/(b - a)"; }
	std::shared_ptr<Func1x1Data> sliderFunction( const std::vector<float> & sliderValues ) override;
	void generateGraphFunction( const std::vector<float> & vs ) override;
};



