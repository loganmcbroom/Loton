#include "FunctionNodeModel.hpp"

#include <QBoxLayout>
#include <QLabel>

#include "NodeDataTypes/NumberData.hpp"
#include "NodeDataTypes/FunctionData.hpp"

using namespace QtNodes;

FunctionNodeModel::FunctionNodeModel()
	: LotonNodeModel()
	{
	}

FunctionNodeModel::~FunctionNodeModel() = default;

void FunctionNodeModel::updateOutput()
	{
	std::shared_ptr<Func1x1Data> myFunction = function();
	Q_ASSERT_X( bool( myFunction ), "FunctionNodeModel::updateOutput",
		"Function nodes must return a function" );
	std::shared_ptr<NumberData> inLock = in.lock();
	flan::Func1x1 f = inLock? [x = myFunction->f( inLock->f )]( float ){ return x; } : myFunction->f;
	uncomposedOut = myFunction;
	out = std::make_shared<Func1x1Data>( f );
	emit dataUpdatedDynamic();
	}

void FunctionNodeModel::setInData( std::shared_ptr<QtNodes::NodeData> data, PortIndex )
	{
	emit computingStarted();
	in = std::dynamic_pointer_cast<NumberData>( data );
	updateOutput();
	emit computingFinished();
	emit dataUpdated( 0 );
	}

std::shared_ptr<NodeData> FunctionNodeModel::outData( PortIndex )
	{
	return out;
	}

NodeDataType FunctionNodeModel::dataType( PortType t, PortIndex ) const
	{
	return t == PortType::In ? NumberData::Type() : Func1x1Data::Type();
	}





//==============================================================================================================================================

Function2x1NodeModel::Function2x1NodeModel()
	: LotonNodeModel()
	{
	}

Function2x1NodeModel::~Function2x1NodeModel() = default;

void Function2x1NodeModel::updateOutput()
	{
	std::shared_ptr<Func2x1Data> myFunction = function();
	Q_ASSERT_X( bool( myFunction ), "FunctionNodeModel::updateOutput",
		"Function nodes must return a function" );
	std::shared_ptr<NumberData> inLock = in.lock();
//	flan::Func2x1 f = inLock? [inner = inLock->f, outer = myFunction->f]( float t, float f )
//		{
//		flan::vec2 g = inner( t, f );
//		return outer( g.t(), g.f() );
//		} : myFunction->f;
	flan::Func2x1 f = inLock? [x = myFunction->f( inLock->f, 0 )]( flan::vec2 ){ return x; } : myFunction->f;
	uncomposedOut = myFunction;
	out = std::make_shared<Func2x1Data>( f );
	emit dataUpdatedDynamic();
	}

void Function2x1NodeModel::setInData( std::shared_ptr<QtNodes::NodeData> data, PortIndex )
	{
	emit computingStarted();
	in = std::dynamic_pointer_cast<NumberData>( data );
	updateOutput();
	emit computingFinished();
	emit dataUpdated( 0 );
	}

std::shared_ptr<NodeData> Function2x1NodeModel::outData( PortIndex )
	{
	return out;
	}

NodeDataType Function2x1NodeModel::dataType( PortType t, PortIndex ) const
	{
	return t == PortType::In ? NumberData::Type() : Func2x1Data::Type();
	}





//==============================================================================================================================================

Function2x2NodeModel::Function2x2NodeModel()
	: LotonNodeModel()
	{
	}

Function2x2NodeModel::~Function2x2NodeModel() = default;

void Function2x2NodeModel::updateOutput()
	{
	std::shared_ptr<Func2x2Data> myFunction = function();
	Q_ASSERT_X( bool( myFunction ), "FunctionNodeModel::updateOutput",
		"Function nodes must return a function" );
	std::shared_ptr<NumberData> inLock = in.lock();
//	flan::Func2x2 f = inLock? [inner = inLock->f, outer = myFunction->f]( flan::vec2 v )
//		{
//		flan::vec2 g = inner( v.t(), v.f() );
//		return outer( g.t(), g.f() );
//		} : myFunction->f;
	flan::Func2x2 f = inLock? [x = myFunction->f( inLock->f, 0 )]( flan::vec2 ){ return x; } : myFunction->f;
	uncomposedOut = myFunction;
	out = std::make_shared<Func2x2Data>( f );
	emit dataUpdatedDynamic();
	}

void Function2x2NodeModel::setInData( std::shared_ptr<QtNodes::NodeData> data, PortIndex )
	{
	emit computingStarted();
	in = std::dynamic_pointer_cast<NumberData>( data );
	updateOutput();
	emit computingFinished();
	emit dataUpdated( 0 );
	}

std::shared_ptr<NodeData> Function2x2NodeModel::outData( PortIndex )
	{
	return out;
	}

NodeDataType Function2x2NodeModel::dataType( PortType t, PortIndex ) const
	{
	return t == PortType::In ? NumberData::Type() : Func2x1Data::Type();
	}
