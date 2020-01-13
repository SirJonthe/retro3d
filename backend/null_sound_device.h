#ifndef RETRO_NULL_SOUND_H
#define RETRO_NULL_SOUND_H

#include "../frontend/retro_sound_device.h"

namespace platform
{

class NullSoundDevice : public retro3d::SoundDevice
{
public:
	bool Init(const retro3d::Sound::Format&) override { return true; }

	void SetReceiverTransform(const retro3d::SharedTransform&) override {}
	void SetReceiverTransform(const retro3d::Transform&) override {}

	retro3d::Transform GetReceiverTransform( void ) const override { return mmlMatrix<4,4>::Identity(); }
	retro3d::Sound::Format GetFormat( void ) const override { return retro3d::Sound::Format{ retro3d::Sound::Channels_Mono, 22050, retro3d::Sound::SampleFormat_UINT8 }; }

	int32_t Play(const retro3d::Voice &, const retro3d::SharedTransform &, double) override { return -1; }
	int32_t Play(const retro3d::Voice &, const retro3d::Transform &, double) override { return -1; }
	void    Kill(int32_t) override {}

	void Update( void ) override {}
};

}

#endif // RETRO_NULL_SOUND_H
