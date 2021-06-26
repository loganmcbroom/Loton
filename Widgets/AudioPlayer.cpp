#include "AudioPlayer.hpp"

#include <QBoxLayout>
#include <QPushButton>
#include <QAudioOutput>
#include <QBuffer>
#include <QResizeEvent>
#include <QFileDialog>

#include "flan/Audio.h"

#include "NodeDataTypes/AudioData.hpp"
#include "Widgets/WaveformIcon.hpp"
#include "Settings.hpp"


// Model =====================================================================

AudioPlayerModel::AudioPlayerModel()
	: output( )
	, buffer( new QBuffer( this ) )
	, array( new QByteArray )
	, waveformModel( new WaveformIconModel )
	, audio()
	{}

bool AudioPlayerModel::playing() const
	{
	return output && output->state() == QAudio::ActiveState;
	}

void AudioPlayerModel::setAudio( std::shared_ptr<AudioData> newAudio )
	{
	audio = newAudio;
	waveformModel->setAudio( audio );
	}

void AudioPlayerModel::play()
	{
	if( !audio ) return;
	if( playing() ) stop();

	const auto & in = audio->audio;

	//Format setup and validation
	QAudioFormat format;
	format.setSampleRate( (int) in.getSampleRate() );
	format.setChannelCount( (int) in.getNumChannels() );
	format.setSampleSize( sizeof( float ) * 8 );
	format.setCodec( "audio/pcm" );
	format.setByteOrder( QAudioFormat::LittleEndian );
	format.setSampleType( QAudioFormat::Float );

	QAudioDeviceInfo info( QAudioDeviceInfo::defaultOutputDevice() );
	if( !info.isFormatSupported( format ) )
		{
		qWarning() << "Raw audio format not supported by backend, cannot play audio.";
		return;
		}

	//Reading audio into owned buffer
	array->resize( int ( in.getNumChannels() * in.getNumFrames() * sizeof( float ) ) );
	float * arrayBuffer = (float * ) array->data();
	for( size_t channel = 0; channel < in.getNumChannels(); ++channel )
		for( size_t sample = 0; sample < in.getNumFrames(); ++sample )
			arrayBuffer[ sample * in.getNumChannels() + channel ] = ( float ) in.getSample( channel, sample );

	buffer->setData( *array );
	buffer->open( QIODevice::ReadOnly );

	output.reset( new QAudioOutput( format ) );
	output->setNotifyInterval( 1000.0f / 60.0f );
//	QObject::connect( output.get(), &QAudioOutput::notify,
//		this, &AudioPlayerModel::handleNotify );
	QObject::connect( output.get(), &QAudioOutput::stateChanged,
		this, &AudioPlayerModel::handlePlayerStateChange );
	output->start( buffer );
	emit startedPlaying();
	}

void AudioPlayerModel::stop()
	{
	if( output )
		output->stop();
	buffer->close();
	emit stoppedPlaying();
	}

void AudioPlayerModel::handlePlayerStateChange( QAudio::State newState )
	{
	switch (newState)
		{
		case QAudio::IdleState: // Finished playing (no more data)
			stop();
			break;

		case QAudio::StoppedState:
			// Stopped for other reasons
			if( output && output->error() != QAudio::NoError )
				qWarning() << output->error();
			break;

		default:
			break;
		}
	}



// View ======================================================================

AudioPlayerView::AudioPlayerView( AudioPlayerModel * _model )
	: model( _model )
	, waveform( new WaveformIconView( model->waveformModel.get() ) )
	, overlay()
	, playButton( new QPushButton( "►" ) )
	, saveButton( new QPushButton( "💾" ) )
	{
	setLayout( new QHBoxLayout() );
	layout()->setContentsMargins( 0, 0, 0, 0 );

	//Play button setup
	layout()->addWidget( playButton );
	playButton->setFixedSize( 25, 25 );
	playButton->setStyleSheet( "font-size: 18px;" );
	QObject::connect( playButton, &QPushButton::clicked, this, [this]()
		{
		if( model->playing() )
			model->stop();
		else
			model->play();
		});
	QObject::connect( model, &AudioPlayerModel::startedPlaying,
					  this, [this](){ playButton->setText( "⬛" ); } );
	QObject::connect( model, &AudioPlayerModel::stoppedPlaying,
					  this, [this](){ playButton->setText( "►" ); } );

	//Save button setup
	layout()->addWidget( saveButton );
	saveButton->setFixedSize( { 25, 25 } );
	saveButton->setStyleSheet( "font-size: 18px;" );
	QObject::connect( saveButton, &QPushButton::clicked, this, [this]()
		{
		if( model && model->audio )
			{
			QString filename = QFileDialog::getSaveFileName( this, "Select Save Location",
				Settings::samplesPath(), "Audio (*.wav)" );

			if( ! filename.isEmpty() )
				model->audio->audio.save( filename.toStdString() );
			}
		} );

	//Waveform setup
	layout()->addWidget( waveform );
	//QObject::connect( waveform, &WaveformIconView::dataUpdated,
	//	this, [this](){ update(); } );
	}

//void AudioPlayerView::paintEvent( QPaintEvent * e )
//	{
//	QWidget::paintEvent( e );
//	QPainter p( this );
//	//QRect playButtonRect = { 0, 0, playButton->width(), playButton->height() };
//	//p.drawImage( playButton->width() + 5, 0, waveform->getIcon() );
//	}

//void AudioPlayerView::resizeEvent( QResizeEvent * event )
//	{
//	QWidget::resizeEvent( event );
//	}



