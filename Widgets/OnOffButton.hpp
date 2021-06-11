#pragma once

#include <QWidget>
#include <QPushButton>

#include "LotonController.hpp"

class QLineEdit;
class QLabel;

class OnOffButtonModel : public LotonController
{
public:
	static const int infinity = 3133700;

	OnOffButtonModel( bool initialValue );

	bool getButtonPosition() const;
	void setButtonPosition( bool );

private:
	QVariant state() const override;
	void setState( const QVariant & ) override;

	bool buttonPosition;
};

class OnOffButtonView : public QPushButton
{ Q_OBJECT
public:
	OnOffButtonView( OnOffButtonModel * model );

	void paintEvent( QPaintEvent * ) override;
	void mousePressEvent( QMouseEvent * me ) override;
	void mouseReleaseEvent( QMouseEvent * me ) override;

private:
	OnOffButtonModel * model;
	bool pressing;
};
