#pragma once

#include <nodes/NodeDataModel>

#include <flan/Function.h>

using QtNodes::NodeDataType;

class FunctionData : public QtNodes::NodeData
{
public:
	virtual int numIns() const = 0;
	virtual int numOuts() const = 0;
};

class Func1x1Data : public FunctionData
{
public:
	Func1x1Data() : f( 0 ) {}
	Func1x1Data( const flan::Func1x1 & _f ) : f( _f ) {}

    NodeDataType type() const override { return Type(); }
	static NodeDataType Type() { return { "Func1x1", "Func1x1" }; }
	virtual int numIns() const override { return 1; }
	virtual int numOuts() const override { return 1; }

	flan::Func1x1 f;
};

class Func2x1Data : public FunctionData
{
public:
	Func2x1Data() : f( 0 ) {}
	Func2x1Data( const flan::Func2x1 & _f ) : f( _f ) {}

	NodeDataType type() const override { return Type(); }
	static NodeDataType Type() { return { "Func2x1", "Func2x1" }; }
	virtual int numIns() const override { return 2; }
	virtual int numOuts() const override { return 1; }

	flan::Func2x1 f;
};

class Func2x2Data : public FunctionData
{
public:
	Func2x2Data() : f( 0 ) {}
	Func2x2Data( const flan::Func2x2 & _f ) : f( _f ) {}

	NodeDataType type() const override { return Type(); }
	static NodeDataType Type() { return { "Func2x2", "Func2x2" }; }
	virtual int numIns() const override { return 2; }
	virtual int numOuts() const override { return 2; }

	flan::Func2x2 f;
};

