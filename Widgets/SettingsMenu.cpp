#include "SettingsMenu.hpp"

#include <QLabel>
#include <QBoxLayout>
#include <QAction>

#include "Settings.hpp"
#include "MainWindow.hpp"
#include "CentralWidget.hpp"
#include "Widgets/NumberSlider.hpp"

static const QColor basicButtonColor = QColor::fromHsl( 0, 0, 40 );

void setButtonColor( QPushButton * b, QColor c )
	{
	 b->setStyleSheet(
		QString("QPushButton {"
					"background-color: #%1; "
					"border: 1px solid #DDDDDD;}"
				"QPushButton:hover {"
					"border: 2px solid white;}")
		.arg( c.rgba(), 0, 16) );
	};

SettingsMenu::SettingsMenu( QWidget * parent )
	: QWidget( parent )
	, header( new QLabel( "Settings" ) )
	, boxes( 4 )
	, colorButtons()
	{
	setLayout( new QVBoxLayout );
	setAttribute( Qt::WA_DeleteOnClose, false );
	layout()->setContentsMargins( 5,0,5,5 );
	setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
	setMinimumWidth( 400 );

	//Header setup
	header->setAlignment( Qt::AlignCenter );
	QFont headerFont( "Roboto Light", 18 );
	headerFont.setStyleStrategy( QFont::PreferAntialias );
	header->setFont( headerFont );
	header->setLayout( new QHBoxLayout );
	header->layout()->setAlignment( Qt::AlignRight );
	header->setStyleSheet( "background-color: rgba(0,0,0,0);" );
	header->setMinimumHeight( 45 );
	header->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout()->setMenuBar( header );

	auto * exitButton = new QPushButton;
	exitButton->setIconSize( QSize(25, 25) );
	QIcon exitIcon;
	exitIcon.addFile( ":Media/close-icon-w.png" );
	exitButton->setIcon( exitIcon );
	exitButton->setStyleSheet( "QPushButton { border: 0px; }" );
	QObject::connect( exitButton, &QPushButton::clicked,
		this, &SettingsMenu::done );
	QObject::connect( this, &SettingsMenu::done,
		Settings::get(), &Settings::saveToRegistry );
	header->layout()->addWidget( exitButton );

	//Box setup
	for( auto & i : boxes )
		{
		auto boxWidget = new QFrame;
		i = new QHBoxLayout;

		layout()->addWidget( boxWidget );
		boxWidget->setStyleSheet( "QWidget { "
						  "margin: 0px;"
						  "border-radius: 5px;"
						  "}" );

		boxWidget->setMinimumHeight( 60 );
		header->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );
		boxWidget->setLayout( i );
		i->setAlignment( Qt::AlignCenter );
		}
	QObject::connect( Settings::get(), &Settings::paletteUpdated, this, [this]( QPalette::ColorRole )
		{
		paintMenu( Settings::palette() );
		} );
	paintMenu( Settings::palette() );


	// PVOC Settings setup
	NumberSlider * PVOCWindowSlider = new NumberSlider( Settings::PVOCWindowSize(), 1, 16777216 );
	PVOCWindowSlider->setFilter( []( float x ){ return std::round( x ); } );
	QObject::connect( PVOCWindowSlider, &NumberSlider::stateChanged, this, [PVOCWindowSlider](){ Settings::setPVOCWindowSize( PVOCWindowSlider->getSliderPosition() ); } );
	addSlider( 1, "FFT Window Size", PVOCWindowSlider );

	NumberSlider * PVOCHopSlider = new NumberSlider( Settings::PVOCHopSize(), 1, 16777216 );
	PVOCHopSlider->setFilter( []( float x ){ return std::round( x ); } );
	QObject::connect( PVOCHopSlider, &NumberSlider::stateChanged, this, [PVOCHopSlider](){ Settings::setPVOCHopSize( PVOCHopSlider->getSliderPosition() ); } );
	addSlider( 1, "FFT Hop Size", PVOCHopSlider );

	NumberSlider * PVOCFFTSlider = new NumberSlider( Settings::PVOCFFTSize(), 1, 24 );
	PVOCFFTSlider->setFilter( []( float x ){ return std::round( x ); } );
	QObject::connect( PVOCFFTSlider, &NumberSlider::stateChanged, this, [PVOCFFTSlider](){ Settings::setPVOCFFTSize( PVOCFFTSlider->getSliderPosition() ); } );
	addSlider( 1, "FFT Transform Size", PVOCFFTSlider );

	NumberSlider * timeoutSlider = new NumberSlider( Settings::processTimeout(), 1, NumberSlider::infinity );
	timeoutSlider->setFilter( []( float x ){ return std::round( x ); } );
	QObject::connect( PVOCFFTSlider, &NumberSlider::stateChanged, this, [timeoutSlider](){ Settings::setProcessTimeout( timeoutSlider->getSliderPosition() ); } );
	addSlider( 1, "Process Timeout (ms)", timeoutSlider );


	// ColorButton setup
	colorButtons.push_back( new ColorButton( QPalette::Highlight ) );
	colorButtons.push_back( new ColorButton( QPalette::Light	 ) );
	colorButtons.push_back( new ColorButton( QPalette::Mid		 ) );
	colorButtons.push_back( new ColorButton( QPalette::Dark		 ) );
	colorButtons.push_back( new ColorButton( QPalette::Shadow	 ) );
	for( auto i : colorButtons )
		boxes[2]->addWidget( i );


	//Color reset button setup
	auto colorResetButton = new QPushButton( "Reset" );
	boxes[2]->addWidget( colorResetButton );
	setButtonColor( colorResetButton, basicButtonColor );
	colorResetButton->setFixedSize( 60, 30 );
	QObject::connect( colorResetButton, &QPushButton::clicked, [this]()
		{
		auto defaultPalette = Settings::defaultPalette();
		Settings::setPalette( defaultPalette );
		for( auto i : colorButtons )
			setButtonColor( i, defaultPalette.color( i->role ) );
		});
	}

void SettingsMenu::paintMenu( const QPalette & pal )
	{
	QString style;

	//style += QString(""
	//				 "{ background-color: #%1; }")
	//	.arg( palette.color( QPalette::Highlight ).rgba(), 0, 16 );

	//style += QString("QMenuBar"
	//				 "{ background-color: #%1; }")
	//	.arg( palette.color( QPalette::Mid ).rgba(), 0, 16 );

	style += QString("QDialog { "
					 "background-color: #%1;"
					 "border-radius: 5px;"
					 "border: 0px solid white;"
					 "}")
		.arg( pal.color( QPalette::Dark ).rgba(), 0, 16 );

	style += QString("QFrame { background-color: #%1; }")
		.arg( pal.color( QPalette::Shadow ).rgba(), 0, 16 );

	setStyleSheet( style );
	}

void SettingsMenu::addButton( int section, const QString & s, QAction * a )
	{
	auto b = new QPushButton( s );
	b->setFixedSize( 60, 30 );
	setButtonColor( b, basicButtonColor );
	QObject::connect( b, &QPushButton::clicked, a, &QAction::trigger );
	boxes[section]->addWidget( b );
	}

void SettingsMenu::addSlider( int section, const QString & label, NumberSlider * slider )
	{
	slider->setFixedWidth( 128 );
	QWidget * sliderBox = new QWidget;
	sliderBox->setLayout( new QVBoxLayout );
	auto l = new QLabel( label );
	l->setAlignment( Qt::AlignCenter );
	l->setStyleSheet( "background-color: rgba(0,0,0,0%)" );
	sliderBox->layout()->addWidget( l );
	sliderBox->layout()->addWidget( slider );
	boxes[section]->addWidget( sliderBox );
	}

//===================================================================================
// ColorButton

SettingsMenu::ColorButton::ColorButton( QPalette::ColorRole _role )
	: role( _role )
	{
	setButtonColor( this, Settings::palette().color( role ) );
	setFixedSize( 30, 30 );
	QObject::connect( this, &QPushButton::clicked, this, [this]()
		{
		selectColor();
		} );
	}

void SettingsMenu::ColorButton::selectColor()
	{
	const QPalette currentPal = Settings::palette();
	QPalette paletteCopy = currentPal;

	ReleaseNotifyColorDialog dialog( currentPal.color( role ) );
	dialog.setOptions( QColorDialog::DontUseNativeDialog );
	dialog.setWindowTitle( "Select Color" );

	QObject::connect( &dialog, &ReleaseNotifyColorDialog::repaintUI, this, [this, &dialog, &paletteCopy]()
		{
		const QColor color = dialog.currentColor();
		if( color.isValid() )
			{
			setButtonColor( this, color );
			paletteCopy.setColor( role, color );
			Settings::setPalette( paletteCopy );
			}
		});

	if( dialog.exec() == QDialog::Accepted )
		{
		Settings::setPalette( paletteCopy );
		}
	else
		{
		Settings::setPalette( currentPal );
		//setButtonColor( this, currentPal.color( role ) );
		}
	}


