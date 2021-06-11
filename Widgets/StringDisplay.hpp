#pragma once

#include <QWidget>
#include <QUndoCommand>

#include "LotonController.hpp"

class QLineEdit;
class QLabel;

class StringDisplayModel : public LotonController
{
public:
	StringDisplayModel( const QString & initial );

	QString string() const;
	void setString( const QString & );

private:
	QString _string;

	QVariant state() const override;
	void setState( const QVariant & ) override;
};

class StringDisplayView : public QWidget
{ Q_OBJECT
public:
	StringDisplayView( StringDisplayModel * model );

	void paintEvent( QPaintEvent * ) override;

private:
	StringDisplayModel * model;
};

class StringDisplay : public StringDisplayModel, public StringDisplayView
{
public:
	StringDisplay( const QString & initial = "" )
		: StringDisplayModel( initial )
		, StringDisplayView( this )
		{}
};
