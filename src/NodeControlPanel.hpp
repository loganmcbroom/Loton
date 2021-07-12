#pragma once

#include <QScrollArea>

class LotonNodeModel;

class NodeControlPanel : public QScrollArea
{ Q_OBJECT
public:
	NodeControlPanel( QWidget * parent = nullptr );

	LotonNodeModel * activeNode();
	void setActiveNode( LotonNodeModel * newNode = nullptr );

private:
	void resizeEvent( QResizeEvent * ) override;

	LotonNodeModel * _activeNode;
};

//#pragma once

//#include <QListWidget>

//class LotonNodeModel;

//class NodeControlPanel : public QListWidget
//{ Q_OBJECT
//public:
//	NodeControlPanel( QWidget * parent = nullptr );

//	void setActivePanel( LotonNodeModel * newNode = nullptr );
//	const LotonNodeModel * getActiveNode() const;

//private:
//	void resizeEvent( QResizeEvent * ) override;

//	LotonNodeModel * activeNode;
//};


