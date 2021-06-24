#include "FunctionNodeModel.hpp"

#include <QBoxLayout>
#include <QLabel>

#include "NodeDataTypes/NumberData.hpp"
#include "NodeDataTypes/FunctionData.hpp"

using namespace QtNodes;

//==============================================================================================================================================

FunctionNodeModel::FunctionNodeModel() = default;
FunctionNodeModel::~FunctionNodeModel() = default;

std::shared_ptr<NodeData> FunctionNodeModel::outData( PortIndex )
	{
	return out;
	}

void FunctionNodeModel::inputsUpdated( std::shared_ptr<QtNodes::NodeData> data, PortIndex )
	{
	// Don't process wipe commands
	if( hasWipedInput() ) return;

	emit computingStarted();
	updateOutput();
	emit computingFinished();
	emit dataUpdated( 0 );
	}

void FunctionNodeModel::wipeOutputs( PortIndex )
	{
	out = makeWipe();
	emit dataUpdated( 0 );
	}



//==============================================================================================================================================

Function1x1NodeModel::Function1x1NodeModel() = default;
Function1x1NodeModel::~Function1x1NodeModel() = default;

void Function1x1NodeModel::updateOutput()
	{
	std::shared_ptr<Func1x1Data> myFunction = function();
	Q_ASSERT_X( bool( myFunction ), "Function1x1NodeModel::updateOutput",
		"Function nodes must return a function" );

	std::shared_ptr<NumberData> in = std::static_pointer_cast<NumberData>( ins[0] );
	flan::Func1x1 f = in? [x = myFunction->f( in->f )]( float ){ return x; } : myFunction->f;

	uncomposedOut = myFunction;
	out = std::make_shared<Func1x1Data>( f );
	emit dataUpdatedDynamic();
	}

NodeDataType Function1x1NodeModel::dataType( PortType t, PortIndex ) const
	{
	return t == PortType::In ? NumberData::Type() : Func1x1Data::Type();
	}





//==============================================================================================================================================

Function2x1NodeModel::Function2x1NodeModel() = default;
Function2x1NodeModel::~Function2x1NodeModel() = default;

void Function2x1NodeModel::updateOutput()
	{
	std::shared_ptr<Func2x1Data> myFunction = function();
	Q_ASSERT_X( bool( myFunction ), "Function1x1NodeModel::updateOutput",
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

NodeDataType Function2x1NodeModel::dataType( PortType t, PortIndex ) const
	{
	return t == PortType::In ? NumberData::Type() : Func2x1Data::Type();
	}





//==============================================================================================================================================

Function2x2NodeModel::Function2x2NodeModel() = default;
Function2x2NodeModel::~Function2x2NodeModel() = default;

void Function2x2NodeModel::updateOutput()
	{
	std::shared_ptr<Func2x2Data> myFunction = function();
	Q_ASSERT_X( bool( myFunction ), "Function1x1NodeModel::updateOutput",
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

NodeDataType Function2x2NodeModel::dataType( PortType t, PortIndex ) const
	{
	return t == PortType::In ? NumberData::Type() : Func2x2Data::Type();
	}
