#pragma once

#include "AudioProcessModel.hpp"

class QLabel;
class QPushButton;

class AudioSourceDataModel : public AudioProcessModel
{
public:
    AudioSourceDataModel();

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


