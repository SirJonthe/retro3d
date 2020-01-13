#include "../common/MiniLib/MML/mmlMath.h"
#include "retro_component.h"
#include "retro_entity.h"

void retro3d::Component::OnSpawn( void )
{}

void retro3d::Component::OnUpdate( void )
{}

void retro3d::Component::OnCollision(retro3d::Entity&)
{}

void retro3d::Component::OnDestroy( void )
{}

retro3d::Component::Component( void ) :
	mtlInherit(this), m_object(nullptr), m_is_active(1), m_should_destroy(false)
{}

retro3d::Component::~Component( void )
{}

void retro3d::Component::Destroy( void )
{
	m_should_destroy = true;
}

bool retro3d::Component::IsDestroyed( void ) const
{
	return m_should_destroy || m_object->IsDestroyed();
}

bool retro3d::Component::IsActive( void ) const
{
	return !IsDestroyed() && m_is_active > 0 && m_object->IsActive();
}

void retro3d::Component::Deactivate( void )
{
	--m_is_active;
}

void retro3d::Component::Activate( void )
{
	m_is_active = mmlMin(1, m_is_active + 1);
}

retro3d::Entity *retro3d::Component::GetObject( void )
{
	return m_object;
}

const retro3d::Entity *retro3d::Component::GetObject( void ) const
{
	return m_object;
}

retro3d::Engine *retro3d::Component::GetEngine( void )
{
	return m_object->GetEngine();
}

const retro3d::Engine *retro3d::Component::GetEngine( void ) const
{
	return m_object->GetEngine();
}


retro3d::RenderDevice *retro3d::Component::GetRenderer( void )
{
	return m_object->GetRenderer();
}

const retro3d::RenderDevice *retro3d::Component::GetRenderer( void ) const
{
	return m_object->GetRenderer();
}

retro3d::SoundDevice *retro3d::Component::GetSoundDevice( void )
{
	return m_object->GetSoundDevice();
}

const retro3d::SoundDevice *retro3d::Component::GetSoundDevice( void ) const
{
	return m_object->GetSoundDevice();
}

retro3d::InputDevice *retro3d::Component::GetInput( void )
{
	return m_object->GetInput();
}

const retro3d::InputDevice *retro3d::Component::GetInput( void ) const
{
	return m_object->GetInput();
}
