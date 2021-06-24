#pragma once

#include "FunctionNodeModel.hpp"

class Func2x1Data;

/*
 * base class for nodes both taking and returning a function data type
 */

class StringDisplayModel;
class NumberSliderModel;

class FunctionArithmeticNodeModel : public LotonNodeModel
{
public:
	FunctionArithmeticNodeModel( const QString & );
	virtual ~FunctionArithmeticNodeModel() override;

	std::shared_ptr<Func2x1Data> insCast( int i ) const;

private:
	virtual std::shared_ptr<NodeData> operation() = 0;
	virtual QString caption() const override = 0;
	virtual QString name() const override = 0;

	bool captionVisible() const override { return false; }
	QString portCaption( PortType, PortIndex ) const override { return ""; }
	bool portCaptionVisible( PortType, PortIndex ) const override { return true; }
	void inputsUpdated( std::shared_ptr<NodeData>, PortIndex  ) override;
	void wipeOutputs( PortIndex ) override;
	std::shared_ptr<NodeData> outData( QtNodes::PortIndex ) override;
	NodeDataType dataType( PortType, PortIndex ) const override;
	unsigned int nPorts( PortType ) const override;
	QJsonObject save() const override;
	void restore( QJsonObject const & ) override;

	void updateOutput();
	void updateDisplay();

	QWidget * makeHeaderWidget() override;
	ControllerPairs makeInputControllers() override;

	std::shared_ptr<NodeData> out;

	const QString symbol;
	std::unique_ptr<StringDisplayModel> stringModel;
	std::vector<std::unique_ptr<NumberSliderModel>> sliderModels;
};

struct FunctionAdditionNodeModel : public FunctionArithmeticNodeModel
{
	FunctionAdditionNodeModel() : FunctionArithmeticNodeModel( "+" ) {}
	QString caption() const override { return "Add"; }
	QString name() const override { return "Arithmetic - Add"; }
	std::shared_ptr<NodeData> operation() override;
};

struct FunctionSubtractionNodeModel : public FunctionArithmeticNodeModel
{
	FunctionSubtractionNodeModel() : FunctionArithmeticNodeModel( "-" ) {}
	QString caption() const override { return "Subtract"; }
	QString name() const override { return "Arithmetic - Subtract"; }
	std::shared_ptr<NodeData> operation() override;
};

struct FunctionMultiplicationNodeModel : public FunctionArithmeticNodeModel
{
	FunctionMultiplicationNodeModel() : FunctionArithmeticNodeModel( "×" ) {}
	QString caption() const override { return "Multiply"; }
	QString name() const override { return "Arithmetic - Multiply"; }
	std::shared_ptr<NodeData> operation() override;
};

struct FunctionDivisionNodeModel : public FunctionArithmeticNodeModel
{
	FunctionDivisionNodeModel() : FunctionArithmeticNodeModel( "÷" ) {}
	QString caption() const override { return "Divide"; }
	QString name() const override { return "Arithmetic - Divide"; }
	std::shared_ptr<NodeData> operation() override;
};


