#include "LuaFunc1x1Model.hpp"

#include <QLayout>

#include "Widgets/FunctionGeneratorPlot.hpp"
#include "Widgets/TextEditor.hpp"
#include "NodeDataTypes/FunctionData.hpp"
#include "LuaHandlers/LuaFunction.hpp"

LuaFunc1x1Model::LuaFunc1x1Model()
	: plotModel( new FunctionGeneratorPlotModel( "Lua 1->1", this ) )
	, editor( new TextEditorModel( "return x" ) )
	{
	auto mainLayout = new QHBoxLayout( mainWidget );
	mainWidget->setLayout( mainLayout );
	mainLayout->setContentsMargins( 0,3,0,0 );
	mainLayout->setSpacing( 0 );

	auto plotView = new FunctionGeneratorPlotView( plotModel, 2 );
	mainLayout->addWidget( plotView );
	plotView->setFixedSize( 120, 120 );
	plotView->setStyleSheet( "QwtPlot { border: 1px solid white; }" );

	QObject::connect( this, &LuaFunc1x1Model::dataUpdatedDynamic, plotModel, [this]() { plotModel->setOut( out ); } );

	QObject::connect( editor.get(), &TextEditorModel::stateChanged, this, [this]()
		{
		updateOutput();
		emit dataUpdated( 0 );
		} );

	updateOutput();
	emit dataUpdated( 0 );
	}

std::shared_ptr<Func1x1Data> LuaFunc1x1Model::function()
	{
	return std::make_shared<Func1x1Data>( LuaFunc1x1( editor->doc->toPlainText() ) );
	}

QWidget * LuaFunc1x1Model::makeHeaderWidget()
	{
	auto plotView = new FunctionGeneratorPlotViewAdv( plotModel, 2 );
	plotView->setFixedHeight( 300 );
	return plotView;
	}

ControllerPairs LuaFunc1x1Model::makeInputControllers()
	{
	auto textView = new TextEditorView( editor.get() );
	textView->setPlaceholderText( "return f(x)" );
	textView->setFixedHeight( 200 );
	return { { QString(), textView } };
	}

QJsonObject LuaFunc1x1Model::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["editor"] = editor->save();
	return modelJson;
	}

void LuaFunc1x1Model::restore( QJsonObject const & p )
	{
	editor->restore( p["editor"].toObject() );
	}
