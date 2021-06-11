#include "LuaFunc2x1Model.hpp"

#include <QLayout>

#include "Widgets/FunctionGeneratorPlot2x1.hpp"
#include "Widgets/TextEditor.hpp"
#include "Widgets/OnOffButton.hpp"
#include "NodeDataTypes/FunctionData.hpp"
#include "LuaHandlers/LuaFunction.hpp"

LuaFunc2x1Model::LuaFunc2x1Model()
	: plotModel( new FunctionGeneratorPlot2x1Model( QString(), this ) )
	, editor( new TextEditorModel( "return x + y" ) )
	, logModeButtonModel( new OnOffButtonModel( true ) )
	{
	plotModel->setLogMode( true );
	QObject::connect( logModeButtonModel.get(), &OnOffButtonModel::stateChanged, this, [this]()
		{
		plotModel->setLogMode( logModeButtonModel->getButtonPosition() );
		} );

	auto mainLayout = new QHBoxLayout( mainWidget );
	mainWidget->setLayout( mainLayout );
	mainLayout->setContentsMargins( 0,3,0,0 );
	mainLayout->setSpacing( 0 );

	auto plotView = new FunctionGeneratorPlot2x1View( plotModel, 3 );
	mainLayout->addWidget( plotView );
	plotView->setFixedSize( 120, 120 );
	plotView->setStyleSheet( "QwtPlot { border: 1px solid white; }" );

	QObject::connect( this, &LuaFunc2x1Model::dataUpdatedDynamic, plotModel, [this]() { plotModel->setOut( out ); } );

	QObject::connect( editor.get(), &TextEditorModel::stateChanged, this, [this]()
		{
		updateOutput();
		emit dataUpdated( 0 );
		} );

	updateOutput();
	emit dataUpdated( 0 );
	}

std::shared_ptr<Func2x1Data> LuaFunc2x1Model::function()
	{
	return std::make_shared<Func2x1Data>( LuaFunc2x1( editor->doc->toPlainText() ) );
	}

QWidget * LuaFunc2x1Model::makeHeaderWidget()
	{
	auto plotView = new FunctionGeneratorPlot2x1ViewAdv( plotModel );
	plotView->setFixedHeight( 400 );
	return plotView;
	}

ControllerPairs LuaFunc2x1Model::makeInputControllers()
	{
	auto textView = new TextEditorView( editor.get() );
	textView->setPlaceholderText( "return f(x,y)" );
	textView->setFixedHeight( 200 );
	return {
		{ QString(), textView },
		{ "Logarithmic y-axis", new OnOffButtonView( logModeButtonModel.get() ) }
		};
	}

QJsonObject LuaFunc2x1Model::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["editor"] = editor->save();
	modelJson["logMode"] = logModeButtonModel->save();
	return modelJson;
	}

void LuaFunc2x1Model::restore( QJsonObject const & p )
	{
	editor->restore( p["editor"].toObject() );
	logModeButtonModel->restore( p["logMode"].toObject() );
	}
