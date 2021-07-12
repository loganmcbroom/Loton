#include "ComboBox.hpp"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

#include "MainWindow.hpp"

LotonComboBoxModel::LotonComboBoxModel( int initial, const QStringList & _options )
	: LotonController( initial )
	, options( _options )
	, _selection( initial )
	{
	}

int LotonComboBoxModel::selection() const
	{
	return _selection;
	}

void LotonComboBoxModel::setSelection( int newSelection )
	{
	_selection = newSelection;
	pushStateChange( "Selection changed" );
	}

QVariant LotonComboBoxModel::state() const
	{
	return selection();
	}

void LotonComboBoxModel::setState( const QVariant & s )
	{
	_selection = s.toInt();
	}

LotonComboBoxView::LotonComboBoxView( LotonComboBoxModel * inModel )
	: QComboBox()
	, model( inModel )
	{
	addItems( model->options );

	setCurrentIndex( static_cast<int>( model->selection() ) );

	QObject::connect( model, &LotonComboBoxModel::destroyed, this, [this](){ model = nullptr; } );
	QObject::connect( this, QOverload<int>::of( &QComboBox::currentIndexChanged ), model, [this]( int i )
		{
		model->setSelection( static_cast<int>( i ) );
		});
	QObject::connect( model, &LotonComboBoxModel::stateChanged, this, [this]()
		{
		if( currentIndex() != model->selection() )
			{
			bool oldState = blockSignals( true );
			setCurrentIndex( static_cast<int>( model->selection() ) );
			blockSignals( oldState );
			}
		});
	}


