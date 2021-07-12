#include "PVOCSourceDataModel.hpp"

#include <QLabel>
#include <QBoxLayout>
#include <QtConcurrent>
#include <QPushButton>
#include <QFileDialog>
#include <QFontMetrics>

#include "NodeDataTypes/PVOCData.hpp"
#include "Settings.hpp"

const size_t width = 128;

class HoverButton : public QPushButton
{
public:
	void enterEvent( QEvent * ) override { setText("📂"); }
	void leaveEvent( QEvent * ) override { setText("📁"); }
};

PVOCSourceDataModel::PVOCSourceDataModel()
	: PVOCProcessModel()
	, filepath( "" )
	, filename( new QLabel( "Select File" ) )
	{
	setValidationState( QtNodes::NodeValidationState::Valid, "" );

	auto selectorLayout = new QHBoxLayout;
	flanLayout->addLayout( selectorLayout );
	selectorLayout->setAlignment( Qt::AlignLeft );

	auto selectorButton = new HoverButton;
	selectorButton->setText( "📁" );
	selectorButton->setFixedSize( 25, 25 );
	selectorButton->setStyleSheet( "font-size: 18px;" );

	selectorLayout->addWidget( selectorButton );
	selectorLayout->addWidget( filename );

	filename->setMaximumWidth( width );

	QObject::connect( selectorButton, &QPushButton::clicked, [this]()
		{
		setFile( QFileDialog::getOpenFileName( mainWidget,
			"Select PVOC",
			Settings::samplesPath(),
			"PVOC Files (*.flan)" ) );
		});
	}

bool PVOCSourceDataModel::process()
	{
	qWarning() << "PVOCSourceDataModel::process was called, which is bad";
	return false;
	}

QString PVOCSourceDataModel::caption() const { return "Source"; }
QString PVOCSourceDataModel::name() const { return "PVOC Source"; }
QString PVOCSourceDataModel::portCaption( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		return "ERROR";
	else if( type == PortType::Out )
		switch( index )
			{
			case 0: return "Audio";
			}

	return "ERROR";
	}
unsigned int PVOCSourceDataModel::nPorts( PortType type ) const
	{
	switch( type )
		{
		case PortType::In:  return 0;
		case PortType::Out: return 1;
		default: return 0;
		}
	}
NodeDataType PVOCSourceDataModel::dataType( PortType type, PortIndex index ) const
	{
	if( type == PortType::In )
		return {"",""};
	else if( type == PortType::Out )
		switch( index )
			{
			case 0:  return PVOCData::Type();
			default: return {"",""};
			}
	else return {"",""};
	}

QJsonObject PVOCSourceDataModel::save() const
	{
	QJsonObject modelJson = NodeDataModel::save();
	modelJson["filepath"] = filepath;
	return modelJson;
	}

void PVOCSourceDataModel::restore( QJsonObject const & p )
	{
	QJsonValue filepath_in = p["filepath"];
	if( ! filepath_in.isUndefined() )
		{
		filepath = filepath_in.toString();
		setFile( filepath );
		}
	}

void PVOCSourceDataModel::setFile( const QString & newFilepath )
	{
	if( newFilepath == "" ) return;

	if( QFileInfo( newFilepath ).exists() )
		{
		filepath = newFilepath;
		filename->setText( QFontMetrics( QFont() ).elidedText( QFileInfo( filepath ).fileName(), Qt::ElideRight, width ) );
		const QString filepath_capturable = filepath;
		setFunctor( [filepath_capturable]()
			{
			return std::shared_ptr<NodeData>( new PVOCData(
				flan::PVOC( filepath_capturable.toStdString() ) ) );
			});
		}
	else
		{
		qDebug() << "Bad filepath in PVOCSourceDataModel";
		filepath = "";
		filename->setText( "Select File" );
		}
	}

