#include "retro_physics_component.h"
#include "../retro_entity.h"
#include "retro_transform_component.h"

namespace retro3d { retro_register_component(PhysicsComponent) }

void retro3d::PhysicsComponent::OnSpawn( void )
{
	AttachTransform(GetObject()->AddComponent<retro3d::TransformComponent>()->GetTransform());
}

void retro3d::PhysicsComponent::OnUpdate( void )
{
	float delta_time = float(GetObject()->DeltaTime());
	StepSimulation(delta_time);
	ApplyLinearImpulse(m_gravity_vec * delta_time);
}

retro3d::PhysicsComponent::PhysicsComponent( void ) : mtlInherit(this), m_gravity_vec()
{
	m_gravity_vec[0] =  0.0f;
	m_gravity_vec[1] = -9.8f;
	m_gravity_vec[2] =  0.0f;
}

mmlVector<3> retro3d::PhysicsComponent::GetGravityVector( void ) const
{
	return m_gravity_vec;
}

void retro3d::PhysicsComponent::SetGravityVector(const mmlVector<3> &vec)
{
	m_gravity_vec = vec;
}
