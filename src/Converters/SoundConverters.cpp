#include "SoundConverters.hpp"

#include <QtConcurrent>

//#include <fftw3.h>

#include <flan/FFTHelper.h>

#include "Settings.hpp"
#include "NodeDataTypes/AudioData.hpp"
#include "NodeDataTypes/PVOCData.hpp"



//=============================================================================================================================
// Converters
//=============================================================================================================================

#ifdef _DEBUG
	static int conversionTracker = 0;
	struct ConversionReporter
	{
	~ConversionReporter()
		{
		if( conversionTracker != 0 )
			qDebug() << "Sound conversion count error: " << conversionTracker;
		}
	};
	static ConversionReporter reporter;
#endif

using namespace QtNodes;

//static const size_t windowSize = 2048;
//static const size_t overlaps = 16;

SoundConverter::~SoundConverter()
	{
	if( conversion )
		conversion->cancel();
	}

void SoundConverter::operator()( std::shared_ptr<NodeData> data )
    {
	convertWithCanceller( data, 0, 0, 0, std::make_shared<std::atomic<bool>>( false ) );
    }

void AudioToPVOCConverter::convertWithCanceller( std::shared_ptr<NodeData> data, size_t windowSize, size_t hopSize, size_t fftSize,
	std::shared_ptr<std::atomic<bool>> c )
	{
	auto audioData = std::dynamic_pointer_cast<AudioData>( data );

	// The current conversion is no longer needed.
	// Cancel it and let it self-delete when finished
	if( conversion )
		{
		conversion->cancel();
		conversion = nullptr;
		}

	if( audioData )
		{
		const size_t windowSize_c = windowSize == 0 ? Settings::PVOCWindowSize() : windowSize;
		const size_t hopSize_c = hopSize == 0 ? Settings::PVOCHopSize() : hopSize;
		size_t fftSize_c = pow( 2, fftSize == 0 ? Settings::PVOCFFTSize() : fftSize );

		if( fftSize_c < windowSize_c )
			fftSize_c = std::pow( 2, std::ceil( std::log2( windowSize_c ) ) );

		Q_ASSERT( fftSize_c >= windowSize_c && fftSize_c <= 16777216 );

		//Handler will self delete when computation completes
		conversion = new AudioToPVOCConversion( audioData, windowSize_c, hopSize_c, fftSize_c, c );

		//When the conversion finishes, we also finish
		QObject::connect( conversion, &FFTConversion::finished,
			this, &AudioToPVOCConverter::finished );

		//If a conversion dies while it is the monitored conversion, set to nullptr
		QObject::connect( conversion, &FFTConversion::destroyed,
			this, [this]( QObject * destroyed )
			{
			if( destroyed == conversion )
				conversion = nullptr;
			} );
		}
	}

void PVOCToAudioConverter::convertWithCanceller( std::shared_ptr<NodeData> data, size_t, size_t, size_t,
	std::shared_ptr<std::atomic<bool>> c )
	{
	auto pvocData = std::dynamic_pointer_cast<PVOCData>( data );

	// The current conversion is no longer needed.
	// Cancel it and let it self-delete when finished
	if( conversion )
		{
		conversion->cancel();
		conversion = nullptr;
		}

	if( pvocData )
		{
		//Handler will self delete when computation completes
		conversion = new PVOCToAudioConversion( pvocData, c );

		//When the conversion finishes, we also finish
		QObject::connect( conversion, &FFTConversion::finished,
			this, &PVOCToAudioConverter::finished );

		//If a conversion dies while it is the monitored conversion, set to nullptr
		QObject::connect( conversion, &FFTConversion::destroyed,
			this, [this]( QObject * destroyed )
			{
			if( destroyed == conversion )
				conversion = nullptr;
			} );
		}
	}




//=============================================================================================================================
// Convertions 47
//=============================================================================================================================

FFTConversion::FFTConversion( size_t windowSize, std::shared_ptr<std::atomic<bool>> c )
	: fft( new flan::FFTHelper( windowSize, true, true, false ) )
	, out( nullptr )
	, outWatcher()
	, canceller( c ? c : std::make_shared<std::atomic<bool>>( false ) )
	{
#ifdef _DEBUG
	++conversionTracker;
#endif
	QObject::connect( &outWatcher, &QFutureWatcher<std::shared_ptr<NodeData>>::finished, this, [this]()
		{
		emit finished( out->result() );
		delete this;
		} );
	}

FFTConversion::~FFTConversion()
	{
#ifdef _DEBUG
	--conversionTracker;
#endif
	}

AudioToPVOCConversion::AudioToPVOCConversion( std::shared_ptr<AudioData> audioData,
	size_t windowSize, size_t hopSize, size_t fftSize, std::shared_ptr<std::atomic<bool>> c )
	: FFTConversion( fftSize, c )
	{
	out.reset( new QFuture<std::shared_ptr<NodeData>>( QtConcurrent::run(
		[this, audioData, windowSize, hopSize, fftSize, useGPU = Settings::useOpenCL(), c]()
			{
			return std::shared_ptr<NodeData>( new PVOCData( useGPU?
				audioData->audio.convertToPVOC( windowSize, hopSize, fftSize, fft, *c ) :
				audioData->audio.convertToPVOC_cpu( windowSize, hopSize, fftSize, fft, *c )
				) );
			} ) ) );
	outWatcher.setFuture( *out );
	}

PVOCToAudioConversion::PVOCToAudioConversion( std::shared_ptr<PVOCData> pvocData, std::shared_ptr<std::atomic<bool>> c )
	: FFTConversion( pvocData->pvoc.getDFTSize(), c )
	{
	out.reset( new QFuture<std::shared_ptr<NodeData>>( QtConcurrent::run(
		[this, pvocData, useGPU = Settings::useOpenCL(), c]()
			{
			return std::shared_ptr<NodeData>( new AudioData( useGPU ?
				pvocData->pvoc.convertToAudio( fft, *c ) :
				pvocData->pvoc.convertToAudio_cpu( fft, *c )
				) );
			} ) ) );
	outWatcher.setFuture( *out );
	}

