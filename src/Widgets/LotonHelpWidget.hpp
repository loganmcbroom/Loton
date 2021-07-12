#pragma once

#include <QLabel>
#include <QTextEdit >

class LotonNodeModel;

class LotonHelpWidget : public QTextEdit
{
public:
	void setActiveNode( LotonNodeModel * );

	LotonHelpWidget();

	QString text;
};
