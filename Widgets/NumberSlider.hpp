#pragma once

#include <QWidget>
#include <QUndoCommand>

#include "LotonController.hpp"

class QLineEdit;
class QLabel;

class NumberSliderModel : public LotonController
{
public:
	static const int infinity = 3133700;

	NumberSliderModel( float defaultValue = 0,
		float lowerBound = -infinity,
		float upperBound = infinity );

	float getSliderPosition() const;
	float getSliderPositionUnfiltered() const;
	void setSliderPositionStatic( float );
	void setSliderPositionDynamic( float );

	void setFilter( std::function< float ( float ) > );

	const float defaultValue;
	const float lowerBound;
	const float upperBound;

private:
	QVariant state() const override;
	void setState( const QVariant & ) override;

	float sliderPosition;
	std::function< float ( float ) > filter;
};

class NumberSliderView : public QWidget
{ Q_OBJECT
public:
	NumberSliderView( NumberSliderModel * model );

	void paintEvent( QPaintEvent * ) override;

	void mousePressEvent( QMouseEvent * me ) override;
	void mouseReleaseEvent( QMouseEvent * me ) override;
	void mouseMoveEvent( QMouseEvent * me ) override;
	void mouseDoubleClickEvent( QMouseEvent * ) override;

private:
	NumberSliderModel * model;
	QPoint grabbedPos;
	QLineEdit * editor;
	bool sliderGrabbed;
};

class NumberSlider : public NumberSliderModel, public NumberSliderView
{
public:
	NumberSlider( float _defaultValue, float lowBound, float highBound )
		: NumberSliderModel( _defaultValue, lowBound, highBound )
		, NumberSliderView( this )
		{}

	static const int infinity = NumberSliderModel::infinity;
};
