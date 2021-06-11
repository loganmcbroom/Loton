#pragma once

#include <QTreeWidget>

class XFlowScene;
class QTreeWidgetItem;

class ProcessTree : public QTreeWidget
{
public:
	ProcessTree();
};

class ProcessExplorer : public QWidget
{ Q_OBJECT
public:
	static QString skipText;

	ProcessExplorer( XFlowScene * scene );

	ProcessTree * tree; // Memory managed by layout

	void engage();

private slots:
	void handleFilterChanged( const QString & text );
	void handleItemClicked( QTreeWidgetItem * item, int column );

private:
	QLineEdit * filter;
};
