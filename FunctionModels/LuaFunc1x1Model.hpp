#pragma once

#include "FunctionModels/FunctionNodeModel.hpp"

class FunctionGeneratorPlotModel;
class TextEditorModel;

class LuaFunc1x1Model : public Function1x1NodeModel
{
public:
	LuaFunc1x1Model();

	QString caption() const override { return "Lua 1->1"; }
	QString name() const override { return "Lua 1->1"; }
	std::shared_ptr<Func1x1Data> function() override;
	QString description() const override
		{
		return R"(This returns a user-created Lua function, defined in the node view.

Input - Number:
	If provided, the node will output a constant function equal to itself evaluated at the Input.
		)";
		}
private:

	QWidget * makeHeaderWidget() override;
	ControllerPairs makeInputControllers() override;
	QJsonObject save() const override;
	void restore( QJsonObject const & p ) override;

	FunctionGeneratorPlotModel * plotModel;
	std::shared_ptr<TextEditorModel> editor;
};
