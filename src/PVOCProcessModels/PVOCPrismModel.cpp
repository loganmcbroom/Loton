#include "PVOCPrismModel.hpp"

#include "Widgets/OnOffButton.hpp"
#include "Widgets/TextEditor.hpp"
#include "NodeDataTypes/PVOCData.hpp"
#include "NodeDataTypes/FunctionData.hpp"
#include "NodeDataTypes/InterpolatorData.hpp"
#include "FunctionModels/LuaHandlers/LuaPrismHandler.hpp"
#include "Settings.hpp"


PVOCPrismModel::PVOCPrismModel()
	: PVOCProcessModel()
	, functionText( new TextEditorModel( "" ) )
	, perNoteModel( new OnOffButtonModel( false ) )
	{
	QObject::connect( functionText.get(), &TextEditorModel::stateChanged, this, &PVOCPrismModel::updateData );
	QObject::connect( perNoteModel.get(), &OnOffButtonModel::stateChanged, this, &PVOCPrismModel::updateData );
	}

bool PVOCPrismModel::process()
	{
	if( ! ins[0] ) return false;

	auto in = std::dynamic_pointer_cast<PVOCData>( ins[0] )->pvoc;

	const bool perNote_c = perNoteModel->getButtonPosition();
	setFunctor( [in, lua = LuaPrismHandler( functionText->doc->toPlainText() ), perNote_c, useGPU = Settings::useOpenCL(), c = canceller]()
		{
		return std::shared_ptr<NodeData>( new PVOCData( in.prism( lua, perNote_c, useGPU, *c ) ) );
		});

	return true;
	}

QString PVOCPrismModel::caption() const { return "Prism"; }
QString PVOCPrismModel::name() const { return "PVOC Prism"; }
QString PVOCPrismModel::portCaption( PortType type, PortIndex index ) const
	{
	return "PVOC";
	}

unsigned int PVOCPrismModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 1;
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType PVOCPrismModel::dataType( PortType type, PortIndex index ) const
	{
	return PVOCData::Type();
	}

ControllerPairs PVOCPrismModel::makeInputControllers()
	{
	auto textView = new TextEditorView( functionText.get() );
	textView->setFixedHeight( 300 );
	textView->setPlaceholderText( R"(This function has access to:
			The note index n
			The time t
			The fundamental frequency, f
			The harmonic index, h (starting at one)
			The number of harmonics, hn
			All harmonic magnitudes in a vector, hs

The function should return the magnitude and frequency that the current harmonic should be written to. For example, the identity function would be:
			return hs[h], f * h
		)" );
	return {
		{ QString(), textView },
		{ "Per Note", new OnOffButtonView( perNoteModel.get() ) }
		};
	}

QJsonObject PVOCPrismModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["function"] = functionText->save();
	modelJson["perNote"] = perNoteModel->save();
	return modelJson;
	}

void PVOCPrismModel::restore( QJsonObject const & p )
	{
	functionText->restore( p["function"].toObject() );
	perNoteModel->restore( p["perNote"].toObject() );
	}


