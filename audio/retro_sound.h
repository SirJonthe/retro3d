#ifndef RETRO_SOUND_H
#define RETRO_SOUND_H

#include <climits>
#include "../common/retro_assets.h"
#include "../common/retro_containers.h"

namespace retro3d
{

//class SoundSection {}; // class for representing a timed section of a sound (think mtlChars and mtlString)

class Sound : public retro3d::Asset< retro3d::Sound >
{
public:
	enum SampleFormat
	{
		SampleFormat_UINT8 = sizeof(uint8_t) * CHAR_BIT,
		SampleFormat_INT16 = sizeof(int16_t) * CHAR_BIT
	};

	enum Channels
	{
		Channels_Mono   = 1,
		Channels_Stereo = 2
	}; // Number of output channels

	struct Format
	{
		Channels     num_channels;
		uint32_t     sample_rate; // samples per second
		SampleFormat sample_format;
	};

	union Sample
	{
		enum Channels
		{
			Left,
			Right,
			ChannelCount
		};
		float channels[ChannelCount]; // -1.0 - 1.0
		struct {
			float left, right;
		} channel;
	};

	struct Debug_Sample
	{
		int16_t left;
		int16_t right;
	};

	union SampleDataPointer
	{
		uint8_t *u8;
		int16_t *i16;
	};

private:
	retro3d::Array< uint8_t > m_samples; // uncompressed PCM samples
	SampleDataPointer         m_sample_ptr;
	Format                    m_format;

private:
	Sample   DecodeU8(uint32_t sample_index) const;
	Sample   DecodeI16(uint32_t sample_index) const;
	uint8_t  EncodeU8(float channel) const;
	int16_t  EncodeI16(float channel) const;

public:
	Sound( void );
	Sound(uint32_t num_samples, const Sound::Format &format);
	Sound(double duration, const Sound::Format &format);
	explicit Sound(uint32_t num_samples);
	explicit Sound(double duration);
	Sound(const Sound &s);
	Sound &operator=(const Sound &s);

	SampleDataPointer GetSampleData( void ) const;
	uint32_t          GetSampleCount( void ) const;
	uint32_t          GetRawByteCount( void ) const;
	Sample            GetSampleFromIndex(uint32_t sample_index) const;
	Sample            GetSampleFromTime(double timestamp) const;
	uint32_t          GetChannelCount( void ) const;
	uint32_t          GetSampleRate( void ) const;
	uint32_t          GetBitRate( void ) const;
	uint32_t          GetBitsPerSample( void ) const;
	double            GetDuration( void ) const;
	uint32_t          GetBytesPerChannel( void ) const;
	uint32_t          GetBytesPerSample( void ) const; // Same as GetBlockAlign
	uint32_t          GetByteRate( void ) const;

	uint32_t GetBitsPerSecond( void ) const; // Same as GetBitRate
	uint32_t GetBytesPerSecond( void ) const; // Same as GetByteRate
	uint32_t GetSamplesPerSecond( void ) const; // Same as GetSampleRate

	void CreateDuration(double duration, const Sound::Format &format);
	void CreateSamples(uint32_t num_samples, const Sound::Format &format);
	void CreateDuration(double duration);
	void CreateSamples(uint32_t num_samples);
	void Destroy( void );
	void SetSample(uint32_t sample_index, const Sound::Sample &sample);

	void Copy(const Sound &s);

	uint32_t GetSampleIndex(double timestamp) const;
	double   GetTimeStamp(uint32_t sample_index) const;

//	void Mix(const Sound &sample);
	void Scale(double duration_scalar); // BUG: Does not work.
	void MirrorChannels( void );
//	void Render(float from_time, float to_time, float l_vol, float r_vol, retro3d::Sound &dst); // renders a stretched time interval with the given parameters to dst (in the format of dst)

	static void Convert(const Sound &src, Sound &dst, const Sound::Format &target_format);
	void Convert(const Sound::Format &target_format);

	Debug_Sample Debug_GetSample(uint32_t sample_index) const;
};

}

#endif // RETRO_SOUND_H
