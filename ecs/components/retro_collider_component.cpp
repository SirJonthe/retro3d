#include "retro_collider_component.h"
#include "retro_transform_component.h"
#include "../retro_entity.h"

void retro3d::ColliderComponent::OnSpawn( void )
{
	m_collider->AttachTransform(GetObject()->AddComponent<retro3d::TransformComponent>()->GetTransform());
	m_reinsert = false;
}

retro3d::ColliderComponent::ColliderComponent( void ) : mtlInherit(this), m_filter_flags(uint64_t(~0)), m_is_static(false), m_reinsert(false)
{
	CreateCollider<retro3d::AABBCollider>();
}

retro3d::Collider &retro3d::ColliderComponent::GetCollider( void )
{
	return *m_collider.GetShared();
}

const retro3d::Collider &retro3d::ColliderComponent::GetCollider( void ) const
{
	return *m_collider.GetShared();
}

uint64_t retro3d::ColliderComponent::GetFilterFlags( void ) const
{
	return m_filter_flags;
}

void retro3d::ColliderComponent::SetFilterFlags(uint64_t filter_flags)
{
	m_filter_flags = filter_flags;
	m_reinsert = true;
}

bool retro3d::ColliderComponent::IsStatic( void ) const
{
	return m_is_static;
}

void retro3d::ColliderComponent::ToggleStatic( void )
{
	m_is_static = !m_is_static;
	m_reinsert = true;
}

bool retro3d::ColliderComponent::ShouldReinsert( void ) const
{
	return m_reinsert;
}

void retro3d::ColliderComponent::ToggleReinsert( void )
{
	m_reinsert = !m_reinsert;
}
