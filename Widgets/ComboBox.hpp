#pragma once

#include <QComboBox>
#include <QUndoCommand>
//#include <QUuid>

#include "LotonController.hpp"

class LotonComboBoxModel : public LotonController
{
public:
	LotonComboBoxModel( int initial, const QStringList & options = {} );

	int selection() const;
	void setSelection( int );

	const QStringList options;

private:
	int _selection;

	QVariant state() const override;
	void setState( const QVariant & ) override;
};

class LotonComboBoxView : public QComboBox
{ Q_OBJECT
public:
	LotonComboBoxView( LotonComboBoxModel * model );

private:
	LotonComboBoxModel * model;
};
