#pragma once

#include <QFutureWatcher>

#include <nodes/NodeData>
#include <nodes/Connection>

class AudioData;
class PVOCData;
namespace std { template<typename T> class complex; };
namespace flan{ class FFTHelper; };
template<class T> class QFuture;

//=========================================================================
// Conversions
// The Conversion classes must be declared in the header to invoke the MOC
// These represent a single conversion used by a converter
class FFTConversion : public QObject
{ Q_OBJECT
public:
	FFTConversion( size_t windowSize, std::shared_ptr<std::atomic<bool>> canceller );
	~FFTConversion();

	void cancel() { if( canceller ) canceller->store( false ); }

signals:
	void finished( std::shared_ptr<QtNodes::NodeData> );

protected:
	std::shared_ptr<flan::FFTHelper> fft;

	std::unique_ptr<QFuture<std::shared_ptr<QtNodes::NodeData>>> out;
	QFutureWatcher<std::shared_ptr<QtNodes::NodeData>> outWatcher;

	std::shared_ptr<std::atomic<bool>> canceller;
};

class AudioToPVOCConversion : public FFTConversion
{ Q_OBJECT
public:
	AudioToPVOCConversion( std::shared_ptr<AudioData> audio,
		size_t windowSize, size_t hopSize, size_t fftSize, std::shared_ptr<std::atomic<bool>> );
};

class PVOCToAudioConversion : public FFTConversion
{ Q_OBJECT
public:
	PVOCToAudioConversion( std::shared_ptr<PVOCData> flan,
		std::shared_ptr<std::atomic<bool>> );
};




//=========================================================================
// Converters

struct SoundConverter : public QtNodes::TypeConverter
{
	~SoundConverter() override;
	void operator()( std::shared_ptr<QtNodes::NodeData> data ) override final;
	// 0 for default
	virtual void convertWithCanceller( std::shared_ptr<QtNodes::NodeData>, size_t windowSize, size_t hopSize, size_t fftSize,
		std::shared_ptr<std::atomic<bool>> ) = 0;
	FFTConversion * conversion = nullptr;
};

struct AudioToPVOCConverter : public SoundConverter
{
	virtual std::shared_ptr<QtNodes::TypeConverter> createNew() override
		{ return std::make_shared<AudioToPVOCConverter>(); }
	void convertWithCanceller( std::shared_ptr<QtNodes::NodeData>, size_t windowSize = 0, size_t hopSize = 0, size_t fftSize = 0,
		std::shared_ptr<std::atomic<bool>> = nullptr ) override;
};

struct PVOCToAudioConverter : public SoundConverter
{
	virtual std::shared_ptr<QtNodes::TypeConverter> createNew() override
		{ return std::make_shared<PVOCToAudioConverter>(); }
	void convertWithCanceller( std::shared_ptr<QtNodes::NodeData>, size_t windowSize = 0, size_t hopSize = 0, size_t fftSize = 0,
		std::shared_ptr<std::atomic<bool>> = nullptr ) override;
};




