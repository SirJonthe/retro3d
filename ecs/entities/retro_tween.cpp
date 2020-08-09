#include "retro_tween.h"

retro3d::ITween::ITween( void ) : mtlInherit(this), m_mode(MODE_REL_LERP), m_duration(1_s), m_timer_type(retro3d::TIMER_GAME)
{}

void retro3d::ITween::SetProperties(retro3d::ITween::Mode mode, retro3d::Time duration, retro3d::TimerType timer_type)
{
	m_mode = mode;
	m_duration = duration;
	m_timer_type = timer_type;
}

void retro3d::ITween::Cancel( void )
{
	Destroy();
}
