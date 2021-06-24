#include "StringDisplay.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QBoxLayout>
#include <QJsonObject>

// Model ======================================================

StringDisplayModel::StringDisplayModel( const QString & initial )
	: LotonController( "" )
	{
	}

QString StringDisplayModel::string() const
	{
	return _string;
	}

void StringDisplayModel::setString( const QString & newString )
	{
	_string = newString;
	emit stringUpdated();
	//pushStateChange( "String changed" );
	}

QVariant StringDisplayModel::state() const
	{
	return string();
	}

void StringDisplayModel::setState( const QVariant & s )
	{
	_string = s.toString();
	}


// View ======================================================

StringDisplayView::StringDisplayView( StringDisplayModel * _model )
	: QWidget()
	, model( _model )
	{
	QObject::connect( model, &StringDisplayModel::destroyed, this, [this](){ model = nullptr; } );

	setLayout( new QHBoxLayout );
	layout()->setMargin( 0 );

	QObject::connect( model, &StringDisplayModel::stateChanged, this, [this](){ repaint(); } );
	QObject::connect( model, &StringDisplayModel::stringUpdated, this, [this](){ repaint(); } );
	}

void StringDisplayView::paintEvent( QPaintEvent * )
	{
	QPainter p( this );
	p.setRenderHint( QPainter::Antialiasing );

	//Draw bounding box
	QPainterPath bound;
	bound.addRoundedRect( QRectF( 1.5, 1.5, width()-2, height()-2 ), 0, 0 );
	p.setPen( QColor::fromHsl( 0, 0, 64 ) );
	p.fillPath( bound, QColor::fromHsl( 0, 0, 32 ) );
	p.drawPath( bound );

	//Draw number
	p.setPen( QColor::fromHsl( 0, 0, 180 ) );
	QFont font( "Arial", 10 );
	font.setStyleHint( QFont::Monospace );
	p.setFont( font );
	p.drawText( 0, 0, width(), height(), Qt::AlignCenter, model->string() );
	}
