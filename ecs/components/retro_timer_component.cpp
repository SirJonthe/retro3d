#include "retro_timer_component.h"

namespace retro3d { retro_register_component(TimerComponent) }

void retro3d::TimerComponent::OnSpawn( void )
{
	m_timer = this->GetObject()->CreateGameTimer();
}

void retro3d::TimerComponent::OnUpdate( void )
{
	const uint32_t ticks = m_timer.GetTicks();
	m_timer.ResetTicks();

	if (m_on_tick.IsNull() == false && m_on_tick->IsNull() == false) {
		const uint32_t clipped_ticks = ticks > 0 && m_multitick == false ? 1 : ticks;
		for (uint32_t i = 0; i < clipped_ticks; ++i) {
			(*m_on_tick.GetShared())();
		}
	}

	if (m_tick_count > -1 && (m_tick_count -= ticks) == 0) {
		Destroy();
	}
}

void retro3d::TimerComponent::OnDestroy( void )
{
	if (m_on_destroy.IsNull() == false) {
		(*m_on_destroy.GetShared())();
	}
}

retro3d::TimerComponent::TimerComponent( void ) : mtlInherit(this), m_timer(), m_tick_count(-1), m_on_tick(), m_on_destroy(), m_multitick(false)
{}

void retro3d::TimerComponent::SetTickRate(retro3d::TimerType timer_type, uint32_t num_ticks, retro3d::Time over_time)
{
	switch (timer_type) {
	case retro3d::TIMER_REAL:
		m_timer = retro3d::RealTimeTimer(num_ticks, over_time);
		break;
	case retro3d::TIMER_SIM:
		m_timer = GetEngine()->CreateSimulationTimer(num_ticks, over_time);
		break;
	case retro3d::TIMER_GAME:
		m_timer = GetObject()->CreateGameTimer(num_ticks, over_time);
		break;
	}
}

void retro3d::TimerComponent::Start( void )
{
	m_timer.Start();
}

void retro3d::TimerComponent::Pause( void )
{
	m_timer.Pause();
}

void retro3d::TimerComponent::Reset( void )
{
	m_timer.Reset();
}

void retro3d::TimerComponent::SetOnTick(const mtlShared<retro3d::IProcedure> &procedure)
{
	m_on_tick = procedure;
}

void retro3d::TimerComponent::DisableOnTick( void )
{
	m_on_tick.Delete();
}

void retro3d::TimerComponent::SetOnDestroy(const mtlShared<retro3d::IProcedure> &procedure)
{
	m_on_destroy = procedure;
}

void retro3d::TimerComponent::DisableOnDestroy( void )
{
	m_on_destroy.Delete();
}
