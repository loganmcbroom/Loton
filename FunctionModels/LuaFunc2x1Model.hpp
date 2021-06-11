#pragma once

#include "FunctionModels/FunctionNodeModel.hpp"

class Point2x1GraphModel;
class FunctionGeneratorPlot2x1Model;
class TextEditorModel;
class OnOffButtonModel;

class LuaFunc2x1Model : public Function2x1NodeModel
{
public:
	LuaFunc2x1Model();

	QString caption() const override { return "Lua 2->1"; }
	QString name() const override { return "Lua 2->1"; }
	std::shared_ptr<Func2x1Data> function() override;
private:

	QWidget * makeHeaderWidget() override;
	ControllerPairs makeInputControllers() override;
	QJsonObject save() const override;
	void restore( QJsonObject const & p ) override;

	FunctionGeneratorPlot2x1Model * plotModel;
	std::shared_ptr<TextEditorModel> editor;
	std::shared_ptr<OnOffButtonModel> logModeButtonModel;
};
