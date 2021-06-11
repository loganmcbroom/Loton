#pragma once

#include <QPushButton>
#include <QPainter>
#include <QPainterPath>

class BypassButton : public QPushButton
{ Q_OBJECT
public:
	static const int size = 6;

	BypassButton()
		: QPushButton()
		{
		setFixedSize( size, size );
		}

	void setColor( QColor c )
		{
		pix.fill( c );
		setPixmap( pix );
		repaint();
		}

	State state = off;
	QPixmap pix;

public slots:
	void setState( State s )
		{
		state = s;
		switch( state )
			{
			case off:		setColor( Qt::red	 ); break;
			case thinking:	setColor( Qt::yellow ); break;
			case on:		setColor( Qt::green  ); break;
			}
		}
};
