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

void retro3d::DelayedCall::SetProcedure(retro3d::Procedure procedure, uint64_t time)
{
	m_procedure = procedure;
	m_timer.SetTickRate(retro3d::Time::FloatTime(time), retro3d::RealTimeTimer::Units::SecondsPerTick);
	m_timer.Start();
}
