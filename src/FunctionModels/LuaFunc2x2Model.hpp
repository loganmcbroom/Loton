#pragma once

#include "FunctionModels/FunctionNodeModel.hpp"

class Point2x2GraphModel;
class FunctionGeneratorPlot2x2Model;
class TextEditorModel;
class OnOffButtonModel;

class LuaFunc2x2Model : public Function2x2NodeModel
{
public:
	LuaFunc2x2Model();

	QString caption() const override { return "Lua 2->2"; }
	QString name() const override { return "Lua 2->2"; }
	std::shared_ptr<Func2x2Data> function() override;
	QString description() const override
		{
		return R"(This returns a user-created Lua function, defined in the node view.

Input - Number:
	If provided, the node will output a constant function equal to itself evaluated at (Input, 0).
		)";
		}

private:

	QWidget * makeHeaderWidget() override;
	ControllerPairs makeInputControllers() override;
	QJsonObject save() const override;
	void restore( QJsonObject const & p ) override;

	FunctionGeneratorPlot2x2Model * plotModel;
	std::shared_ptr<TextEditorModel> editor;
	std::shared_ptr<OnOffButtonModel> logModeButtonModel;
};
