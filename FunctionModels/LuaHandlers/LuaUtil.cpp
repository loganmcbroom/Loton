#include "LuaUtil.hpp"

#include <lua.hpp>

#include <QDebug>

void lua_dumpstack( lua_State * L )
	{
	qDebug() << "Lua stack dump: ";
	const int top = lua_gettop( L );
	for( int i = 1; i <= top; i++ )
		{
		const int t = lua_type( L, i );
		switch (t)
			{
			case LUA_TSTRING:	qDebug() << lua_tostring( L, i );	break;
			case LUA_TBOOLEAN:	qDebug() << ( lua_toboolean( L, i ) ? "true" : "false" ); break;
			case LUA_TNUMBER:	qDebug() << lua_tonumber( L, i ); break;
			default:			qDebug() << lua_typename( L, t ); break;
			}
		}
	qDebug() << "\n";
	}



//int lua_print( lua_State * L )
//	{
//	int nargs = lua_gettop( L );
//	for( int i = 1; i <= nargs; ++i )
//		{
//		if( lua_isstring( L, i ) )	qDebug() << lua_tostring( L, i );
//		else						qDebug() << "You tried to print a non-string in lua";
//		}
//	return 0;
//	}

//const luaL_Reg printlib [] =
//	{
//	{ "print", lua_print },
//	{ nullptr, nullptr }
//	};
