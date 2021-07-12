#include "LuaFunction.hpp"

#include <lua.hpp>
#include <lualib.h>

#include <QDebug>

LuaFunction::LuaFunction( int _numArgs, int _numReturns, const QString & s )
	: numArgs( _numArgs )
	, numReturns( _numReturns )
	, funcString( QString("function USER_LUA_FUNCTION( " + QString( _numArgs == 1 ? " x " : "x, y" ) + " ) ") + s + " end " )
	, L( luaL_newstate() )
	{
	luaL_openlibs(L);
	if( luaL_loadstring( L, funcString.toUtf8() ) != 0 )
		{
		funcString = QString();
		return;
		}
	lua_call( L, 0, 0 );
//	lua_getglobal( L, "_G" );
//	luaL_register( L, nullptr, printlib );
//	lua_pop( L, 1 );
	}

LuaFunction::~LuaFunction()
	{
	if( L )
		lua_close( L );
	}

LuaFunction::LuaFunction( const LuaFunction & other )
	: numArgs( other.numArgs )
	, numReturns( other.numReturns )
	, funcString( other.funcString )
	, L( luaL_newstate() )
	{
	luaL_openlibs(L);
	if( luaL_loadstring( L, funcString.toUtf8() ) != 0 )
		{
		funcString = QString();
		return;
		}
	lua_call( L, 0, 0 );
	}

LuaFunction::LuaFunction( LuaFunction && other )
	: numArgs( std::move( other.numArgs ) )
	, numReturns( std::move( other.numReturns ) )
	, funcString( std::move( other.funcString ) )
	, L( other.L )
	{
	other.L = nullptr;
	other.funcString = QString();
	}

std::vector<float> LuaFunction::callLua( const std::vector<float> & v )
	{
	Q_ASSERT( v.size() == numArgs );
	Q_ASSERT( lua_gettop( L ) == 0 );

	auto err = [this]( const QString & s )
		{
		handleError( s );
		lua_settop( L, 0 );
		funcString = QString();
		return std::vector<float>( numReturns, 0 );
		};

	if( funcString.isNull() )
		return err( "No Lua function loaded." );

	// Push function to top of stack
	lua_getglobal( L, "USER_LUA_FUNCTION" );

	// Push inputs
	for( auto f : v )
		lua_pushnumber( L, f );

	// Call function
	if( lua_pcall( L, numArgs, numReturns, 0 ) != 0 )
		return err( lua_tostring( L, -1 ) );

	// Get outputs
	if( lua_gettop( L ) != numReturns )
		return err( QString( "Lua returned the wrong number of things; Expected " )
			+ QString::number( numReturns ) + ", recieved " + QString::number( lua_gettop( L ) ) );
	std::vector<float> outs;
	for( int i = 1; i <= numReturns; ++i )
		{
		if( !lua_isnumber( L, i ) )
			return err( "Lua return was not a number." );
		outs.push_back( lua_tonumber( L, i ) );
		}

	lua_settop( L, 0 );
	return outs;
	}

void LuaFunction::handleError( const QString & s )
	{
	//qDebug() << "[Lua] " << s;
	//lua_dumpstack( L );
	}
