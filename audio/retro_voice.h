#ifndef RETRO_VOICE_H
#define RETRO_VOICE_H

#include "../common/MiniLib/MML/mmlVector.h"
#include "retro_sound.h"
#include "../common/retro_geom.h"

namespace retro3d
{

class Voice
{
public:
	enum Type
	{
		Ambient,     // equally audible within range AABB
		Directional, // ambient sound coming from certain direction, equally audible within range AABB
		Point,       // originating from point, falloff based on receiver position within range AABB (fallof approaches 0 at end of radius)
		Cone         // originating from point, falloff based on receiver position within range AABB and sound direction
	};

private:
	mtlShared<retro3d::Sound> m_sound;
	float                     m_range_radius;
	float                     m_volume;
	float                     m_pitch;
	float                     m_playback_speed;
	Type                      m_type;
	// NOTE: origin and direction is determined separately by supplied transform matrix

public:
	Voice( void );

	mtlShared<retro3d::Sound> GetSound( void ) const;
	float GetRangeRadius( void ) const;
	float GetVolume( void ) const;
	float GetPitch( void ) const;
	float GetPlaybackSpeed( void ) const;
	Type  GetType( void ) const;

	void SetSound(const mtlShared<retro3d::Sound> &sound);
	void SetRangeRadius(float radius);
	void SetVolume(float volume);
	void SetPitch(float pitch);
	void SetPlaybackSpeed(float playback_speed);
	void SetType(retro3d::Voice::Type type);
};

};

#endif // RETRO_VOICE_H
