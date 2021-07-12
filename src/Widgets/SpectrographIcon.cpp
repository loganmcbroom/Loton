#include "SpectrographIcon.hpp"

#include <QColor>
#include <QPixmap>
#include <QtConcurrent>
#include <QDrag>

#include <flan/PVOC.h>

#include "NodeDataTypes/PVOCData.hpp"
#include "Settings.hpp"

// Model =====================================================================

SpectrographIconModel:: SpectrographIconModel()
	{
	}

void SpectrographIconModel::setPVOC( std::shared_ptr<PVOCData> in )
	{
	pvoc = in;
	emit dataUpdated( in );
	}

// View ======================================================================

SpectrographIconView::SpectrographIconView( SpectrographIconModel * _model )
	: model( _model )
	, icon()
	, watcher()
	, canceller( nullptr )
	{
	QObject::connect( model, &SpectrographIconModel::destroyed, this, [this](){ model = nullptr; } );
	QObject::connect( model, &SpectrographIconModel::dataUpdated, this, &SpectrographIconView::updateData );
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
	updateData( model->pvoc );
	}

QImage SpectrographIconView::getIcon() const
	{
	flan::Graph b = !icon.isCanceled() && icon.isFinished()? icon.result() : flan::Graph();
	return QImage( b.data(), b.width(), b.height(), QImage::Format_BGR888 );
	}

void SpectrographIconView::updateData( std::shared_ptr<PVOCData> in )
	{
	icon = QFuture<flan::Graph>();
	watcher.setFuture( icon );
	if( canceller ) canceller->store( true );

	if( !in ) return;

	canceller.reset( new std::atomic<bool>( false ) );
	const QSize currentSize = size();
	icon = QtConcurrent::run( [in, currentSize, c = canceller]()
		{
		const float timelineScale = currentSize.height() > 64 ?
			std::min( currentSize.width() * .025f, 12.0f )
			: 0;
		return in->pvoc.convertToGraph( flan::Rect( 0, 0, -1, -1 ), currentSize.width(), currentSize.height(), timelineScale, *c );
		});

	QObject::connect( &watcher, &QFutureWatcher<QImage>::finished,
		this, &SpectrographIconView::onDataUpdated );

	watcher.setFuture( icon );
	}

void SpectrographIconView::onDataUpdated()
	{
	setPixmap( QPixmap::fromImage( getIcon() ) );
	}

void SpectrographIconView::resizeEvent( QResizeEvent * e )
	{
	QWidget::resizeEvent( e );
	updateData( model->pvoc );
	}

void SpectrographIconView::mousePressEvent( QMouseEvent * event )
	{
	if( model && model->pvoc )
		{
		flan::PVOC * pPVOC = &model->pvoc->pvoc;

		QMimeData * mimeData = new QMimeData;
		auto bytes = QByteArray::fromRawData( (char*) &pPVOC, sizeof( pPVOC ) );
		mimeData->setData( "pointer/flan::PVOC", bytes );

		QDrag * drag = new QDrag( model );
		drag->setMimeData( mimeData );
		drag->exec();
		}
	}
