#include "retro_timer_component.h"

namespace retro3d { retro_register_component(TimerComponent) }

#define MS_PER_SEC 1000

void retro3d::TimerComponent::OnUpdate( void )
{
	m_ticks += GetEngine()->DeltaTime() * m_bps;
	m_p_ticks = m_ticks;
	m_i_ticks = uint64_t(m_ticks);

	for (uint64_t i = 0; i < m_i_ticks && m_on_tick != nullptr; ++i) {
		m_on_tick();
	}

	m_ticks -= m_i_ticks;
}

retro3d::TimerComponent::TimerComponent( void ) : mtlInherit(this), m_ticks(0.0), m_p_ticks(0.0), m_i_ticks(0), m_on_tick(nullptr), m_is_ticking(false)
{}

void retro3d::TimerComponent::SetTickRate(double rate, retro3d::RealTimeTimer::Units units)
{
	rate = mmlMax(rate, 0.0);
	RETRO3D_ASSERT(rate > 0.0);
	m_bps = (units == retro3d::RealTimeTimer::TicksPerSecond) ? rate : 1.0 / rate;
}

double retro3d::TimerComponent::GetTickRate(retro3d::RealTimeTimer::Units units) const
{
	return (units == retro3d::RealTimeTimer::TicksPerSecond) ? m_bps : 1.0 / m_bps;
}

void retro3d::TimerComponent::Start( void )
{
	if (!IsTicking()) {
		m_is_ticking = true;
	}
}

void retro3d::TimerComponent::Stop( void )
{
	if (IsTicking()) {
		m_is_ticking = false;
	}
}

void retro3d::TimerComponent::Toggle( void )
{
	if (IsTicking()) {
		Stop();
	} else {
		Start();
	}
}

void retro3d::TimerComponent::Reset( void )
{
	m_ticks = 0.0;
}

bool retro3d::TimerComponent::IsTicking( void ) const
{
	return m_is_ticking;
}

bool retro3d::TimerComponent::IsStopped( void ) const
{
	return !m_is_ticking;
}

bool retro3d::TimerComponent::IsDue( void ) const
{
	return m_i_ticks > 0;
}

double retro3d::TimerComponent::GetTime( void ) const
{
	return m_p_ticks;
}

uint64_t retro3d::TimerComponent::GetTicks( void ) const
{
	return m_i_ticks;
}

void retro3d::TimerComponent::SetProcedure(retro3d::Procedure procedure)
{
	m_on_tick = procedure;
}

void retro3d::TimerComponent::RemoveProcedure( void )
{
	m_on_tick = nullptr;
}
