#pragma once

#include <QLabel>
#include <QPainter>
#include <QPainterPath>

class LED : public QLabel
	{ Q_OBJECT
public:
	static int size() { return 6; }

	enum class State
		{
		off,
		thinking,
		on,
		};

	LED();

	void setColor( QColor c );

	State state = State::off;
	QPixmap pix;

public slots:
	void setState( State s );
};
