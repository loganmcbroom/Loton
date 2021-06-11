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
	using Func2x1Ptr = std::shared_ptr<Func2x1Data>;

	FunctionArithmeticNodeModel( const QString & );
	virtual ~FunctionArithmeticNodeModel() override;

private:
	virtual Func2x1Ptr operation( const std::vector<Func2x1Ptr> & ) = 0;
	virtual QString caption() const override = 0;
	virtual QString name() const override = 0;

	bool captionVisible() const override { return false; }
	QString portCaption( PortType, PortIndex ) const override { return ""; }
	bool portCaptionVisible( PortType, PortIndex ) const override { return true; }
	void setInData( std::shared_ptr<NodeData>, PortIndex  ) override;
	std::shared_ptr<NodeData> outData( QtNodes::PortIndex ) override;
	NodeDataType dataType( PortType, PortIndex ) const override;
	unsigned int nPorts( PortType ) const override;
	void inputConnectionDeleted( PortIndex ) override;

	void updateOutput();

	QWidget * makeHeaderWidget() override;
	ControllerPairs makeInputControllers() override;
	void updateDisplay();

	std::vector<std::shared_ptr<Func2x1Data>> ins;
	Func2x1Ptr out;

	const QString symbol;
	std::unique_ptr<StringDisplayModel> stringModel;
	std::unique_ptr<NumberSliderModel> xSliderModel;
	std::unique_ptr<NumberSliderModel> ySliderModel;
};

struct FunctionAdditionNodeModel : public FunctionArithmeticNodeModel
{
	FunctionAdditionNodeModel() : FunctionArithmeticNodeModel( "+" ) {}
	QString caption() const override { return "Add"; }
	QString name() const override { return "Addition"; }
	Func2x1Ptr operation( const std::vector<Func2x1Ptr> & ins ) override;
};

struct FunctionSubtractionNodeModel : public FunctionArithmeticNodeModel
{
	FunctionSubtractionNodeModel() : FunctionArithmeticNodeModel( "-" ) {}
	QString caption() const override { return "Subtract"; }
	QString name() const override { return "Subtraction"; }
	Func2x1Ptr operation( const std::vector<Func2x1Ptr> & ins ) override;
};

struct FunctionMultiplicationNodeModel : public FunctionArithmeticNodeModel
{
	FunctionMultiplicationNodeModel() : FunctionArithmeticNodeModel( "×" ) {}
	QString caption() const override { return "Multiply"; }
	QString name() const override { return "Multiplication"; }
	Func2x1Ptr operation( const std::vector<Func2x1Ptr> & ins ) override;
};

struct FunctionDivisionNodeModel : public FunctionArithmeticNodeModel
{
	FunctionDivisionNodeModel() : FunctionArithmeticNodeModel( "÷" ) {}
	QString caption() const override { return "Divide"; }
	QString name() const override { return "Division"; }
	Func2x1Ptr operation( const std::vector<Func2x1Ptr> & ins ) override;
};


