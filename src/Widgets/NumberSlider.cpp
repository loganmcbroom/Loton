#include "NumberSlider.hpp"

#include <algorithm>

#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QDebug>
#include <QApplication>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QBoxLayout>
#include <QLabel>

#include <MainWindow.hpp>

struct DoubleOrNothingValidator : public QDoubleValidator
{
virtual QValidator::State validate( QString & input, int &pos ) const override
	{
	if( input == "" )
		return QValidator::Acceptable;
	else
		return QDoubleValidator::validate( input, pos );
	}
};

// Model ======================================================

NumberSliderModel::NumberSliderModel( float _defaultValue, float _lowerBound, float _upperBound )
	: LotonController( _defaultValue )
	, defaultValue( _defaultValue )
	, lowerBound( _lowerBound )
	, upperBound( _upperBound )
	, sliderPosition( _defaultValue )
	, filter( nullptr )
	{
	}

float NumberSliderModel::getSliderPosition() const
	{
	return filter?
		std::clamp( filter( sliderPosition ), lowerBound, upperBound ) :
		sliderPosition;
	}

float NumberSliderModel::getSliderPositionUnfiltered() const
	{
	return sliderPosition;
	}

void NumberSliderModel::setSliderPositionStatic( float x )
	{
	sliderPosition = std::clamp( x, lowerBound, upperBound );
	pushStateChange( "Slider moved" );
	}

void NumberSliderModel::setSliderPositionDynamic( float x )
	{
	sliderPosition = std::clamp( x, lowerBound, upperBound );
	emit stateChangedDynamic();
	}

void NumberSliderModel::setFilter( std::function< float ( float ) > newFilter )
	{
	filter = newFilter;
	}

QVariant NumberSliderModel::state() const
	{
	return getSliderPositionUnfiltered();
	}

void NumberSliderModel::setState( const QVariant & s )
	{
	sliderPosition = std::clamp( s.toFloat(), lowerBound, upperBound );
	}


// View ======================================================

NumberSliderView::NumberSliderView( NumberSliderModel * _model )
	: QWidget()
	, model( _model )
	, grabbedPos()
	, editor( nullptr )
	, sliderGrabbed( false )
	{
	setFixedHeight( 30 );

	QObject::connect( model, &NumberSliderModel::destroyed, this, [this](){ model = nullptr; } );

	setLayout( new QHBoxLayout );
	layout()->setMargin( 0 );

	//Editor setup
	editor = new QLineEdit;
	layout()->addWidget( editor );
	editor->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
	editor->setValidator( new DoubleOrNothingValidator() );
	QFont font( "Arial", 10 );
	font.setStyleHint( QFont::Monospace );
	editor->setFont( font );
	QObject::connect( model, &NumberSliderModel::stateChangedDynamic, editor, [this]()
		{
		editor->setText( QString::number( std::round( model->getSliderPosition() * 1000.0f ) / 1000.0 ) );
		repaint();
		});
	QObject::connect( editor, &QLineEdit::editingFinished, [this]()
		{
		if( editor->text() != "" )
			model->setSliderPositionStatic( editor->text().toFloat() );
		editor->setVisible( false );
		});
	editor->setAlignment( Qt::AlignCenter );
	editor->setStyleSheet( "background-color: rgba(0,0,0,0);" );
	editor->setVisible( false );
	}

void NumberSliderView::paintEvent( QPaintEvent * )
	{
	QPainter p( this );
	p.setRenderHint( QPainter::Antialiasing );

	//Draw bounding box
	QPainterPath bound;
	bound.addRoundedRect( QRectF( 1.5, 1.5, width()-2, height()-2 ), 0, 0 );
	p.setPen( QColor::fromHsl( 0, 0, 64 ) );
	p.fillPath( bound, QColor::fromHsl( 0, 0, 32 ) );
	p.drawPath( bound );

	//Draw shading
	QPainterPath shading;
	static const float pi = std::acos( -1.0f );
	//const float amountFilled = ( sliderPosition - lowerBound() ) / ( upperBound() - lowerBound() ) * 2.0 - 1.0; //between -1 and 1
	const float scaledAmountFilled = atan( model->getSliderPosition() ) / pi + 0.5f; //between 0 and 1
	shading.addRoundedRect( QRectF( 1.5f, 1.5f, ( width() - 2 ) * scaledAmountFilled, height()-2 ), 0, 0 );
	QColor shadingColor = Qt::white;
	shadingColor.setAlpha( 20 );
	p.fillPath( shading, shadingColor );

	//If not in editing mode, draw slider value
	if( !editor->isVisible() )
		{
		p.setPen( QColor::fromHsl( 0, 0, 180 ) );
		QFont font( "Arial", 10 );
		font.setStyleHint( QFont::Monospace );
		p.setFont( font );
		p.drawText( 0, 0, width(), height(), Qt::AlignCenter,
			QString::number( (double) std::round( model->getSliderPosition() * 1000.0f ) / 1000.0 ) );
		}
	}


void NumberSliderView::mousePressEvent( QMouseEvent * me )
	{
	if( me->buttons() & Qt::LeftButton || me->buttons() & Qt::RightButton )
		{
		if( QApplication::queryKeyboardModifiers() & Qt::ControlModifier )
			{
			model->setSliderPositionStatic( model->defaultValue );
			}
		else
			{
			grabbedPos = me->pos();
			}
		}
	}

void NumberSliderView::mouseReleaseEvent( QMouseEvent * )
	{
	if( sliderGrabbed )
		{
		sliderGrabbed = false;
		model->pushStateChange( "Slider moved" );
		}
	}

void NumberSliderView::mouseMoveEvent( QMouseEvent * me )
	{
	sliderGrabbed = true;
	const QPoint deltaPos = me->pos() - grabbedPos;
	grabbedPos = me->pos();
	float sliderPos = model->getSliderPositionUnfiltered();
	const float scaling = ( me->buttons() & Qt::LeftButton ) ? .01f : .001f;
	const float newPos = sliderPos + float( deltaPos.x() - deltaPos.y() ) * ( sqrt( abs( sliderPos ) ) + 0.1f ) * scaling;
	model->setSliderPositionDynamic( std::round( newPos * 1000.0f ) / 1000.0 );
	update();
	}

void NumberSliderView::mouseDoubleClickEvent( QMouseEvent * )
	{
	editor->setVisible( true );
	editor->setText( "" );
	editor->setFocus();
	}

