#include "PVOCPlayer.hpp"

#include <QBoxLayout>
#include <QPushButton>
#include <QAudioOutput>
#include <QBuffer>
#include <QFileDialog>

#include <flan/PVOC.h>

#include "NodeDataTypes/PVOCData.hpp"
#include "Widgets/SpectrographIcon.hpp"
#include "Settings.hpp"

// Model =====================================================================

PVOCPlayerModel::PVOCPlayerModel()
	: spectroModel( new SpectrographIconModel )
	, flan()
	{}

//bool PVOCPlayerModel::playing() const
//	{
//	return output && output->state() == QAudio::ActiveState;
//	}

void PVOCPlayerModel::setPVOC( std::shared_ptr<PVOCData> newPVOC )
	{
	flan = newPVOC;
	spectroModel->setPVOC( flan );
	}

//void PVOCPlayerModel::play()
//	{
//	if( !audio ) return;
//	if( playing() ) stop();

//	const auto & in = audio->audio;

//	//Format setup and validation
//	QAudioFormat format;
//	format.setSampleRate( (int) in.getSampleRate() );
//	format.setChannelCount( (int) in.getNumChannels() );
//	format.setSampleSize( sizeof( float ) * 8 );
//	format.setCodec( "audio/pcm" );
//	format.setByteOrder( QAudioFormat::LittleEndian );
//	format.setSampleType( QAudioFormat::Float );

//	QAudioDeviceInfo info( QAudioDeviceInfo::defaultOutputDevice() );
//	if( !info.isFormatSupported( format ) )
//		{
//		qWarning() << "Raw audio format not supported by backend, cannot play audio.";
//		return;
//		}

//	//Reading audio into owned buffer
//	array->resize( int ( in.getNumChannels() * in.getNumFrames() * sizeof( float ) ) );
//	float * arrayBuffer = (float * ) array->data();
//	for( size_t channel = 0; channel < in.getNumChannels(); ++channel )
//		for( size_t sample = 0; sample < in.getNumFrames(); ++sample )
//			arrayBuffer[ sample * in.getNumChannels() + channel ] = ( float ) in.getSample( channel, sample );

//	buffer->setData( *array );
//	buffer->open( QIODevice::ReadOnly );

//	output.reset( new QAudioOutput( format ) );
//	QObject::connect( output.get(), &QAudioOutput::stateChanged, this, &PVOCPlayerModel::handlePlayerStateChange );
//	output->start( buffer );
//	emit startedPlaying();
//	}

//void PVOCPlayerModel::stop()
//	{
//	output->stop();
//	buffer->close();
//	emit stoppedPlaying();
//	}

//void PVOCPlayerModel::handlePlayerStateChange( QAudio::State newState )
//	{
//	switch (newState)
//		{
//		case QAudio::IdleState: // Finished playing (no more data)
//			stop();
//			break;

//		case QAudio::StoppedState:
//			// Stopped for other reasons
//			if( output && output->error() != QAudio::NoError )
//				qWarning() << output->error();
//			break;

//		default:
//			break;
//		}
//	}



// View ======================================================================

PVOCPlayerView::PVOCPlayerView( PVOCPlayerModel * _model )
	: model( _model )
	//, playButton( new QPushButton( "►" ) )
	, saveButton( new QPushButton( "💾" ) )
	{
	auto layout = new QHBoxLayout;
	setLayout( layout );
	layout->setMargin( 0 );

	//Play button setup
	//layout->addWidget( playButton );
	//playButton->setFixedSize( { 25, 25 } );
	//playButton->setStyleSheet( "font-size: 18px;" );
//	QObject::connect( playButton, &QPushButton::clicked, this, [this]()
//		{
//		if( model->playing() )
//			model->stop();
//		else
//			model->play();
//		});
//	QObject::connect( model, &PVOCPlayerModel::startedPlaying,
//					  this, [this](){ playButton->setText( "⬛" ); } );
//	QObject::connect( model, &PVOCPlayerModel::stoppedPlaying,
//					  this, [this](){ playButton->setText( "►" ); } );

	//Save button setup
	layout->addWidget( saveButton );
	saveButton->setFixedSize( { 25, 25 } );
	saveButton->setStyleSheet( "font-size: 18px;" );
	QObject::connect( saveButton, &QPushButton::clicked, this, [this]()
		{
		if( model && model->flan )
			{
			QString filename = QFileDialog::getSaveFileName( this, "Select Save Location",
				Settings::samplesPath(), "PVOC (*.flan)" );

			if( ! filename.isEmpty() )
				model->flan->flan.save( filename.toStdString() );
			}
		} );

	layout->addWidget( new SpectrographIconView( model->spectroModel.get() ) );
	}

