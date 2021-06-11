#include "Settings.hpp"

#include <QDir>
#include <QSettings>
#include <QWidget>
#include <QDebug>

bool Settings::instanceExists = false;

class Settings::impl
{
public:
	impl()
		{
		defaultPal.setColor( QPalette::Highlight,	QColor::fromHsl( 117, 128, 170 ) );
		defaultPal.setColor( QPalette::Light,		QColor::fromHsl( 1, 165, 140 ) );
		defaultPal.setColor( QPalette::Mid,			QColor::fromHslF( .58, .09, .21 ) );
		defaultPal.setColor( QPalette::Dark,		QColor::fromHslF( .58, .08, .14 ) );
		defaultPal.setColor( QPalette::Shadow,		QColor::fromHslF( .58, .08, .09 ) );

		defaultPVOCWindowSize = 2048;
		defaultPVOCHopSize = 128;
		defaultPVOCFFTSize = 12;
		defaultTimeout = 10000;
		}

	QPalette defaultPal;
	QPalette pal;

	size_t defaultPVOCWindowSize;
	size_t PVOCWindowSize;

	size_t defaultPVOCHopSize;
	size_t PVOCHopSize;

	size_t defaultPVOCFFTSize;
	size_t PVOCFFTSize;

	size_t defaultTimeout;
	size_t timeout;

	QString samplesPath;
	QString projectsPath;
	QString currentProjectPath;

};

Settings::Settings()
	: pImpl( new impl )
	{
	instanceExists = true;

	QSettings s;

	//Set defaults
	if( ! s.contains( "Palette/Highlight"	) ) s.setValue( "Palette/Highlight",	pImpl->defaultPal.color( QPalette::Highlight ) );
	if( ! s.contains( "Palette/Light"		) ) s.setValue( "Palette/Light",		pImpl->defaultPal.color( QPalette::Light	 ) );
	if( ! s.contains( "Palette/Mid"			) ) s.setValue( "Palette/Mid",			pImpl->defaultPal.color( QPalette::Mid		 ) );
	if( ! s.contains( "Palette/Dark"		) ) s.setValue( "Palette/Dark",			pImpl->defaultPal.color( QPalette::Dark		 ) );
	if( ! s.contains( "Palette/Shadow"		) ) s.setValue( "Palette/Shadow",		pImpl->defaultPal.color( QPalette::Shadow	 ) );
	if( ! s.contains( "samplesPath"			) ) s.setValue( "samplesPath",			QString() );
	if( ! s.contains( "projectsPath"		) ) s.setValue( "projectsPath",			QString() );
	if( ! s.contains( "currentProjectPath"	) ) s.setValue( "currentProjectPath",	QString() );
	if( ! s.contains( "PVOC/windowSize"		) ) s.setValue( "PVOC/windowSize",		pImpl->defaultPVOCWindowSize );
	if( ! s.contains( "PVOC/hopSize"		) ) s.setValue( "PVOC/hopSize",			pImpl->defaultPVOCHopSize );
	if( ! s.contains( "PVOC/fftSize"		) ) s.setValue( "PVOC/fftSize",			pImpl->defaultPVOCFFTSize );
	if( ! s.contains( "timeout"				) ) s.setValue( "timeout",				pImpl->defaultTimeout );

	//Read settings from registry
	pImpl->pal.setColor( QPalette::Highlight,	s.value( "Palette/Highlight" ).value<QColor>() );
	pImpl->pal.setColor( QPalette::Light,		s.value( "Palette/Light"	 ).value<QColor>() );
	pImpl->pal.setColor( QPalette::Mid,			s.value( "Palette/Mid"		 ).value<QColor>() );
	pImpl->pal.setColor( QPalette::Dark,		s.value( "Palette/Dark"		 ).value<QColor>() );
	pImpl->pal.setColor( QPalette::Shadow,		s.value( "Palette/Shadow"	 ).value<QColor>() );
	pImpl->samplesPath			= s.value( "samplesPath"		).value<QString>();
	pImpl->projectsPath			= s.value( "projectsPath"		).value<QString>();
	pImpl->currentProjectPath	= s.value( "currentProjectPath" ).value<QString>();
	pImpl->PVOCWindowSize		= s.value( "PVOC/windowSize"	).value<size_t>();
	pImpl->PVOCHopSize			= s.value( "PVOC/hopSize"		).value<size_t>();
	pImpl->PVOCFFTSize			= s.value( "PVOC/fftSize"		).value<size_t>();
	pImpl->timeout				= s.value( "timeout"			).value<size_t>();
	}

void Settings::saveToRegistry()
	{
	QSettings s;

	//Write settings to registry
	s.setValue( "Palette/Highlight",	pImpl->pal.color( QPalette::Highlight ) );
	s.setValue( "Palette/Light",		pImpl->pal.color( QPalette::Light ) );
	s.setValue( "Palette/Mid",			pImpl->pal.color( QPalette::Mid ) );
	s.setValue( "Palette/Dark",			pImpl->pal.color( QPalette::Dark ) );
	s.setValue( "Palette/Shadow",		pImpl->pal.color( QPalette::Shadow ) );
	s.setValue( "samplesPath",			pImpl->samplesPath );
	s.setValue( "projectsPath",			pImpl->projectsPath );
	s.setValue( "currentProjectPath",	pImpl->currentProjectPath );
	s.setValue( "PVOC/windowSize",		pImpl->PVOCWindowSize );
	s.setValue( "PVOC/hopSize",			pImpl->PVOCHopSize );
	s.setValue( "PVOC/fftSize",			pImpl->PVOCFFTSize );
	s.setValue( "timeout",				pImpl->timeout );
	}

Settings::~Settings()
	{
	instanceExists = false;
	saveToRegistry();

	delete pImpl;
	}

const QPalette & Settings::defaultPalette()
	{
	return get()->pImpl->defaultPal;
	}

const QPalette & Settings::palette()
	{
	return get()->pImpl->pal;
	}

void Settings::setPalette( const QPalette & newPal )
	{
	//Figure out which colors changed
	std::vector<QPalette::ColorRole> usedColorRoles =
		{
		QPalette::Shadow,
		QPalette::Dark,
		QPalette::Mid,
		QPalette::Light,
		QPalette::Highlight
		};
	std::vector<QPalette::ColorRole> changedColors;
	for( auto i : usedColorRoles )
		{
		if( get()->pImpl->pal.color( i ) != newPal.color( i ) )
			changedColors.push_back( i );
		}

	get()->pImpl->pal = newPal;

	for( auto i : changedColors )
		emit get()->paletteUpdated( i );
	}

void Settings::setDynamicColor( QWidget * widget, QString styleText, QPalette::ColorRole role )
	{
	QObject::connect( get(), &Settings::paletteUpdated, widget, [widget, styleText, role]( QPalette::ColorRole changedColor )
		{
		if( role == changedColor )
			widget->setStyleSheet( styleText.arg( get()->palette().color( role ).rgba(), 0, 16 ) );
		});
	widget->setStyleSheet( styleText.arg( get()->palette().color( role ).rgba(), 0, 16 ) );
	}

QString & Settings::samplesPath()
	{
	return get()->pImpl->samplesPath;
	}

QString & Settings::projectsPath() { return get()->pImpl->projectsPath;	}

QString & Settings::currentProjectPath() { return get()->pImpl->currentProjectPath;	}

size_t Settings::PVOCWindowSize() { return get()->pImpl->PVOCWindowSize; }
void Settings::setPVOCWindowSize( int newWindowSize ) {	get()->pImpl->PVOCWindowSize = newWindowSize; }

size_t Settings::PVOCHopSize() { return get()->pImpl->PVOCHopSize; }
void Settings::setPVOCHopSize( int newHopSize ) { get()->pImpl->PVOCHopSize = newHopSize; }

size_t Settings::PVOCFFTSize() { return get()->pImpl->PVOCFFTSize; }
void Settings::setPVOCFFTSize( int newFFTSize )	{ get()->pImpl->PVOCFFTSize = newFFTSize; }

size_t Settings::processTimeout() { return get()->pImpl->timeout; }
void Settings::setProcessTimeout( int newMs ) {	get()->pImpl->timeout = newMs; }

Settings * Settings::get()
	{
	static Settings singleton;
	return instanceExists? &singleton : nullptr;
	}
