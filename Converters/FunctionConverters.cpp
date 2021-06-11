#include "FunctionConverters.hpp"

#include "NodeDataTypes/NumberData.hpp"
#include "NodeDataTypes/FunctionData.hpp"
#include "NodeDataTypes/InterpolatorData.hpp"

using namespace QtNodes;

void NumberToFunc1x1Converter::operator()( std::shared_ptr<NodeData> data )
	{
	auto numberData = std::dynamic_pointer_cast<NumberData>( data );

	if( numberData )
		emit finished( std::make_shared<Func1x1Data>( numberData->f ) );
	else
		emit finished( std::shared_ptr<Func1x1Data>() );
	}
void NumberToFunc2x1Converter::operator()( std::shared_ptr<NodeData> data )
	{
	auto numberData = std::dynamic_pointer_cast<NumberData>( data );

	if( numberData )
		emit finished( std::make_shared<Func2x1Data>( numberData->f ) );
	else
		emit finished( std::shared_ptr<Func2x1Data>() );
	}
void NumberToFunc2x2Converter::operator()( std::shared_ptr<NodeData> data )
	{
	auto numberData = std::dynamic_pointer_cast<NumberData>( data );

	if( numberData )
		emit finished( std::make_shared<Func2x2Data>( numberData->f ) );
	else
		emit finished( std::shared_ptr<Func2x2Data>() );
	}

void Func1x1ToNumberConverter::operator()( std::shared_ptr<NodeData> data )
	{
	auto numberData = std::dynamic_pointer_cast<Func1x1Data>( data );

	if( numberData )
		emit finished( std::make_shared<NumberData>( numberData->f( 0 ) ) );
	else
		emit finished( std::shared_ptr<NumberData>() );
	}
void Func1x1ToFunc2x1Converter::operator()( std::shared_ptr<NodeData> data )
	{
	auto funcData = std::dynamic_pointer_cast<Func1x1Data>( data );

	if( funcData )
		emit finished( std::make_shared<Func2x1Data>( funcData->f ) );
	else
		emit finished( std::shared_ptr<Func2x1Data>() );
	}
void Func1x1ToFunc2x2Converter::operator()( std::shared_ptr<NodeData> data )
	{
	auto funcData = std::dynamic_pointer_cast<Func1x1Data>( data );
	if( funcData )
		emit finished( std::make_shared<Func2x2Data>(
			[funcData]( flan::vec2 tf ){ return flan::vec2( funcData->f( tf.t() ), tf.f() ); } ) );
	else
		emit finished( std::shared_ptr<Func2x2Data>() );
	}
void Func1x1ToInterpolatorConverter::operator()( std::shared_ptr<NodeData> data )
	{
	auto funcData = std::dynamic_pointer_cast<Func1x1Data>( data );
	if( funcData )
		emit finished( std::make_shared<InterpolatorData>( funcData->f.f ) );
	else
		emit finished( std::shared_ptr<InterpolatorData>() );
	}

void Func2x1ToNumberConverter::operator()( std::shared_ptr<NodeData> data )
	{
	auto numberData = std::dynamic_pointer_cast<Func2x1Data>( data );
	if( numberData )
		emit finished( std::make_shared<NumberData>( numberData->f( 0, 0 ) ) );
	else
		emit finished( std::shared_ptr<NumberData>() );
	}
void Func2x1ToFunc1x1Converter::operator()( std::shared_ptr<NodeData> data )
	{
	auto funcData = std::dynamic_pointer_cast<Func2x1Data>( data );
	if( funcData )
		emit finished( std::make_shared<Func1x1Data>( std::bind( funcData->f, std::placeholders::_1, 0 ) ) );
	else
		emit finished( std::shared_ptr<Func1x1Data>() );
	}
void Func2x1ToFunc2x2Converter::operator()( std::shared_ptr<NodeData> data )
	{
	auto funcData = std::dynamic_pointer_cast<Func2x1Data>( data );
	if( funcData )
		emit finished( std::make_shared<Func2x2Data>(
			[funcData]( flan::vec2 tf ){ return flan::vec2{ funcData->f( tf.t(), tf.f() ), tf.f() }; }
			) );
	else
		emit finished( std::shared_ptr<Func2x2Data>() );
	}
