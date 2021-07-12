#include "LuaPrismHandler.hpp"

#include <array>

#include <lua.hpp>
#include <lualib.h>

#include <QDebug>

#include "LuaUtil.hpp"


// Mag array functions =======================================================================================================================

static const char* arrayName = "MAG_ARRAY";

// metatable method for handling "array[index]"
static int arrayIndex( lua_State* L )
	{
	const std::vector<float>** pData = (const std::vector<float>**) luaL_checkudata( L, 1, arrayName ); // Get pointer to c array
	const std::vector<float>& data = **pData;
	const int index = luaL_checkint( L, 2 );
	if( 0 < index && index <= data.size() )
		lua_pushnumber( L, data[index-1] );
	else
		lua_pushnumber( L, 0 );
	return 1;
	}

// Create a metatable for our array type and store in lua registry
static void createArrayMetatable( lua_State* L )
	{
	static const struct luaL_Reg array[] =
		{
		{ "__index",  arrayIndex },
		{ nullptr, nullptr }
		};
	luaL_newmetatable( L, arrayName );
	luaL_openlib( L, nullptr, array, 0 );
	lua_settop( L, 0 );
	}

// LuaPrismHandler =======================================================================================================================

void LuaPrismHandler::initLua()
	{
	L = luaL_newstate();
	luaL_openlibs( L );
	createArrayMetatable( L );
	if( luaL_loadstring( L, funcString.toUtf8() ) != 0 )
		{
		funcString = QString();
		return;
		}
	lua_pcall( L, 0, 0, 0 );
	//	lua_getglobal( L, "_G" );
//	luaL_register( L, nullptr, printlib );
//	lua_pop( L, 1 );
	}

static const QString userFunctionString = R"(
	function USER_LUA_FUNCTION( n, t, f, h, hn, hs )

	)"; // "end" will be added later

LuaPrismHandler::LuaPrismHandler( const QString & s )
	: funcString( userFunctionString + s + " end " )
	, L( nullptr )
	{
	initLua();
	}

LuaPrismHandler::~LuaPrismHandler()
	{
	if( L )
		lua_close( L );
	}

LuaPrismHandler::LuaPrismHandler( const LuaPrismHandler & other )
	: funcString( other.funcString )
	, L( nullptr )
	{
	initLua();
	}

LuaPrismHandler::LuaPrismHandler( LuaPrismHandler && other )
	: funcString( std::move( other.funcString ) )
	, L( other.L )
	{
	other.L = nullptr;
	other.funcString = QString();
	}

flan::PVOCBuffer::MF LuaPrismHandler::operator()( int note, float time, int harmonic, float baseFreq, const std::vector<float> & mags )
	{
	Q_ASSERT( lua_gettop( L ) == 0 );

	auto err = [this]( const QString & s )
		{
		handleError( s );
		lua_settop( L, 0 );
		funcString = QString();
		return flan::PVOCBuffer::MF{ 0.0f, 0.0f };
		};

	if( funcString.isNull() )
		return err( "No Lua function loaded." );

	// Push function to top of stack
	lua_getglobal( L, "USER_LUA_FUNCTION" );

	// Push inputs
	lua_pushnumber( L, note + 1 );
	lua_pushnumber( L, time );
	lua_pushnumber( L, baseFreq );
	lua_pushnumber( L, harmonic+1 );
	lua_pushnumber( L, mags.size() );
	const std::vector<float>** const pArray = (const std::vector<float>**) lua_newuserdata( L, sizeof( const std::vector<float>** ) );
	*pArray = &mags;
	luaL_getmetatable( L, arrayName );
	lua_setmetatable( L, -2 );

	// Call function
	if( lua_pcall( L, 6, 2, 0 ) != 0 )
		return err( lua_tostring( L, -1 ) );

	// Get outputs
	if( lua_gettop( L ) != 2 ) return err( QString( "Lua returned the wrong number of things; Expected 2, recieved " ) + QString::number( lua_gettop( L ) ) );
	if( !lua_isnumber( L, 1 ) ) return err( "Lua return was not a number." );
	if( !lua_isnumber( L, 2 ) ) return err( "Lua return was not a number." );

	flan::PVOCBuffer::MF mf;
	mf.m = lua_tonumber( L, 1 );
	mf.f = lua_tonumber( L, 2 );

	lua_settop( L, 0 ); // Shouldn't be needed, but who knows how lua works
	return mf;
	}

void LuaPrismHandler::handleError( const QString & s )
	{
	//qDebug() << "[Lua] " << s;
	//lua_dumpstack( L );
	}
