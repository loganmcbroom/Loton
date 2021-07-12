#pragma once

#include <vector>

#include <QString>

#include <flan/Utility/vec2.h>

class lua_State;

class LuaFunction
{
public:
	LuaFunction( int _numArgs, int _numReturns, const QString & s );
	~LuaFunction();
	LuaFunction( const LuaFunction & other );
	LuaFunction( LuaFunction && other );
	LuaFunction & operator=( const LuaFunction & other ) = delete;
	LuaFunction & operator=( LuaFunction && other ) = delete;

	std::vector<float> callLua( const std::vector<float> & v );

protected:
	int numArgs;
	int numReturns;
	QString funcString;
	lua_State * L;

	void handleError( const QString & s );
};

struct LuaFunc1x1 : public LuaFunction
{
	LuaFunc1x1( const QString & s ) : LuaFunction( 1, 1, s ) {}
	float operator()( float x ) { return callLua( { x } )[0]; }
};

struct LuaFunc2x1 : public LuaFunction
{
	LuaFunc2x1( const QString & s ) : LuaFunction( 2, 1, s ) {}
	float operator()( flan::vec2 v )
		{
		return callLua( { v.x(), v.y() } )[0];
		}
};

struct LuaFunc2x2 : public LuaFunction
{
	LuaFunc2x2( const QString & s ) : LuaFunction( 2, 2, s ) {}
	flan::vec2 operator()( flan::vec2 v )
		{
		auto w = callLua( { v.x(), v.y() } );
		return { w[0], w[1] };
		}
};
