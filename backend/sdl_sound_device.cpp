#ifdef RETRO3D_USE_SDL1
	#include <SDL/SDL_mixer.h>
	#include <SDL/SDL.h>
#elif defined(RETRO3D_USE_SDL2)
	#include <SDL2/SDL_mixer.h>
	#include <SDL2/SDL.h>
#endif
#include "sdl_sound_device.h"

void platform::SDLSoundDevice::UpdateAABB(platform::SDLSoundDevice::PlaybackJob &j)
{
	const double r = double(j.radius);
	j.world_aabb = retro3d::AABB(mmlTranslation(j.world_transform->GetFinalMatrix()), mmlVector<3>(r, r, r));
}

void platform::SDLSoundDevice::UpdateVolume(platform::SDLSoundDevice::PlaybackJob &j)
{
	float vol = j.volume;
	switch (j.type) {
	case retro3d::Voice::Ambient:
		vol = j.volume;
		break;
	case retro3d::Voice::Point:
		{
			const mmlVector<3> sfx_vec = retro3d::VectorFromAToB(mmlTranslation(j.world_transform->GetFinalMatrix()), mmlTranslation(m_world_transform->GetFinalMatrix()));
			const float d = sfx_vec.Len();
			vol = mmlIsApproxEqual(d, 0.0f) == false ? mmlMin(1.0f, j.volume / d) : 1.0f;
			// set panning
//			const mmlVector<3> right_normal = m_world_transform->GetRight();
//			const mmlVector<3> left_normal = -right_normal;
//			std::cout << right_normal[0] << "," << right_normal[1] << "," << right_normal[2] << std::endl;
//			std::cout << left_normal[0] << "," << left_normal[1] << "," << left_normal[2] << std::endl;
//			uint32_t left_channel = mmlClamp(Uint8(127), Uint8(mmlDot(sfx_vec, left_normal) * 255.0f), Uint8(255));
//			uint32_t right_channel = mmlClamp(Uint8(127), Uint8(mmlDot(sfx_vec, right_normal) * 255.0f), Uint8(255));
//			std::cout << j.channel << "(" << left_channel << "," << right_channel << ")" << std::endl;
//			Mix_SetPanning(j.channel, mmlClamp(Uint8(127), Uint8(mmlDot(sfx_vec, left_normal) * 255.0f), Uint8(255)), mmlClamp(Uint8(127), Uint8(mmlDot(sfx_vec, right_normal) * 255.0f), Uint8(255)));
			// There seems to be an issue with Mix_SetPanning with default drivers (program halts, fucks up audio for other processes)
		}
		break;
	default: break;
	}
	Mix_VolumeChunk(reinterpret_cast<Mix_Chunk*>(j.api_data), int(vol * MIX_MAX_VOLUME));
}

platform::SDLSoundDevice::SDLSoundDevice( void ) :
	m_internal_transform_copy(retro3d::SharedTransform::Create()),
	m_world_transform(m_internal_transform_copy),
	m_format{ retro3d::Sound::Channels_Mono, 0, retro3d::Sound::SampleFormat_UINT8 },
	m_audio_count(0)
{}

platform::SDLSoundDevice::~SDLSoundDevice( void )
{
	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
	m_format.sample_rate = 0;
}

bool platform::SDLSoundDevice::Init(const retro3d::Sound::Format &format)
{
	if (m_format.sample_rate != 0) { return false; }

	int freq = 0, chan = 0;
	Uint16 fmt = 0;

	bool result =
		SDL_InitSubSystem(SDL_INIT_AUDIO) == 0 &&
//		Mix_Init(MIX_INIT_OGG|MIX_INIT_MOD|MIX_INIT_MP3|MIX_INIT_FLAC) && // only needed when support for OGG, MOD, MP3, and FLAC is needed. WAV supported by default.
		Mix_OpenAudio(int(format.sample_rate), format.sample_format == retro3d::Sound::SampleFormat_INT16 ? AUDIO_S16LSB : AUDIO_U8, format.num_channels, 4096) == 0 &&
		Mix_QuerySpec(&freq, &fmt, &chan) != 0 &&
		(fmt == AUDIO_S16LSB || fmt == AUDIO_U8) &&
		(chan == 1 || chan == 2);

	if (result == true) {
		m_format.sample_rate = uint32_t(freq);
		m_format.sample_format = fmt == AUDIO_S16LSB ? retro3d::Sound::SampleFormat_INT16 : retro3d::Sound::SampleFormat_UINT8;
		m_format.num_channels = chan == 1 ? retro3d::Sound::Channels_Mono : retro3d::Sound::Channels_Stereo;
	} else {
		m_format.sample_rate = 0;
		m_format.sample_format = retro3d::Sound::SampleFormat_UINT8;
		m_format.num_channels = retro3d::Sound::Channels_Mono;
		Mix_CloseAudio();
		Mix_Quit();
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
	}

	return result;
}

void platform::SDLSoundDevice::SetReceiverTransform(const retro3d::SharedTransform &world_transform)
{
	if (world_transform.IsNull() == false) {
		m_world_transform = world_transform;
	} else {
		*m_internal_transform_copy.GetShared() = mmlMatrix<4,4>::Identity();
		m_world_transform = m_internal_transform_copy;
	}
}

void platform::SDLSoundDevice::SetReceiverTransform(const retro3d::Transform &world_transform)
{
	*m_internal_transform_copy.GetShared() = world_transform;
	m_world_transform = m_internal_transform_copy;
}

retro3d::Transform platform::SDLSoundDevice::GetReceiverTransform( void ) const
{
	return *m_world_transform.GetShared();
}

retro3d::Sound::Format platform::SDLSoundDevice::GetFormat( void ) const
{
	return m_format;
}

int32_t platform::SDLSoundDevice::Play(const retro3d::Voice &voice, const retro3d::SharedTransform &world_transform, double time_stamp)
{
	if (m_format.sample_rate == 0) { return -1; } // the device has failed to initialize

	PlaybackJob j;
	j.sound = voice.GetSound();
	retro3d::Sound *sfx = j.sound.GetShared();
	if (sfx != nullptr && sfx->GetSampleCount() > 0) {

		if (m_format.sample_rate != sfx->GetSampleRate() || m_format.num_channels != sfx->GetChannelCount() || uint32_t(m_format.sample_format) != sfx->GetBitsPerSample()) {
			return -1;
		}

		uint32_t start_sample_index = sfx->GetSampleIndex(time_stamp);
		if (start_sample_index > sfx->GetSampleCount()) { return -1; }
		start_sample_index = start_sample_index * sfx->GetBytesPerSample();

		j.world_transform = world_transform;
		j.radius = voice.GetRangeRadius();
		UpdateAABB(j);
		j.type = voice.GetType();
		j.volume = voice.GetVolume();
		j.api_data = new Mix_Chunk;
		Mix_Chunk *chunk = reinterpret_cast<Mix_Chunk*>(j.api_data);
		chunk->allocated = 0;
		chunk->abuf = sfx->GetSampleData().u8 + start_sample_index;
		chunk->alen = sfx->GetRawByteCount() - start_sample_index;
		UpdateVolume(j);
		j.channel = Mix_PlayChannel(-1, chunk, 0);
		int32_t index = m_audio_count++;

		if (j.channel != -1) {
			m_playback_jobs[index] = j;
		}

		return j.channel != -1 ? index : -1;
	}

	return -1;
}

int32_t platform::SDLSoundDevice::Play(const retro3d::Voice &voice, const retro3d::Transform &world_transform, double time_stamp)
{
	retro3d::SharedTransform shared;
	shared.New();
	*shared.GetShared() = world_transform;
	return Play(voice, shared, time_stamp);
}

void platform::SDLSoundDevice::Kill(int32_t audio_index)
{
	if (audio_index < 0) { return; }

	Mix_FreeChunk(reinterpret_cast<Mix_Chunk*>(m_playback_jobs[audio_index].api_data));
	m_playback_jobs.erase(audio_index);
}

void platform::SDLSoundDevice::Update( void )
{
	for (PlaybackJobs::iterator i = m_playback_jobs.begin(); i != m_playback_jobs.end(); ++i) {
		UpdateAABB(i->second);
		UpdateVolume(i->second);
	}
}
