#ifndef RETRO_PHYSICS_COMPONENT_H
#define RETRO_PHYSICS_COMPONENT_H

#include "../retro_component.h"
#include "../../physics/retro_physics.h"

namespace retro3d
{

retro_component(PhysicsComponent), public retro3d::PhysicsProperties
{
private:
	mmlVector<3> m_gravity_vec;

protected:
	void OnSpawn( void ) override;
	void OnUpdate( void ) override;

public:
	PhysicsComponent( void );

	mmlVector<3> GetGravityVector( void ) const;
	void SetGravityVector(const mmlVector<3> &vec);
};

}

#endif // RETRO_PHYSICS_COMPONENT_H
