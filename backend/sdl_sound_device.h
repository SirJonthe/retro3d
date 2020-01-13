#ifndef SDL_SOUND_DEVICE_H
#define SDL_SOUND_DEVICE_H

#include <unordered_map>
#include "../frontend/retro_sound_device.h"

// Roll own mixer
// https://ericscrivner.me/2017/10/getting-circular-sdl-audio/

namespace platform
{

class SDLSoundDevice : public retro3d::SoundDevice
{
private:
	struct PlaybackJob
	{
		mtlShared<retro3d::Sound>  sound;
		void                      *api_data;
		retro3d::SharedTransform   world_transform;
		retro3d::AABB              world_aabb;
		int32_t                    channel;
		retro3d::Voice::Type       type;
		float                      volume;
		float                      radius;
	};

	typedef std::unordered_map<int32_t, PlaybackJob> PlaybackJobs;

private:
	PlaybackJobs             m_playback_jobs;
	retro3d::SharedTransform m_internal_transform_copy;
	retro3d::SharedTransform m_world_transform;
	retro3d::Sound::Format   m_format;
	int32_t                  m_audio_count;

private:
	void UpdateAABB(PlaybackJob &j);
	void UpdateVolume(PlaybackJob &j);

public:
	SDLSoundDevice( void );
	~SDLSoundDevice( void ) override;

	bool Init(const retro3d::Sound::Format &format) override;

	void SetReceiverTransform(const retro3d::SharedTransform &world_to_view) override;
	void SetReceiverTransform(const retro3d::Transform &world_to_view) override;

	retro3d::Transform     GetReceiverTransform( void ) const override;
	retro3d::Sound::Format GetFormat( void ) const override;

	int32_t Play(const retro3d::Voice &voice, const retro3d::SharedTransform &world_transform, double time_stamp = 0.0) override;
	int32_t Play(const retro3d::Voice &voice, const retro3d::Transform &world_transform, double time_stamp = 0.0) override;
	void    Kill(int32_t audio_index) override;

	void Update( void ) override;
};

}

#endif // SDL_SOUND_DEVICE_H
