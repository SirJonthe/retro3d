#include "retro_sound.h"
#include "../common/retro_assert.h"
#include "../common/MiniLib/MML/mmlMath.h"
#include "../common/MiniLib/MML/mmlFixed.h"

retro3d::Sound::Sample retro3d::Sound::DecodeU8(uint32_t sample_index) const
{
	constexpr float i8min = float(std::numeric_limits<int8_t>::min());
	constexpr float i8max = float(std::numeric_limits<int8_t>::max());
	constexpr float i8max_inv = 1.0f / i8max;

	sample_index = sample_index * m_format.num_channels;

	Sample s = {{
		(float(m_sample_ptr.u8[sample_index]) + i8min) * i8max_inv,
		(float(m_sample_ptr.u8[sample_index + (uint32_t(m_format.num_channels) >> 1)]) + i8min) * i8max_inv
	}};

	return s;
}

retro3d::Sound::Sample retro3d::Sound::DecodeI16(uint32_t sample_index) const
{
	constexpr float i16min_inv = 1.0f / std::numeric_limits<int16_t>::min();
	constexpr float i16max_inv = 1.0f / std::numeric_limits<int16_t>::max();

	sample_index = sample_index * m_format.num_channels;

	const int16_t *l = reinterpret_cast<const int16_t*>(m_sample_ptr.i16 + sample_index);
	const int16_t *r = reinterpret_cast<const int16_t*>(m_sample_ptr.i16 + sample_index + (uint32_t(m_format.num_channels) >> 1));

	Sample s = {{
		float( *l < 0 ? (mmlAbs(int32_t(*l)) * i16min_inv) : (*l * i16max_inv) ),
		float( *r < 0 ? (mmlAbs(int32_t(*r)) * i16min_inv) : (*r * i16max_inv) )
	}};

	return s;
}

uint8_t retro3d::Sound::EncodeU8(float channel) const
{
	constexpr float u8max_div2 = std::numeric_limits<uint8_t>::max() * 0.5f;
	uint8_t u8 = uint8_t((channel + 1.0f) * u8max_div2);
	return u8;
}

int16_t retro3d::Sound::EncodeI16(float channel) const
{
	int16_t i16 = int16_t( channel < 0.0f ?
		mmlAbs(channel) * std::numeric_limits<int16_t>::min() :
		channel * std::numeric_limits<int16_t>::max()
	);
	return i16;
}

retro3d::Sound::Sound( void ) : m_samples(), m_sample_ptr{ nullptr }, m_format{ Channels_Mono, 0, SampleFormat_UINT8 }
{
	m_samples.poolMemory = true;
}

retro3d::Sound::Sound(uint32_t num_sample, const Sound::Format &format) : Sound()
{
	CreateSamples(num_sample, format);
}

retro3d::Sound::Sound(double duration, const Sound::Format &format) : Sound()
{
	CreateDuration(duration, format);
}

retro3d::Sound::Sound(uint32_t num_sample) : Sound()
{
	CreateSamples(num_sample);
}

retro3d::Sound::Sound(double duration) : Sound()
{
	CreateDuration(duration);
}

retro3d::Sound::Sound(const retro3d::Sound &s) : Sound()
{
	Copy(s);
}

retro3d::Sound &retro3d::Sound::operator=(const retro3d::Sound &s)
{
	Copy(s);
	return *this;
}

retro3d::Sound::SampleDataPointer retro3d::Sound::GetSampleData( void ) const
{
	return m_sample_ptr;
}

uint32_t retro3d::Sound::GetSampleCount( void ) const
{
	return uint32_t(m_samples.GetSize()) / uint32_t(m_format.num_channels) / GetBytesPerChannel();
}

uint32_t retro3d::Sound::GetRawByteCount( void ) const
{
	return uint32_t(m_samples.GetSize());
}

retro3d::Sound::Sample retro3d::Sound::GetSampleFromIndex(uint32_t sample_index) const
{
	RETRO3D_ASSERT(sample_index < GetSampleCount());

	Sample s;

	switch (m_format.sample_format) {
	case SampleFormat_UINT8:
		s = DecodeU8(sample_index);
		break;
	case SampleFormat_INT16:
		s = DecodeI16(sample_index);
		break;
	default: break;
	}

	return s;
}

retro3d::Sound::Sample retro3d::Sound::GetSampleFromTime(double timestamp) const
{
	const uint32_t sample_index = GetSampleIndex(timestamp);
	return GetSampleFromIndex(sample_index);
}

uint32_t retro3d::Sound::GetChannelCount( void ) const
{
	return m_format.num_channels;
}

uint32_t retro3d::Sound::GetSampleRate( void ) const
{
	return m_format.sample_rate;
}

uint32_t retro3d::Sound::GetBitRate( void ) const
{
	return m_format.sample_rate * m_format.sample_format * m_format.num_channels;
}

uint32_t retro3d::Sound::GetBitsPerSample( void ) const
{
	return m_format.sample_format;
}

double retro3d::Sound::GetDuration( void ) const
{
	return GetTimeStamp(GetSampleCount());
}

uint32_t retro3d::Sound::GetBytesPerChannel( void ) const
{
	return m_format.sample_format == SampleFormat_UINT8 ? sizeof(uint8_t) : sizeof(int16_t);
}

uint32_t retro3d::Sound::GetBytesPerSample( void ) const
{
	return GetBytesPerChannel() * m_format.num_channels;
}

uint32_t retro3d::Sound::GetByteRate( void ) const
{
	return m_format.sample_rate * GetBytesPerSample();
}

uint32_t retro3d::Sound::GetBitsPerSecond( void ) const
{
	return GetBitRate();
}

uint32_t retro3d::Sound::GetBytesPerSecond( void ) const
{
	return GetByteRate();
}

uint32_t retro3d::Sound::GetSamplesPerSecond( void ) const
{
	return GetSampleRate();
}

void retro3d::Sound::CreateDuration(double duration, const retro3d::Sound::Format &format)
{
	const uint32_t num_samples = uint32_t(std::ceil(double(duration) * format.sample_rate));
	CreateSamples(num_samples, format);
}

void retro3d::Sound::CreateSamples(uint32_t num_samples, const retro3d::Sound::Format &format)
{
	m_format = format;
	m_samples.Create(int(num_samples * GetBytesPerSample()));
	m_sample_ptr.u8 = num_samples > 0 ? &(m_samples[0]) : nullptr;
}

void retro3d::Sound::CreateDuration(double duration)
{
	CreateDuration(duration, m_format);
}

void retro3d::Sound::CreateSamples(uint32_t num_samples)
{
	CreateSamples(num_samples, m_format);
}

void retro3d::Sound::Destroy( void )
{
	m_samples.Free();
	m_sample_ptr.u8 = nullptr;
}

void retro3d::Sound::SetSample(uint32_t sample_index, const retro3d::Sound::Sample &sample)
{
	RETRO3D_ASSERT(sample_index < GetSampleCount());

	sample_index = sample_index * m_format.num_channels;

	switch (m_format.sample_format) {
	case SampleFormat_UINT8:
		switch (m_format.num_channels) {
		case Channels_Mono:
			{
				const float L = mmlAbs(sample.channel.left);
				const float R = mmlAbs(sample.channel.right);
				const float TOT = L + R;
				if (mmlIsApproxEqual(TOT, 0.0f) == false) {
					const float L_RATIO = L / TOT;
					const float R_RATIO = R / TOT;
					m_sample_ptr.u8[sample_index] = EncodeU8(sample.channel.left * L_RATIO + sample.channel.right * R_RATIO);
				} else {
					m_sample_ptr.u8[sample_index] = 0;
				}
			}
			break;
		case Channels_Stereo:
			m_sample_ptr.u8[sample_index]     = EncodeU8(sample.channel.left);
			m_sample_ptr.u8[sample_index + 1] = EncodeU8(sample.channel.right);
			break;
		default: break;
		}
		break;
	case SampleFormat_INT16:
		switch (m_format.num_channels) {
		case Channels_Mono:
			{
				const float L = mmlAbs(sample.channel.left);
				const float R = mmlAbs(sample.channel.right);
				const float TOT = L + R;
				if (mmlIsApproxEqual(TOT, 0.0f) == false) {
					const float L_RATIO = L / TOT;
					const float R_RATIO = R / TOT;
					m_sample_ptr.i16[sample_index] = EncodeI16(sample.channel.left * L_RATIO + sample.channel.right * R_RATIO);
				} else {
					m_sample_ptr.i16[sample_index] = 0;
				}
			}
			break;
		case Channels_Stereo:
			m_sample_ptr.i16[sample_index]     = EncodeI16(sample.channel.left);
			m_sample_ptr.i16[sample_index + 1] = EncodeI16(sample.channel.right);
			break;
		default: break;
		}
		break;
	default: break;
	}
}

void retro3d::Sound::Copy(const retro3d::Sound &s)
{
	if (this == &s) { return; }

	m_samples = s.m_samples;
	if (m_samples.GetSize() > 0) {
		m_format        = s.m_format;
		m_sample_ptr.u8 = &(m_samples[0]);
	} else {
		Destroy();
	}
}

uint32_t retro3d::Sound::GetSampleIndex(double timestamp) const
{
	const uint32_t sample_index = uint32_t(timestamp * m_format.sample_rate) % GetSampleCount();
	return sample_index;
}

double retro3d::Sound::GetTimeStamp(uint32_t sample_index) const
{
	// sample count and sample rate will be very large numbers so we need to use integer arithmetic
	double sec = double(sample_index / GetSampleRate());
	double msec = (sample_index % GetSampleRate()) / double(GetSampleRate());
	return sec + msec;
}

/*void retro3d::Sound::Mix(const retro3d::Sound &sample)
{
	const Sound *sample_a = this;
	const Sound *sample_b = &sample;
	if (sample_a->GetDuration() < sample_b->GetDuration()) {
		mmlSwap(sample_a, sample_b);
	}

	const uint64_t i_inc = (uint64_t(GetSampleRate()) << 16) / sample.GetSampleRate();
	for () {
		Sample a = GetSample(i);
		Sample b = GetSample(i);
		Sample out;
		for (uint32_t c = 0; c < Sample::ChannelCount; ++c) {

			int32_t a32 = int32_t(a.channels[c]);
			int32_t b32 = int32_t(b.channels[c]);
			constexpr int32_t i16min = std::numeric_limits<int16_t>::min();
			constexpr int32_t i16max = std::numeric_limits<int16_t>::max();
			int32_t mix = mmlClamp(i16min, a32 + b32, i16max);

			out.channels[c] = int16_t(mix);
		}
	}
}*/

void retro3d::Sound::Scale(double duration_scalar)
{
	const uint32_t src_sample_count = GetSampleCount();
	const uint32_t dst_sample_count = uint32_t(src_sample_count * duration_scalar);
	Sound sfx(dst_sample_count, m_format);

	const mml::fixed<mml::uint64,32> src_scaled_sample_count = mml::fixed<mml::uint64,32>(src_sample_count);
	const mml::fixed<mml::uint64,32> src_inc = src_scaled_sample_count / dst_sample_count;

	mml::fixed<mml::uint64,32> src_index = 0;
	for (uint32_t dst_index = 0; dst_index < dst_sample_count; ++dst_index) {
		Sample src = GetSampleFromIndex(uint32_t(mml::uint64(src_index)));
		sfx.SetSample(dst_index, src);
		src_index += src_inc;
	}

	*this = sfx;
}

void retro3d::Sound::MirrorChannels( void )
{
	if (GetChannelCount() <= 1) { return; }
	for (uint32_t i = 0; i < GetSampleCount(); ++i) {
		Sample s = GetSampleFromIndex(i);
		const float L = mmlAbs(s.channel.left);
		const float R = mmlAbs(s.channel.right);
		const float TOT = L + R;
		if (mmlIsApproxEqual(TOT, 0.0f) == false) {
			const float L_RATIO = L / TOT;
			const float R_RATIO = R / TOT;
			const float c = s.channel.left * L_RATIO + s.channel.right * R_RATIO;
			s.channel.left = s.channel.right = c;
			SetSample(i, s);
		}
	}
}

void retro3d::Sound::Convert(const retro3d::Sound &src, retro3d::Sound &dst, const retro3d::Sound::Format &target_format)
{
	if (src.m_format.sample_rate == target_format.sample_rate && src.m_format.num_channels == target_format.num_channels && src.m_format.sample_format == target_format.sample_format) {
		dst = src;
		return;
	}

	const retro3d::Sound *src_ptr = &src;
	if (&dst == &src) {
		src_ptr = new retro3d::Sound(src); // copy this since we will be overwriting it
	}

	dst.CreateSamples(src_ptr->GetSampleCount(), target_format);
	const uint32_t dst_sample_count = dst.GetSampleCount();
	for (uint32_t i = 0; i < dst_sample_count; ++i) {
		retro3d::Sound::Sample s = src_ptr->GetSampleFromIndex(i);
		dst.SetSample(i, s);
	}

	if (&dst == &src) {
		delete src_ptr;
	}
}

void retro3d::Sound::Convert(const retro3d::Sound::Format &target_format)
{
	Convert(*this, *this, target_format);
}

retro3d::Sound::Debug_Sample retro3d::Sound::Debug_GetSample(uint32_t sample_index) const
{
	sample_index = sample_index * m_format.num_channels;
	Debug_Sample s;
	switch (m_format.sample_format) {
	case SampleFormat_UINT8:
		switch (m_format.num_channels) {
		case Channels_Mono:
			s.left = s.right = m_sample_ptr.u8[sample_index];
			break;
		case Channels_Stereo:
			s.left = m_sample_ptr.u8[sample_index];
			s.right = m_sample_ptr.u8[sample_index + 1];
			break;
		}
		break;
	case SampleFormat_INT16:
		switch (m_format.num_channels) {
		case Channels_Mono:
			s.left = s.right = m_sample_ptr.i16[sample_index];
			break;
		case Channels_Stereo:
			s.left = m_sample_ptr.i16[sample_index];
			s.right = m_sample_ptr.i16[sample_index + 1];
			break;
		}
		break;
	}
	return s;
}
