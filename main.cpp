/*
Ideas:
	Player simple and advanced view for more advanced features
		Player zooming and moving
		Player moving line to Loton audio player view
		Player PVOCPlayer make play button work?
		Player time-scan feature in flanplayer, click to hear frame at that position
	PVOC::freeze
	Audio::lowPass
	Audio::chop
	subnodes
	most dynamic casts could be static
	allow all functions to auto-compose, removes all the duplicate function classes
	surface should be computing on another thread
	use Loton colors for audio/flan
	combine LuaFunc2x1/2x2Models under base class
	use fancy path finding in qmake

Todo:
	upload nodeeditor branch

Bugs:
Task:
	upload repo
*/



#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <nodes/NodeStyle>
#include <nodes/FlowViewStyle>
#include <nodes/ConnectionStyle>

#include "MainWindow.hpp"

static void setNodesStyle()
    {

    QtNodes::FlowViewStyle::setStyle( R"(
        {
        "FlowViewStyle":
            {
            "BackgroundColor": [25, 25, 25],
            "FineGridColor": [35, 35, 35],
            "CoarseGridColor": [108, 160, 83]
            }
        })");

    QtNodes::NodeStyle::setNodeStyle( R"(
        {
        "NodeStyle":
            {
            "NormalBoundaryColor": [255, 255, 255],
            "SelectedBoundaryColor": [160, 255, 160],
            "GradientColor0": [33, 36, 39],
            "GradientColor1": [33, 36, 39],
            "GradientColor2": [33, 36, 39],
            "GradientColor3": [33, 36, 39],
            "ShadowColor": [20, 20, 20],
            "FontColor" : "white",
            "FontColorFaded" : "gray",
            "ConnectionPointColor": "red",
            "FilledConnectionPointColor": "cyan",
            "ErrorColor": "red",
            "WarningColor": [128, 128, 0],

            "PenWidth": 1.0,
            "HoveredPenWidth": 1.5,

            "ConnectionPointDiameter": 8.0,

			"Opacity": 1.0
            }
        })");

    QtNodes::ConnectionStyle::setConnectionStyle( R"(
        {
        "ConnectionStyle":
            {
            "ConstructionColor": "gray",
            "NormalColor": "darkcyan",
            "SelectedColor": [100, 100, 100],
            "SelectedHaloColor": "orange",
            "HoveredColor": "lightcyan",

            "LineWidth": 3.0,
            "ConstructionLineWidth": 2.0,
            "PointDiameter": 10.0,

            "UseDataDefinedColors": true
            }
        })");
    }


#include <lua.hpp>
#include <lualib.h>
int main( int argc, char * argv[] )
	{	
	QApplication app( argc, argv );
	QCoreApplication::setOrganizationName( "Logan McBroom" );
	QCoreApplication::setApplicationName( "Loton" );

	QFile file( ":/dark.qss" );
	file.open( QFile::ReadOnly | QFile::Text );
	QTextStream stream( &file );
	app.setStyleSheet( stream.readAll() );

	setNodesStyle();

	MainWindow::instantiate();
	MainWindow * window = MainWindow::getInstance();
	window->show();

	return app.exec();
    }
