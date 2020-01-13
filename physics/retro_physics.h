#ifndef RETRO_PHYSICS_H
#define RETRO_PHYSICS_H

#include "../common/MiniLib/MML/mmlMatrix.h"
#include "../common/MiniLib/MML/mmlVector.h"
#include "../common/MiniLib/MTL/mtlPointer.h"
#include "../common/MiniLib/MML/mmlQuaternion.h"
#include "../common/retro_transform.h"
#include "retro_collider.h"

// https://www.gamasutra.com/view/feature/131761/c_data_structures_for_rigidbody_.php?page=5

namespace retro3d
{

class PhysicsProperties
{
private:
	retro3d::SharedTransform m_transform;
	mmlVector<3>             m_center_of_mass_offset;
	mmlVector<3>             m_linear_velocity;
	mmlVector<3>             m_angular_velocity; // unit is axis, length is speed
	mmlMatrix<3,3>           m_inv_inertia_tensor;
	float                    m_mass;
	float                    m_inv_mass;
	float                    m_friction_static;
	float                    m_friction_kinetic;
	float                    m_restitution;

public:
	PhysicsProperties( void );

	void                      UpdateInertiaTensor(const mmlVector<3> &rigidbody_halfextents);

	void                      AttachTransform(retro3d::SharedTransform t);
	void                      SetTransform(const retro3d::Transform &transform);
	const retro3d::Transform &GetTransform( void ) const;
	retro3d::Transform       &GetTransform( void );
	retro3d::SharedTransform  GetSharedTransform( void ) const;

	void ApplyLinearImpulse(const mmlVector<3> &impulse);
	void ApplyForce(const mmlVector<3> &world_impact_point, mmlVector<3> world_force_direction, float force_magnitude);

	void NullifyVelocity( void );
	void NullifyLinearVelocity( void );
	void NullifyAngularVelocity( void );

	mmlVector<3> GetVelocity(const mmlVector<3> &world_point) const;
	mmlVector<3> GetLinearVelocity( void ) const;
	mmlVector<3> GetAngularVelocity( void ) const;

	mmlVector<3> GetMomentum(const mmlVector<3> &world_point) const;
	mmlVector<3> GetLinearMomentum( void ) const;
	mmlVector<3> GetAngularMomentum( void ) const;

	void  SetMass(float mass);
	float GetMass( void ) const;
	float GetInverseMass( void ) const;

	mmlVector<3> GetCenterOfMass( void ) const;

	void StepSimulation(float time_delta);
};

struct RigidBody
{
	float          mass = 1.0f;
	float          inv_mass = 1.0f;
	mmlMatrix<3,3> inv_body_inertia_tensor = mmlMatrix<3,3>::Identity(); // prevents bodies with irregular shape to rotate freely. must be calculated based on shape.
	float          restitution_coef = 1.0f;

	uint32_t number_of_bounding_vertices;
	enum { MaxNumberOfBoundingVertices = 20 };
	mmlVector<3> body_bounding_verts[MaxNumberOfBoundingVertices];

	enum { NumberOfConfigurations = 2 };

	struct Configuration
	{
		mmlVector<3>   cm_position = mmlVector<3>::Fill(0.0f);
		mmlMatrix<3,3> orientation = mmlMatrix<3,3>::Identity();

		mmlVector<3>   cm_velocity = mmlVector<3>::Fill(0.0f);
		mmlVector<3>   angular_momentum = mmlVector<3>::Fill(0.0f);

		mmlVector<3>   cm_force = mmlVector<3>::Fill(0.0f);
		mmlVector<3>   torque = mmlVector<3>::Fill(0.0f);

		mmlMatrix<3,3> inv_world_inertia_tensor = mmlMatrix<3,3>::Identity();
		mmlVector<3>   angular_velocity = mmlVector<3>::Fill(0.0f);

		mmlVector<3> bounding_verts[MaxNumberOfBoundingVertices];

		// don't need bounding vertices...
	} configs[NumberOfConfigurations];

	// TODO: something to calculate inertia tensor
};

class PhysicsIntegrator
{
private:
	enum CollisionState
	{
		Penetrating,
		Colliding,
		Clear
	} collision_state;

	mmlVector<3> collision_normal;
	int32_t      colliding_body_index;
	int32_t      colliding_corner_index;
	int32_t      source_config_index;
	int32_t      target_config_index;

	enum { NumberOfWalls = 6 };
	struct Wall
	{
		mmlVector<3> normal;
		float        d;
	};
	Wall walls[NumberOfWalls];

	enum { NumberOfBodies = 6 };
	retro3d::RigidBody bodies[NumberOfBodies];

	void           Initialize( void );
	void           ComputeForces(int32_t config_index);
	void           Integrate(float delta_time);
	CollisionState CheckForCollisions(int32_t config_index);
	void           ResolveCollisions(int32_t config_index);
	void           CalculateVertices(int32_t config_index);

public:
	PhysicsIntegrator(float world_x, float world_y, float world_z);

	void Simulate(float delta_time);
};

}

#endif // RETRO_PHYSICS_H
