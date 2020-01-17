#ifndef RETRO_SOUND_DEVICE_H
#define RETRO_SOUND_DEVICE_H

#include <cstdint>
#include "retro_device.h"
#include "../audio/retro_voice.h"
#include "../graphics/retro_camera.h"
#include "../common/retro_transform.h"

namespace retro3d
{

class SoundDevice : public retro3d::Device
{
public:
	virtual ~SoundDevice( void );

	virtual bool Init(const retro3d::Sound::Format &format) = 0;

	virtual void SetReceiverTransform(const retro3d::SharedTransform &world_transform) = 0;
	virtual void SetReceiverTransform(const retro3d::Transform &world_transform) = 0;

	virtual retro3d::Transform GetReceiverTransform( void ) const = 0;
	virtual retro3d::Sound::Format GetFormat( void ) const = 0;

	virtual int32_t Play(const retro3d::Voice &voice, const retro3d::SharedTransform &world_transform, double time_stamp = 0.0) = 0;
	virtual int32_t Play(const retro3d::Voice &voice, const retro3d::Transform &world_transform, double time_stamp = 0.0) = 0;
	virtual void    Kill(int32_t audio_index) = 0;

	virtual void Update( void ) = 0;
};

}

#endif // RETRO_SOUND_DEVICE_H
