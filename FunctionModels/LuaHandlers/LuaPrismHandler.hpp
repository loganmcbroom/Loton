#pragma once

#include <vector>

#include <QString>

#include <flan/PVOCBuffer.h>

class lua_State;

class LuaPrismHandler
{
public:
	LuaPrismHandler( const QString & s );
	~LuaPrismHandler();
	LuaPrismHandler( const LuaPrismHandler & other );
	LuaPrismHandler( LuaPrismHandler && other );
	LuaPrismHandler & operator=( const LuaPrismHandler & other ) = delete;
	LuaPrismHandler & operator=( LuaPrismHandler && other ) = delete;

	flan::PVOCBuffer::MF operator()( int note, float time, int harmonic, float baseFreq, const std::vector<float> & mags );

protected:
	QString funcString;
	lua_State * L;

	void handleError( const QString & s );
	void initLua();
};
