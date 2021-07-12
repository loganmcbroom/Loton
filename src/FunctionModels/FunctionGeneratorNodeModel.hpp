#pragma once

#include "FunctionNodeModel.hpp"

using QtNodes::NodeData;
using QtNodes::PortIndex;

class NumberSlider;
class FunctionGeneratorPlotModel;

/*
 * Base class for nodes that create functions from slider inputs
 */

class FunctionGeneratorNodeModel : public Function1x1NodeModel
{ Q_OBJECT
public:
	FunctionGeneratorNodeModel();
	virtual ~FunctionGeneratorNodeModel() override;
	void initialize();

protected:
	std::vector<NumberSlider *> sliders;
	std::shared_ptr<Func1x1Data> graphFunction;

private:
	//Interface
	virtual QString caption() const override = 0;
	virtual QString name() const override = 0;
	virtual std::shared_ptr<Func1x1Data> sliderFunction( const std::vector<float> & ) = 0;
	virtual size_t numSliders() const = 0;
	virtual QString sliderCaption( size_t n ) const = 0;
	virtual float defaultSliderValue( size_t n ) const = 0;
	virtual float sliderLowerBound( size_t n ) const;
	virtual float sliderUpperBound( size_t n ) const;
	virtual QString graphTitle() const  = 0;
	virtual void generateGraphFunction( const std::vector<float> & );
	virtual int evalFrequency() const;

	QWidget * makeHeaderWidget() override;
	ControllerPairs makeInputControllers() override;
	QJsonObject save() const override final;
	void restore( QJsonObject const & p ) override final;
	std::shared_ptr<Func1x1Data> function() override final;

	FunctionGeneratorPlotModel * plotModel;
};

