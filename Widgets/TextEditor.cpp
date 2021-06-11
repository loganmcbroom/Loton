#include "TextEditor.hpp"

#include <functional>
#include <string>

#include <QTextDocument>
#include <QKeyEvent>
#include <QDebug>
#include <QAction>
#include <QSyntaxHighlighter>

#include "LuaSyntaxHighlighter.hpp"

#include "MainWindow.hpp"

// Document ========================================================================================================================================

std::unordered_map<QUuid, std::weak_ptr<TextEditorDocumunt>> TextEditorDocumunt::registry;

std::shared_ptr<TextEditorDocumunt> TextEditorDocumunt::make( QUuid id, const QString & init )
	{
	auto out = std::make_shared<TextEditorDocumunt>( id, init );
	registry[id] = std::weak_ptr<TextEditorDocumunt>( out );
	return out;
	}

TextEditorDocumunt::TextEditorDocumunt( QUuid _id, const QString & init )
	: QTextDocument( init )
	, id( _id )
	{
	//QObject::connect( this, &QTextDocument::undoCommandAdded, this, &TextEditorDocumunt::documentStateChanged );
	new LuaSyntaxHighlighter( this ); // This becomes parent
	}

TextEditorDocumunt::~TextEditorDocumunt()
	{
	registry.erase( id );
	}



// Model ========================================================================================================================================

TextEditorModel::TextEditorModel( const QString & initial )
	: LotonController( initial )
	, doc( TextEditorDocumunt::make( id(), initial ) )
	{
	QObject::connect( this, &TextEditorModel::documentChanged, this, &TextEditorModel::handleDocumentChanged );
	emit documentChanged();
	}

QVariant TextEditorModel::state() const
	{
	return doc->toPlainText();
	}

void TextEditorModel::setState( const QVariant & v )
	{
	if( TextEditorDocumunt::registry.count( id() ) != 0 ) // Restoring a living doc
		{
		doc = TextEditorDocumunt::registry[ id() ].lock();
		Q_ASSERT( doc );
		emit documentChanged();
		}
	else // Restoring from file
		{
		doc->setPlainText( v.toString() );
		}
	}

void TextEditorModel::handleDocumentChanged()
	{
	QObject::connect( doc.get(), &TextEditorDocumunt::contentsChanged, this, &TextEditorModel::stateChanged );
	QObject::connect( doc.get(), &QTextDocument::undoCommandAdded, this, [this]()
		{
		LotonController::manager().undoStack->push( new TextChangeProxyCommand( doc ) );
		} );
	emit stateChanged();
	}



// View ========================================================================================================================================

// Stops undo and redo override
class TextEditorShortcutFilter : public QObject
{
public:
	TextEditorShortcutFilter( TextEditorView * o ) : QObject( o ) {}
	bool eventFilter( QObject *, QEvent * event ) override
		{
		if( event->type() == QEvent::ShortcutOverride )
			{
			QKeyEvent * e = static_cast<QKeyEvent *>( event );
			return ( e->key() == Qt::Key_Z || e->key() == Qt::Key_Y  ) && e->modifiers() & Qt::ControlModifier;
			}
		return false;
		}
};

TextEditorView::TextEditorView( TextEditorModel * inModel )
	: QTextEdit()
	{
	setDocument( inModel->doc.get() );
	installEventFilter( new TextEditorShortcutFilter( this ) );
	setStyleSheet("font: 9pt \"Courier\"; background-color: #272822; selection-background-color: #49483E");

	// Fix tab size to 4 spaces
	QFont f( "Courier" );
	f.setPointSize( 9 );
	QFontMetrics m( f );
	setTabStopDistance( 4 * m.width( ' ' ) );
	}

//void TextEditorView::focusOutEvent( QFocusEvent * )
//	{
//	emit static_cast<TextEditorDocumunt *>( document() )->documentStateChanged();
//	}



// Command ========================================================================================================================================

TextChangeProxyCommand::TextChangeProxyCommand( std::shared_ptr<TextEditorDocumunt> _doc )
	: QUndoCommand( "Text Changed" )
	, doc( _doc )
	{
	}

void TextChangeProxyCommand::undo()
	{
	doc->undo();
	//doc->documentStateChanged();
	}

void TextChangeProxyCommand::redo()
	{
	if( pushRedoStopper )
		{
		pushRedoStopper = false;
		return;
		}
	doc->redo();
	//doc->documentStateChanged();
	}

