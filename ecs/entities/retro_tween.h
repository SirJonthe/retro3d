#ifndef RETRO_TWEEN_H
#define RETRO_TWEEN_H

#include "../retro_entity.h"
#include "../../common/retro_defs.h"

namespace retro3d
{

// A generic interface for an interpolation process (tween).
class ITween : public mtlInherit< retro3d::Entity, ITween >
{
public:
	// The mode in which to tween the object.
	enum Mode
	{
		MODE_REL_LERP,  // Relative linear interpolation. Relative does not lock target into place if other processes are manipulating the object, but may miss the destination.
		MODE_REL_SLERP, // Relative spherical interpolation. Relative does not lock target into place if other processes are manipulating the object, but may miss the destination.
		MODE_REL_QLERP, // Relative quaternion interpolation. Relative does not lock target into place if other processes are manipulating the object, but may miss the destination.
		MODE_ABS_LERP,  // Absolute linear interpolation. Absolute locks target into place regardless if other processes are manipulating the object, but will hit destination.
		MODE_ABS_SLERP, // Absolute spherical interpolation. Absolute locks target into place regardless if other processes are manipulating the object, but will hit destination.
		MODE_ABS_QLERP  // Absolute quaternion interpolation. Absolute locks target into place regardless if other processes are manipulating the object, but will hit destination.
	};

protected:
	Mode                m_mode;
	retro3d::Time       m_duration;
	retro3d::TimerType  m_timer_type;

public:
	// Construction
	ITween( void );

	// Sets the mode in which to tween.
	void SetProperties(Mode mode, retro3d::Time duration, retro3d::TimerType timer_type = retro3d::TIMER_GAME);

	// Forces the tween to finish
	virtual void Finish( void ) = 0;

	// Stops the tweening process cold.
	void Cancel( void );
};

template < typename type_t >
class Tween : public mtlInherit< ITween, Tween<type_t> >
{
private:
	type_t *x;
	type_t  x0;
	type_t  a;
	type_t  b;

protected:
	// Tweens the target and calls user-defined OnUpdate.
	void OnUpdate( void ) override;

public:
	// Construction.
	Tween( void ) = delete;
	Tween(type_t &from, const type_t &to, ITween::Mode mode, retro3d::Time duration, retro3d::TimerType timer_type = retro3d::TIMER_GAME);

	// Forces the tween to finish.
	void Finish( void ) override;
};

}

template < typename type_t >
void retro3d::Tween<type_t>::OnUpdate( void )
{
	const float  d = mmlClamp(0.0f, float((Entity::LifeTime(ITween::m_timer_type) / ITween::m_duration).GetFloatSeconds()), 1.0f);
	const type_t y = x0;

	if (x != nullptr) {
		switch (ITween::m_mode) {
		case ITween::MODE_REL_LERP:
			x0 = mmlLerp(a, b, d);
			*x += (x0 - y);
			break;
		case ITween::MODE_REL_SLERP:
			x0 = mmlSlerp(a, b, d);
			*x += (x0 - y);
			break;
		case ITween::MODE_REL_QLERP:
			x0 = mmlQlerp(a, b, d);
			*x += (x0 - y);
			break;
		case ITween::MODE_ABS_LERP:
			*x = mmlLerp(a, b, d);
			x0 = x;
			break;
		case ITween::MODE_ABS_SLERP:
			*x = mmlSlerp(a, b, d);
			x0 = x;
			break;
		case ITween::MODE_ABS_QLERP:
			*x = mmlQlerp(a, b, d);
			x0 = x;
			break;
		}
	}

	if (d >= 1.0f) {
		Entity::Destroy();
	}
}

template < typename type_t >
retro3d::Tween<type_t>::Tween(type_t &from, const type_t &to, retro3d::ITween::Mode mode, retro3d::Time duration, retro3d::TimerType timer_type) : mtlInherit< ITween,Tween<type_t> >(this), x(&from), x0(from), a(from), b(to)
{
	ITween::SetProperties(mode, duration, timer_type);
}

template < typename type_t >
void retro3d::Tween<type_t>::Finish( void )
{
	if (x != nullptr) {
		const type_t y = x0;

		switch (ITween::m_mode) {
		case ITween::MODE_REL_LERP:
		case ITween::MODE_REL_SLERP:
		case ITween::MODE_REL_QLERP:
			x0 = b;
			*x += (x0 - y);
			break;
		case ITween::MODE_ABS_LERP:
		case ITween::MODE_ABS_SLERP:
		case ITween::MODE_ABS_QLERP:
			*x = b;
			x0 = b;
			break;
		}
	}
	Entity::Destroy();
}

#endif // RETRO_TWEEN_H
