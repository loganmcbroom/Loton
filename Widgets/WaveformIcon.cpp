#include "WaveformIcon.hpp"

#include <QColor>
#include <QPixmap>
#include <QtConcurrent>
#include <QDrag>
#include <QMouseEvent>

#include <flan/Audio.h>

#include "NodeDataTypes/AudioData.hpp"
#include "Settings.hpp"

#include <iostream> // temp

// Model =====================================================================

WaveformIconModel:: WaveformIconModel()
	{
	}

void WaveformIconModel::setAudio( std::shared_ptr<AudioData> in )
	{
	audio = in;
	emit dataUpdated( in );
    }

// View ======================================================================

WaveformIconView::WaveformIconView( WaveformIconModel * _model )
	: model( _model )
	, icon()
	, watcher()
	, canceller( nullptr )
	{
	QObject::connect( model, &WaveformIconModel::destroyed,
		this, [this](){ model = nullptr; } );
	QObject::connect( model, &WaveformIconModel::dataUpdated,
		this, &WaveformIconView::updateData );
//	QObject::connect( Settings::get(), &Settings::paletteUpdated, this, [this]( QPalette::ColorRole role )
//		{
//		if( role == QPalette::Highlight )
//			{
//			QImage current = getIcon();
//			current.setColor( 1, Settings::palette().color( role ).rgb() );
//			setPixmap( QPixmap::fromImage( current ) );
//			repaint();
//			}
//		} );
	//updateData( model->audio );
	}

QImage WaveformIconView::getIcon() const
	{
	flan::Graph b = !icon.isCanceled() && icon.isFinished()? icon.result() : flan::Graph();
	QImage q( b.data(), b.width(), b.height(), QImage::Format_BGR888 );
	return q;
	}

void WaveformIconView::updateData( std::shared_ptr<AudioData> in )
	{
	icon = QFuture<flan::Graph>();
	watcher.setFuture( icon );
	if( canceller ) canceller->store( true );

	if( !in ) return;

	canceller.reset( new std::atomic<bool>( false ) );
	const QSize currentSize = size();
	icon = QtConcurrent::run( [in, currentSize, c = canceller]()
		{
		return in->audio.convertToGraph( flan::Interval( 0, -1 ), currentSize.width(), currentSize.height(),
			currentSize.height() > 64? currentSize.height() * .075f : 0, *c );
		});

	QObject::connect( &watcher, &QFutureWatcher<QImage>::finished, this, &WaveformIconView::handleBitmapFinished );

	watcher.setFuture( icon );
	}

void WaveformIconView::handleBitmapFinished()
	{
	setPixmap( QPixmap::fromImage( getIcon() ) );
	emit dataUpdated();
	}

void WaveformIconView::resizeEvent( QResizeEvent * e )
	{
	QWidget::resizeEvent( e );
	updateData( model->audio );
	}

//void WaveformIconView::wheelEvent( QWheelEvent * event )
//	{
//	}

void WaveformIconView::mousePressEvent( QMouseEvent * event )
	{
	if( model && model->audio )
		{
		flan::Audio * pAudio = &model->audio->audio;

		QMimeData * mimeData = new QMimeData;
		auto bytes = QByteArray::fromRawData( (char*) &pAudio, sizeof( pAudio ) );
		mimeData->setData( "pointer/flan::Audio", bytes );

		QDrag * drag = new QDrag( model );
		drag->setMimeData( mimeData );
		drag->exec();
		}
	}
