#include "OnOffButton.hpp"

#include <QLayout>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>

#include "Settings.hpp"

// Model ======================================================

OnOffButtonModel::OnOffButtonModel( bool init )
	: LotonController( init )
	, buttonPosition( init )
	{
	}

bool OnOffButtonModel::getButtonPosition() const
	{
	return buttonPosition;
	}

void OnOffButtonModel::setButtonPosition( bool b )
	{
	buttonPosition = b;
	pushStateChange( "Button Pressed" );
	}

QVariant OnOffButtonModel::state() const
	{
	return getButtonPosition();
	}

void OnOffButtonModel::setState( const QVariant & s )
	{
	buttonPosition = s.toBool();
	}


// View ======================================================


OnOffButtonView::OnOffButtonView( OnOffButtonModel * inModel )
	: QPushButton()
	, model( inModel )
	, pressing( false )
	{
	setFixedHeight( 30 );

//	setDown( model->getButtonPosition() );

//	QObject::connect( model, &OnOffButtonModel::destroyed, this, [this](){ model = nullptr; } );
//	QObject::connect( this, &QPushButton::clicked, model, [this]( bool b )
//		{
//		model->setButtonPosition( b );
//		});
//	QObject::connect( model, &OnOffButtonModel::stateChanged, this, [this]()
//		{
//		if( isDown() != model->getButtonPosition() )
//			{
//			bool oldState = blockSignals( true );
//			setDown( model->getButtonPosition() );
//			blockSignals( oldState );
//			}
//		});

	//setLayout( new QHBoxLayout );
	//layout()->setMargin( 0 );
	}

void OnOffButtonView::paintEvent( QPaintEvent * )
	{
	QPainter p( this );
	p.setRenderHint( QPainter::Antialiasing );

	auto & pal = Settings::palette();

	//Draw bounding box
	QPainterPath bound;
	bound.addRoundedRect( QRectF( 1.5, 1.5, width()-2, height()-2 ), 0, 0 );
	p.setPen( QColor::fromHsl( 0, 0, 64 ) );
	p.fillPath( bound, QColor::fromHsl( 0, 0, 32 ) );
	p.drawPath( bound );

	//Draw shading
	QPainterPath shading;
	shading.addRoundedRect( QRectF( 1.5f, 1.5f, width() - 2, height()-2 ), 0, 0 );
	const QColor shadingColor = model->getButtonPosition() ? pal.color( QPalette::Highlight ) : pal.color( QPalette::Shadow );
	p.fillPath( shading, shadingColor );

	p.setPen( model->getButtonPosition() ? QColor::fromHsl( 0, 0, 30 ) : QColor::fromHsl( 0, 0, 200 ) );
	QFont font( "Arial", 10 );
	font.setStyleHint( QFont::Monospace );
	p.setFont( font );
	p.drawText( 0, 0, width(), height(), Qt::AlignCenter, model->getButtonPosition() ? "On" : "Off" );
	}


void OnOffButtonView::mousePressEvent( QMouseEvent * me )
	{
	if( me->buttons() & Qt::LeftButton || me->buttons() & Qt::RightButton )
		{
		pressing = true;
		}
	}

void OnOffButtonView::mouseReleaseEvent( QMouseEvent * )
	{
	pressing = false;
	model->setButtonPosition( !model->getButtonPosition() );
	update();
	}
