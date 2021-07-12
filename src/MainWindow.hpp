#pragma once

#include <QMainWindow>

//temp
#include <QKeyEvent>
#include <QAction>

class CentralWidgetManager;
class LotonNodeModel;

class MainWindow : public QMainWindow
{ Q_OBJECT
public:
	static void instantiate() { instance = new MainWindow; }
	static MainWindow * getInstance() { return instance; }

	static LotonNodeModel * activeNode();
	static void setActiveNode( LotonNodeModel * );

	CentralWidgetManager * centralWidgetManager;


public slots:
	void open( QString filename = QString() );
	bool save();
    bool saveAs();
	void newProject();
	void duplicateNode();
	void resetZoom();
	void findNode();
    void settings();
	void undo();
	void redo();
	void sampleFolderSelect();
	void projectFolderSelect();
	void updateTitle();

	void createMixNode();
	void createJoinNode();
	void createCutNode();
	void createSetVolumeNode();
	void createPanNode();

private:
	friend class SettingsMenu;

	MainWindow();
	static MainWindow * instance;

	void createNode( QString nodeName );
    void closeEvent( QCloseEvent * event ) override;
    bool maybeSave();
    void loadProject( const QString & fileName );
    void saveProject( const QString & fileName );

    void createActions();
    void createMenus();

	void paintAs( const QPalette & pal );

    QAction * openAct;
    QAction * saveAct;
	QAction * saveAsAct;
	QAction * newAct;
	QAction * duplicateAct;
	QAction * resetZoomAct;
	QAction * findNodeAct;
	QAction * settingsAct;
	QAction * undoAct;
	QAction * redoAct;
	QAction * sampleFolderSelectAct;
	QAction * projectFolderSelectAct;

	QAction * nodeMixAct;
	QAction * nodeJoinAct;
	QAction * nodeCutAct;
	QAction * nodeSetVolumeAct;
	QAction * nodePanAct;
};


//This is all to fix some bullshit with color dialogs, don't worry about it
#include <QColorDialog>
class ReleaseNotifyColorDialogEventFilter : public QObject
{ Q_OBJECT
public:
	bool eventFilter( QObject * obj, QEvent * event );
signals:
	void mouseReleased();
};

class ReleaseNotifyColorDialog : public QColorDialog
{ Q_OBJECT
public:
	ReleaseNotifyColorDialog( QColor c );

signals:
	void repaintUI();
};
