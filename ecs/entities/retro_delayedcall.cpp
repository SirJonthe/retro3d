#include "retro_delayedcall.h"

void retro3d::DelayedCall::OnSpawn( void )
{
	RETRO3D_ASSERT(m_procedure != nullptr && m_timer.IsTicking() == true);
}

void retro3d::DelayedCall::OnUpdate( void )
{
	if (m_timer.IsDue() == true || m_timer.IsTicking() == false) {
		Destroy();
	}
}

void retro3d::DelayedCall::OnDestroy( void )
{
	if (m_procedure != nullptr) {
		m_procedure();
	}
}

retro3d::DelayedCall::DelayedCall( void ) : mtlInherit(this), m_timer(), m_procedure(nullptr)
{}

void retro3d::DelayedCall::SetProcedure(retro3d::Procedure procedure, retro3d::Time time, bool scale_time)
{
	m_procedure = procedure;

	if (scale_time == false) {
		m_timer.Pause();
		m_timer.Reset();
		m_timer.SetParent(nullptr);
		m_timer.SetTickRate(1, time);
	} else {
		m_timer = GetEngine()->SpawnChildTimer(1, time);
	}

	m_timer.Start();
}
