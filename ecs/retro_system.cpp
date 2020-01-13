#include "retro_system.h"
#include "../common/MiniLib/MML/mmlMath.h"

retro3d::ISystem::ISystem( void ) :
	mtlBase(this),
	m_is_active(1), m_should_destroy(false)
{}

retro3d::ISystem::~ISystem( void )
{}

void retro3d::ISystem::Destroy( void )
{
	m_should_destroy = true;
}

bool retro3d::ISystem::IsDestroyed( void ) const
{
	return m_should_destroy;
}

bool retro3d::ISystem::IsActive( void ) const
{
	return IsDestroyed() == false && m_is_active > 0;
}

void retro3d::ISystem::Deactivate( void )
{
	--m_is_active;
}

void retro3d::ISystem::Activate( void )
{
	m_is_active = mmlMin(1, m_is_active + 1);
}

retro3d::Engine *retro3d::ISystem::GetEngine( void )
{
	return m_engine;
}

const retro3d::Engine *retro3d::ISystem::GetEngine( void ) const
{
	return m_engine;
}
