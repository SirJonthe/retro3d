#include "../common/MiniLib/MML/mmlMath.h"
#include "retro_entity.h"

void retro3d::Entity::OnSpawn( void )
{}

void retro3d::Entity::OnUpdate( void )
{}

void retro3d::Entity::OnCollision(retro3d::Entity&)
{}

void retro3d::Entity::OnDestroy( void )
{}

retro3d::Entity::Entity( void ) :
	mtlInherit(this),
	m_tag("entity"), m_filter_flags(1), m_uuid(reinterpret_cast<uint64_t>(this)),
	m_engine(nullptr),
	m_game_timer(1, 1_s),
	m_delta_time(0.0),
	m_is_active(1), m_should_destroy(false)
{
	m_game_timer.Start();
}

void retro3d::Entity::Destroy( void )
{
	m_should_destroy = true;
}

bool retro3d::Entity::IsDestroyed( void ) const
{
	return m_should_destroy;
}

bool retro3d::Entity::IsActive( void ) const
{
	return !IsDestroyed() && m_is_active > 0;
}

void retro3d::Entity::Deactivate( void )
{
	--m_is_active;
	m_game_timer.Pause();
}

void retro3d::Entity::Activate( void )
{
	m_is_active = mmlMin(1, m_is_active + 1);
	if (m_is_active > 0) {
		m_game_timer.Start();
	}
}

uint64_t retro3d::Entity::GetUUID( void ) const
{
	return m_uuid;
}

const retro3d::Engine *retro3d::Entity::GetEngine( void ) const
{
	return m_engine;
}

retro3d::Engine *retro3d::Entity::GetEngine( void )
{
	return m_engine;
}

retro3d::RenderDevice *retro3d::Entity::GetRenderer( void )
{
	return m_engine->GetRenderer();
}

const retro3d::RenderDevice *retro3d::Entity::GetRenderer( void ) const
{
	return m_engine->GetRenderer();
}

retro3d::SoundDevice *retro3d::Entity::GetSoundDevice( void )
{
	return m_engine->GetSoundDevice();
}

const retro3d::SoundDevice *retro3d::Entity::GetSoundDevice( void ) const
{
	return m_engine->GetSoundDevice();
}

retro3d::InputDevice *retro3d::Entity::GetInput( void )
{
	return m_engine->GetInput();
}

const retro3d::InputDevice *retro3d::Entity::GetInput( void ) const
{
	return m_engine->GetInput();
}

const std::string &retro3d::Entity::GetTag( void ) const
{
	return m_tag;
}

void retro3d::Entity::SetTag(const std::string &tag)
{
	m_tag = tag;
}

retro3d::Time retro3d::Entity::RealLifeTime( void ) const
{
	return GetEngine()->RealTime() - m_real_time_spawn;
}

retro3d::Time retro3d::Entity::SimulationLifeTime( void ) const
{
	return GetEngine()->SimulationTime() - m_sim_time_spawn;
}

retro3d::Time retro3d::Entity::GameLifeTime( void ) const
{
	return m_game_time;
}

retro3d::Time retro3d::Entity::LifeTime(retro3d::TimerType type) const
{
	switch (type) {
	case TIMER_REAL: return RealLifeTime();
	case TIMER_SIM:  return SimulationLifeTime();
	case TIMER_GAME: return GameLifeTime();
	}
	return retro3d::Time();
}

double retro3d::Entity::DeltaTime( void ) const
{
	return m_delta_time;
}

void retro3d::Entity::SetTimeScale(double time_scale)
{
	m_game_timer.SetTickRate(1, retro3d::Time::FloatSeconds(1.0 / time_scale), false);
}

double retro3d::Entity::GetTimeScale( void ) const
{
	return 1.0 / m_game_timer.GetTimePerTick().GetFloatSeconds();
}

retro3d::RealTimeTimer retro3d::Entity::CreateGameTimer(uint32_t num_ticks, retro3d::Time over_time) const
{
	return m_game_timer.SpawnChild(num_ticks, over_time);
}

retro3d::RealTimeTimer retro3d::Entity::CreateTimer(retro3d::TimerType type, uint32_t num_ticks, retro3d::Time over_time) const
{
	switch (type) {
	case TIMER_REAL: return retro3d::RealTimeTimer(num_ticks, over_time);
	case TIMER_SIM:  return GetEngine()->CreateSimulationTimer(num_ticks, over_time);
	case TIMER_GAME: return CreateGameTimer(num_ticks, over_time);
	}
	return retro3d::RealTimeTimer(num_ticks, over_time);
}

uint64_t retro3d::Entity::GetFilterFlags( void ) const
{
	return m_filter_flags;
}

bool retro3d::Entity::GetFilterFlag(uint32_t flag_index) const
{
	return (m_filter_flags & (uint64_t(1) << flag_index)) != 0;
}

void retro3d::Entity::SetFilterFlags(uint64_t filter_flags)
{
	m_filter_flags = filter_flags;
}

void retro3d::Entity::SetFilterFlag(uint32_t flag_index, bool state)
{
	if (state == true) {
		m_filter_flags |= (uint64_t(1) << flag_index);
	} else {
		m_filter_flags &= ~(uint64_t(1) << flag_index);
	}
}
