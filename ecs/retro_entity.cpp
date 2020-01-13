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
	m_name("entity"), m_uuid(reinterpret_cast<uint64_t>(this)),
	m_engine(nullptr),
	m_time(0.0), m_delta_time(0.0), m_time_scale(1.0),
	m_is_active(1), m_should_destroy(false)
{}

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
}

void retro3d::Entity::Activate( void )
{
	m_is_active = mmlMin(1, m_is_active + 1);
}

uint64_t retro3d::Entity::GetInstanceID( void ) const
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

const std::string &retro3d::Entity::GetName( void ) const
{
	return m_name;
}

void retro3d::Entity::SetName(const std::string &name)
{
	m_name = name;
}

double retro3d::Entity::Time( void ) const
{
	return m_time;
}

double retro3d::Entity::DeltaTime( void ) const
{
	return m_delta_time;
}

void retro3d::Entity::SetTimeScale(double scale)
{
	m_time_scale = scale;
}

double retro3d::Entity::GetTimeScale( void ) const
{
	return m_time_scale;
}
