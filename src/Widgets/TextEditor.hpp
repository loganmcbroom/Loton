#pragma once

#include <QTextEdit>
#include <QUndoCommand>

#include "LotonController.hpp"

class TextEditorDocumunt : public QTextDocument
{ Q_OBJECT
public:
	static std::shared_ptr<TextEditorDocumunt> make( QUuid, const QString & = "" );
	static std::unordered_map<QUuid, std::weak_ptr<TextEditorDocumunt>> registry; // Manages all living documents, to maintain their stacks

	TextEditorDocumunt( QUuid, const QString & = "" );
	~TextEditorDocumunt();

	const QUuid id;

signals:
	//void documentStateChanged();
};



class TextEditorModel : public LotonController
{ Q_OBJECT
public:
	TextEditorModel( const QString & initial );

	std::shared_ptr<TextEditorDocumunt> doc;

signals:
	void documentChanged();

private:
	QVariant state() const override;
	void setState( const QVariant & ) override;
	void handleDocumentChanged();
};



class TextEditorView : public QTextEdit
{ Q_OBJECT
public:
	TextEditorView( TextEditorModel * model );

private:
//	void focusOutEvent( QFocusEvent * e ) override;
};



class TextChangeProxyCommand : public QUndoCommand
{
public:
	TextChangeProxyCommand( std::shared_ptr<TextEditorDocumunt> doc );
	std::shared_ptr<TextEditorDocumunt> doc;
	bool pushRedoStopper = true;

	void undo() override;
	void redo() override;
};
