#include "retro_delayedcall.h"

void retro3d::DelayedCall::OnUpdate( void )
{
	if (LifeTime(m_timer_type) >= m_time) {
		(*m_procedure.GetShared())();
		Destroy();
	}
}

retro3d::DelayedCall::DelayedCall(const mtlShared<retro3d::IProcedure> &procedure, retro3d::Time time, retro3d::TimerType timer_type) : mtlInherit(this), m_time(time), m_timer_type(timer_type), m_procedure(procedure)
{}
