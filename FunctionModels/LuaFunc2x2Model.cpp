#include "LuaFunc2x2Model.hpp"

#include <QLayout>

#include "Widgets/FunctionGeneratorPlot2x2.hpp"
#include "Widgets/TextEditor.hpp"
#include "Widgets/OnOffButton.hpp"
#include "NodeDataTypes/FunctionData.hpp"
#include "LuaHandlers/LuaFunction.hpp"

LuaFunc2x2Model::LuaFunc2x2Model()
	: plotModel( new FunctionGeneratorPlot2x2Model( QString(), this ) )
	, editor( new TextEditorModel( "return x,y" ) )
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

	auto plotView = new FunctionGeneratorPlot2x2View( plotModel );
	mainLayout->addWidget( plotView );
	plotView->setFixedSize( 120, 120 );
	plotView->setStyleSheet( "QwtPlot { border: 1px solid white; }" );

	QObject::connect( this, &LuaFunc2x2Model::dataUpdatedDynamic, plotModel, [this]() { plotModel->setOut( out ); } );

	QObject::connect( editor.get(), &TextEditorModel::stateChanged, this, [this]()
		{
		updateOutput();
		emit dataUpdated( 0 );
		} );

	updateOutput();
	emit dataUpdated( 0 );
	}

std::shared_ptr<Func2x2Data> LuaFunc2x2Model::function()
	{
	return std::make_shared<Func2x2Data>( LuaFunc2x2( editor->doc->toPlainText() ) );
	}

QWidget * LuaFunc2x2Model::makeHeaderWidget()
	{
	auto plotView = new FunctionGeneratorPlot2x2ViewAdv( plotModel );
	plotView->setFixedHeight( 400 );
	return plotView;
	}

ControllerPairs LuaFunc2x2Model::makeInputControllers()
	{
	auto textView = new TextEditorView( editor.get() );
	textView->setPlaceholderText( "return t(x,y), f(x,y)" );
	textView->setFixedHeight( 200 );
	return {
		{ QString(), textView },
		{ "Logarithmic y-axis", new OnOffButtonView( logModeButtonModel.get() ) }
		};
	}

QJsonObject LuaFunc2x2Model::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["editor"] = editor->save();
	modelJson["logMode"] = logModeButtonModel->save();
	return modelJson;
	}

void LuaFunc2x2Model::restore( QJsonObject const & p )
	{
	editor->restore( p["editor"].toObject() );
	logModeButtonModel->restore( p["logMode"].toObject() );
	}
