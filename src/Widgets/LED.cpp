#include "LED.hpp"

LED::LED()
	: QLabel()
	, pix( size(), size() )
    {
    setColor( Qt::red );
    }

void LED::setColor( QColor c )
	{
	pix.fill( c );
	setPixmap( pix );
	repaint();
	}

void LED::setState( State s )
	{
	state = s;
	switch( state )
		{
		case State::off:		setColor( Qt::red	 ); break;
		case State::thinking:	setColor( Qt::yellow ); break;
		case State::on:			setColor( Qt::green  ); break;
		}
    }
