#include "retro_voice.h"

retro3d::Voice::Voice( void ) : m_sound(), m_range_radius(50.0f), m_volume(1.0f), m_pitch(1.0f), m_playback_speed(1.0f), m_type(retro3d::Voice::Point)
{}

mtlShared<retro3d::Sound> retro3d::Voice::GetSound( void ) const
{
	return m_sound;
}

float retro3d::Voice::GetRangeRadius( void ) const
{
	return m_range_radius;
}

float retro3d::Voice::GetVolume( void ) const
{
	return m_volume;
}

float retro3d::Voice::GetPitch( void ) const
{
	return m_pitch;
}

float retro3d::Voice::GetPlaybackSpeed( void ) const
{
	return m_playback_speed;
}

retro3d::Voice::Type retro3d::Voice::GetType( void ) const
{
	return m_type;
}

void retro3d::Voice::SetSound(const mtlShared<retro3d::Sound> &sound)
{
	m_sound = sound;
}

void retro3d::Voice::SetRangeRadius(float radius)
{
	m_range_radius = mmlAbs(radius);
}

void retro3d::Voice::SetVolume(float volume)
{
	m_volume = mmlClamp(0.0f, volume, 1.0f);
}

void retro3d::Voice::SetPitch(float pitch)
{
	m_pitch = pitch;
}

void retro3d::Voice::SetPlaybackSpeed(float playback_speed)
{
	m_playback_speed = playback_speed;
}

void retro3d::Voice::SetType(retro3d::Voice::Type type)
{
	m_type = type;
}
