#pragma once

#include <QFileSystemModel>
#include <QTreeView>

class SampleExplorerSortProxy;

class SampleExplorerModel : public QFileSystemModel
{ Q_OBJECT
public:
	SampleExplorerModel();

signals:
	void sampleSaved( QModelIndex );

private:
	Qt::DropActions supportedDropActions() const override;
	Qt::ItemFlags flags( const QModelIndex & index ) const override;
	QStringList mimeTypes() const override;
	QMimeData * mimeData( const QModelIndexList & indexes ) const override;

	// Decide if data can be dropped
	bool canDropMimeData( const QMimeData *, Qt::DropAction,
		int, int, const QModelIndex & ) const override;

	// Handle dropped data
	bool dropMimeData( const QMimeData * mimeData, Qt::DropAction,
		int, int, const QModelIndex & i ) override;
};

class SampleExplorer : public QTreeView
{ Q_OBJECT
public:
	SampleExplorer();

	void setRootPath ( QString path );

private slots:
	void handleSampleSaved( QModelIndex );
	//void handleFileRenamed(

private:
	void keyPressEvent( QKeyEvent * event ) override;

	std::unique_ptr<SampleExplorerModel> model;
	std::unique_ptr<SampleExplorerSortProxy> sortFilter;
};
