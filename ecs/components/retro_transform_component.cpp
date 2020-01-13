#include "retro_transform_component.h"

void retro3d::TransformComponent::OnUpdate( void )
{
	m_old_transform = m_transform->GetFinalMatrix();
}

retro3d::TransformComponent::TransformComponent( void ) : mtlInherit(this), m_old_transform()
{
	m_transform.New();
}

const retro3d::SharedTransform retro3d::TransformComponent::GetTransform( void ) const
{
	return m_transform;
}

retro3d::SharedTransform retro3d::TransformComponent::GetTransform( void )
{
	return m_transform;
}

mmlVector<3> retro3d::TransformComponent::GetLastMove( void ) const
{
	return m_transform->GetPosition() - m_old_transform.GetPosition();
}

retro3d::Transform retro3d::TransformComponent::GetLastTransform( void ) const
{
	return m_old_transform;
}
