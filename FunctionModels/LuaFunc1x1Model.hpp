#pragma once

#include "FunctionModels/FunctionNodeModel.hpp"

class FunctionGeneratorPlotModel;
class TextEditorModel;

class LuaFunc1x1Model : public FunctionNodeModel
{
public:
	LuaFunc1x1Model();

	QString caption() const override { return "Lua 1->1"; }
	QString name() const override { return "Lua 1->1"; }
	std::shared_ptr<Func1x1Data> function() override;
private:

	QWidget * makeHeaderWidget() override;
	ControllerPairs makeInputControllers() override;
	QJsonObject save() const override;
	void restore( QJsonObject const & p ) override;

	FunctionGeneratorPlotModel * plotModel;
	std::shared_ptr<TextEditorModel> editor;
};
