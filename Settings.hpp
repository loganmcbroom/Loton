#pragma once

#include <QString>
#include <QPalette>

class QDir;
class QWidget;

class Settings : public QObject
{ Q_OBJECT
public:
	static Settings * get();

	static const QPalette & defaultPalette();
	static const QPalette & palette();
	static void setPalette( const QPalette & );
	static void setDynamicColor( QWidget *, QString styleText, QPalette::ColorRole );

	static QString & samplesPath();
	static QString & projectsPath();
	static QString & currentProjectPath();
	static size_t PVOCWindowSize(); // Number of frames processed per hop, default is 2048
	static size_t PVOCHopSize(); // Number of frames to hop, default is 128
	static size_t PVOCFFTSize(); // This is stored as the exponent of 2 to use, default 12 (2^12 = 4096)
	static size_t processTimeout(); // Number of milliseconds before process times out
	static void setPVOCWindowSize( int newWindowSize );
	static void setPVOCHopSize( int newHopSize );
	static void setPVOCFFTSize( int newFFTSize );
	static void setProcessTimeout( int ms );

signals:
	void paletteUpdated( QPalette::ColorRole );

public slots:
	void saveToRegistry();

private:
	Settings();
	~Settings();

	class impl;
	impl * pImpl;
	static bool instanceExists;
};


