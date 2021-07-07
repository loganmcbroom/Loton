#pragma once

#include <QTreeWidget>

class LotonFlowScene;
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

	ProcessExplorer( LotonFlowScene * scene );

	ProcessTree * tree; // Memory managed by layout

	void engage();

private slots:
	void handleFilterChanged( const QString & text );
	void handleItemClicked( QTreeWidgetItem * item, int column );

private:
	QLineEdit * filter;
};
