#include "ProcessExplorer.hpp"

#include <QLineEdit>
#include <QWidgetAction>
#include <QTreeWidget>
#include <QHeaderView>
#include <QBoxLayout>
#include <QMimeData>
#include <QDrag>

#include <nodes/Node>

#include "LotonFlowScene.hpp"

//===================================================================================
// ProcessTree

ProcessTree::ProcessTree()
	{
	setDragEnabled( true );
	}

//===================================================================================
// ProcessExplorer

QString ProcessExplorer::skipText = QStringLiteral("skip me");

// This widget is a nearly direct copy of the NodeEditor context menu
ProcessExplorer::ProcessExplorer( LotonFlowScene * scene )
	{
	setLayout( new QVBoxLayout );
	layout()->setContentsMargins( 0, 0, 0, 0 );

	// Filter setup
	filter = new QLineEdit();
	layout()->addWidget( filter );
	filter->setPlaceholderText( QStringLiteral("Processes... ") );
	filter->setClearButtonEnabled( true );
	QObject::connect( filter, &QLineEdit::textChanged,
		this, &ProcessExplorer::handleFilterChanged );

	// Process tree setup
	tree = new ProcessTree();
	layout()->addWidget( tree );
	tree->header()->close();
//	QObject::connect( tree, &QTreeWidget::itemClicked,
//		this, &ProcessTree::handleItemClicked );

	// Add categories to tree
	QMap<QString, QTreeWidgetItem*> categories;
	for( auto const & cat : scene->registry().categories() )
		{
		auto item = new QTreeWidgetItem( tree );
		item->setText( 0, cat );
		item->setData( 0, Qt::UserRole, skipText );
		categories[cat] = item;
		}

	// Add category entries to tree
	for( auto const & assoc : scene->registry().registeredModelsCategoryAssociation() )
		{
		auto parent = categories[assoc.second];
		auto item   = new QTreeWidgetItem( parent );
		item->setText( 0, assoc.first);
		item->setData( 0, Qt::UserRole, assoc.first );
		}
	}

void ProcessExplorer::engage()
	{
	filter->setFocus();
	filter->selectAll();
	}

void ProcessExplorer::handleFilterChanged( const QString & text )
	{
	for( int catIndex = 0; catIndex < tree->topLevelItemCount(); ++catIndex )
		{
		auto category = tree->topLevelItem( catIndex );
		for( int itemIndex = 0; itemIndex < category->childCount(); ++itemIndex )
			{
			auto child = category->child( itemIndex );
			auto modelName = child->data( 0, Qt::UserRole ).toString();
			const bool match = modelName.contains( text, Qt::CaseInsensitive );
			child->setHidden( ! match );
			}
		}
	}

void ProcessExplorer::handleItemClicked( QTreeWidgetItem * item, int column )
	{
	QString modelName = item->data(0, Qt::UserRole).toString();

	if( modelName == skipText ) return;

	//Create drag
	QMimeData * mimeData = new QMimeData;
	mimeData->setData( "text/Node", modelName.toUtf8() );

	QDrag * drag = new QDrag( this );
	drag->setMimeData( mimeData );
	drag->exec();
	}
