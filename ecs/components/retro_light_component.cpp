#include "retro_light_component.h"
#include "retro_transform_component.h"
#include "../retro_entity.h"

namespace retro3d { retro_register_component(LightComponent) }

void retro3d::LightComponent::OnSpawn( void )
{
	m_collider.AttachTransform(GetObject()->AddComponent<retro3d::TransformComponent>()->GetTransform());
	m_collider.SetLocalAABB(retro3d::AABB(mmlVector<3>::Fill(0.0f), mmlVector<3>::Fill(radius)));
}

retro3d::LightComponent::LightComponent( void ) : mtlInherit(this), Light(), m_is_static(true), m_reinsert(false), m_filter_flags(~uint64_t(0))
{}

uint64_t retro3d::LightComponent::GetFilterFlags( void ) const
{
	return m_filter_flags;
}

void retro3d::LightComponent::SetFilterFlags(uint64_t filter_flags)
{
	m_filter_flags = filter_flags;
}

bool retro3d::LightComponent::IsStatic( void ) const
{
	return m_is_static;
}

void retro3d::LightComponent::ToggleStatic( void )
{
	m_is_static = !m_is_static;
}

bool retro3d::LightComponent::ShouldReinsert( void ) const
{
	return m_reinsert;
}

void retro3d::LightComponent::ToggleReinsert( void )
{
	m_reinsert = !m_reinsert;
}

retro3d::AABBCollider &retro3d::LightComponent::GetActivationBounds( void )
{
	return m_collider;
}
