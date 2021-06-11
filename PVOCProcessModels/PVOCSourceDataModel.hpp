#pragma once

#include "PVOCProcessModels/PVOCProcessModel.hpp"

class PVOCSourceDataModel : public PVOCProcessModel
{
public:
	PVOCSourceDataModel();

	bool process() override;

	QString caption() const override;
	QString name() const override;
	QString portCaption(PortType, PortIndex) const override;
	unsigned int nPorts( PortType type ) const override;
	NodeDataType dataType( PortType type, PortIndex index ) const override;
	QJsonObject save() const override;
	void restore( QJsonObject const & p ) override;

	void setFile( const QString & filepath );

	QString filepath;
	QLabel * filename; // managed by parent
};
