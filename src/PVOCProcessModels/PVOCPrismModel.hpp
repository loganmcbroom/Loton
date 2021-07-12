#pragma once

#include "PVOCProcessModel.hpp"

class TextEditorModel;
class OnOffButtonModel;

class PVOCPrismModel : public PVOCProcessModel
{
public:
	PVOCPrismModel();

	bool process() override;

	QString caption() const override;
	QString name() const override;
	QString portCaption(PortType, PortIndex) const override;
	unsigned int nPorts( PortType type ) const override;
	NodeDataType dataType( PortType type, PortIndex index ) const override;
	std::vector<PortIndex> portsRequired() const override { return { 0 }; }
	ControllerPairs makeInputControllers() override;
	QJsonObject save() const override;
	void restore( QJsonObject const & p ) override;
	QString description() const override
		{
		return R"(Prism, in theory, allows complete control over the frequency and magnitude of every harmonic of every note in the input.
Individual notes are extracted from the input, and each harmonic of each note is processed by the suppied function.
This function is too complex to have a data type, so it is built into the node view. The function has access to:
	The note index n
	The time t
	The fundamental frequency, f
	The harmonic index, h (starting at one)
	The number of harmonics, hn
	All harmonic magnitudes in a vector, hs

The function should return the magnitude and frequency that the current harmonic should be written to.
For example, the identity function would be:
	return hs[h], f * h
		)";
		}

	std::unique_ptr<TextEditorModel> functionText;
	std::unique_ptr<OnOffButtonModel> perNoteModel;
};
