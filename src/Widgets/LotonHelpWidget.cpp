#include "LotonHelpWidget.hpp"

#include <QBoxLayout>

#include "Settings.hpp"
#include "LotonNodeModel.hpp"

LotonHelpWidget::LotonHelpWidget()
	: QTextEdit()
	{
	setViewportMargins( 20, 20, 20, 20 );

	Settings::setDynamicColor( this, "QScrollArea { background-color: #%1; }", QPalette::Dark );

	QFont f;
	f.setPointSize( 10 );
	f.setStyleStrategy( QFont::PreferAntialias );
	setFont( f );
	}

void LotonHelpWidget::setActiveNode( LotonNodeModel * w )
	{
	setText( w->name() + "\n\n" + w->description() );
	}
