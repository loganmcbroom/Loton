#pragma once

#include <QLabel>
#include <QPainter>
#include <QPainterPath>

class LED : public QLabel
	{ Q_OBJECT
public:
	static const int size = 6;

	enum State
		{
		off,
		thinking,
		on,
		};

	LED()
		: QLabel()
		, pix( size, size )
        {
        setColor( Qt::red );
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
