#include "SampleExplorer.hpp"

#include <QDebug>
#include <QMimeData>
#include <QDateTime>
#include <QSortFilterProxyModel>
#include <QKeyEvent>
#include <QMessageBox>
#include <QFile>

#include <flan/Audio.h>
#include <flan/PVOC.h>

#include "Settings.hpp"

class SampleExplorerSortProxy : public QSortFilterProxyModel
{
public:
	SampleExplorerSortProxy()
		: QSortFilterProxyModel()
		{
		setRecursiveFilteringEnabled( true );
		setSortCaseSensitivity( Qt::CaseInsensitive );
		}

	bool lessThan( const QModelIndex & left, const QModelIndex & right ) const
		{
		QFileSystemModel * fsm = qobject_cast<QFileSystemModel *>( sourceModel() );

		QFileInfo leftFileInfo  = fsm->fileInfo( left  );
		QFileInfo rightFileInfo = fsm->fileInfo( right );

		// Move dirs upper
		if( !leftFileInfo.isDir() &&  rightFileInfo.isDir() ) return false;
		if(  leftFileInfo.isDir() && !rightFileInfo.isDir() ) return true;

		//I have no idea why the negation is needed, but it is
		return QSortFilterProxyModel::lessThan( left, right );
		}
};

//===========================================================================
// Model

SampleExplorerModel::SampleExplorerModel()
	: QFileSystemModel()
	{
	setReadOnly( false );

	setRootPath( QDir::currentPath() );
	setFilter(  QDir::AllDirs | QDir::Files | QDir::NoDot | QDir::NoDotDot );
	setNameFilters( {"*.wav", "*.flac", "*.aiff", "*.flan"} );
	setNameFilterDisables( false );
	}

Qt::DropActions SampleExplorerModel::supportedDropActions() const
	{
	return Qt::CopyAction | Qt::MoveAction;
	}

Qt::ItemFlags SampleExplorerModel::flags( const QModelIndex & index ) const
	{
	Qt::ItemFlags defaultFlags = QFileSystemModel::flags( index );

	if( index.isValid() )
		return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
	else
		return Qt::ItemIsDropEnabled | defaultFlags;
	}

QStringList SampleExplorerModel::mimeTypes() const
	{
	QStringList types;
	types << "text/uri-list";
	return types;
	}

QMimeData * SampleExplorerModel::mimeData( const QModelIndexList & indexes ) const
	{
	//Get file urls
	QList<QUrl> urls;
	foreach( const QModelIndex & index, indexes )
		if( index.isValid() )
			urls.push_back( QUrl::fromLocalFile( filePath( index ) ) );

	//Convert urls to mime
	QMimeData * mimeData = new QMimeData;
	mimeData->setUrls( urls );
	return mimeData;
	}

// Decide if data can be dropped
bool SampleExplorerModel::canDropMimeData( const QMimeData * data, Qt::DropAction,
	int, int column, const QModelIndex & ) const
	{
	if( ! data->hasFormat( "pointer/flan::Audio" )	// flan::Audio
	 && ! data->hasFormat( "pointer/flan::PVOC" ) ) // flan::PVOC
		return false;

	if( column > 0 )
		return false;

	return true;
	}

bool SampleExplorerModel::dropMimeData( const QMimeData * mimeData, Qt::DropAction,
	int, int, const QModelIndex & i )
	{
	QModelIndex rootPathIndex = index( rootPath() );
	QString droppedPath = filePath( i );
	QFileInfo droppedPathInfo( droppedPath );
	if( ! droppedPathInfo.dir().exists() ) return false;

	// Path setup
	//drop on folder -> save to folder
	//drop on file -> save to folder that contains file
	QString savePath;
	if( droppedPathInfo.isDir() ) // Check if dropped on file
		savePath = droppedPath;
	else if( droppedPathInfo.isFile() )
		savePath = droppedPathInfo.dir().absolutePath();
	else
		{
		qDebug() << "Sample Explorer recieved drop in unknown location ??? How?";
		return false;
		}
	savePath += "/_Loton_"
		+ QDateTime::currentDateTimeUtc().toString( "yyyy-MM-dd-HH-mm-ss-zzz" );
	qDebug() << savePath;


	// Super illegal typeless pointer drag and drop action
	// What, you want me to copy a flan file to clipboard?
	// We could use a registry and pass ids, but I won't.

	if( mimeData->hasFormat( "pointer/flan::Audio" ) )
		{
		QByteArray bytes = mimeData->data( "pointer/flan::Audio" );
		flan::Audio * pAudio = *( flan::Audio ** )( bytes.data() );
		savePath += ".wav";
		bool saved = pAudio->save( savePath.toStdString() );
		if( !saved ) return false;
		}
	else if( mimeData->hasFormat( "pointer/flan::PVOC" ) )
		{
		QByteArray bytes = mimeData->data( "pointer/flan::PVOC" );
		flan::PVOC * pPVOC = *( flan::PVOC ** )( bytes.data() );
		savePath += ".flan";
		bool saved = pPVOC->save( savePath.toStdString() );
		if( !saved ) return false;
		}
	else
		return false;

	QModelIndex savedSampleIndex = index( savePath );
	if( savedSampleIndex.isValid() )
		emit sampleSaved( savedSampleIndex );
	else qDebug() << "SampleExplorerModel::dropMimeData: Couldn't find the saved file ???";

	return true;
	}


//===========================================================================
// View

SampleExplorer::SampleExplorer()
	: QTreeView( nullptr )
	, model( new SampleExplorerModel )
	, sortFilter( new SampleExplorerSortProxy )
	{
	sortFilter->setSourceModel( model.get() );

	// Set options
	setStyleSheet( "QTreeView { margin: 0px; border-width: 0px; }" );
	setModel( sortFilter.get() );
	setColumnHidden( 1, true );
	setColumnHidden( 2, true );
	setColumnHidden( 3, true );
	setHeaderHidden( true );
	setRootIndex( sortFilter->mapFromSource( model->index( Settings::samplesPath() ) ) );
	setSortingEnabled( true );
	setSelectionMode( QAbstractItemView::ExtendedSelection );
	setDragEnabled( true );
	setAcceptDrops( true );
	setDropIndicatorShown( true );
	setAutoScroll( true );
	setEditTriggers( QAbstractItemView::SelectedClicked | QAbstractItemView::EditKeyPressed );

	QObject::connect( model.get(), &SampleExplorerModel::sampleSaved,
		this, &SampleExplorer::handleSampleSaved );

	QObject::connect( model.get(), &QFileSystemModel::fileRenamed, this, [this]()
		{
		sortFilter->sort( 0 );
		repaint();
		} );
	sortFilter->sort( 0 );
	}

void SampleExplorer::setRootPath( QString path )
	{
	setRootIndex( sortFilter->mapFromSource( model->index( path ) ) );
	}

void SampleExplorer::handleSampleSaved( QModelIndex i )
	{
	if( i.isValid() )
		{
		auto mappedIndex = sortFilter->mapFromSource( i );
		setCurrentIndex( mappedIndex );
		edit( mappedIndex );
		}
	}

void SampleExplorer::keyPressEvent( QKeyEvent * event )
	{
	if( event->key() == Qt::Key_Delete )
		{
		event->accept();

		//Double check
		auto question = QMessageBox::question( this,
			"Delete Files?",
			"Are you sure you want to permanently delete this?" );
		if( question != QMessageBox::Yes ) return;

		auto is = selectedIndexes();
		for( auto & i : is )
			{
			auto mappedIndex = sortFilter->mapToSource( i );
			QFile::remove( model->filePath( mappedIndex ) );
			//qDebug() << "Killing: " << model->filePath( mappedIndex );
			}
		}
	else QTreeView::keyPressEvent( event );
	}


