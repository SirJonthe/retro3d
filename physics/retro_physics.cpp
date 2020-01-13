#include "../common/MiniLib/MGL/mglCollision.h"
#include "retro_physics.h"

retro3d::PhysicsProperties::PhysicsProperties( void ) : m_transform(), m_center_of_mass_offset(mmlVector<3>::Fill(0.0f)), m_linear_velocity(mmlVector<3>::Fill(0.0f)), m_angular_velocity(mmlVector<3>::Fill(0.0f)), m_inv_inertia_tensor(mmlMatrix<3,3>::Identity()), m_mass(1.0f), m_inv_mass(1.0f), m_friction_static(0.2f), m_friction_kinetic(0.1f), m_restitution(0.5f)
{
	m_transform.New();
}

void retro3d::PhysicsProperties::UpdateInertiaTensor(const mmlVector<3> &rigidbody_halfextents)
{
	mmlVector<3> h = mmlAbs(rigidbody_halfextents);
	m_inv_inertia_tensor = mmlMatrix<3,3>::Identity();
	m_inv_inertia_tensor[0][0] = 3.0f / (m_mass * (h[1] * h[1] + h[2] * h[2]));
	m_inv_inertia_tensor[1][1] = 3.0f / (m_mass * (h[0] * h[0] + h[2] * h[2]));
	m_inv_inertia_tensor[2][2] = 3.0f / (m_mass * (h[0] * h[0] + h[1] * h[1]));

	std::cout << "it=" << std::endl;
	for (int i = 0; i < 3; ++i) {
		std::cout << "\t";
		for (int j = 0; j < 3; ++j) {
			std::cout << m_inv_inertia_tensor[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void retro3d::PhysicsProperties::AttachTransform(retro3d::SharedTransform t)
{
	m_transform = t;
	if (m_transform.IsNull()) {
		m_transform.New();
	}
}

void retro3d::PhysicsProperties::SetTransform(const retro3d::Transform &transform)
{
	*m_transform.GetShared() = transform;
}

const retro3d::Transform &retro3d::PhysicsProperties::GetTransform( void ) const
{
	return *m_transform.GetShared();
}

retro3d::Transform &retro3d::PhysicsProperties::GetTransform( void )
{
	return *m_transform.GetShared();
}

retro3d::SharedTransform retro3d::PhysicsProperties::GetSharedTransform( void ) const
{
	return m_transform;
}

void retro3d::PhysicsProperties::ApplyLinearImpulse(const mmlVector<3> &impulse)
{
	m_linear_velocity += impulse;
}

void retro3d::PhysicsProperties::ApplyForce(const mmlVector<3> &world_impact_point, mmlVector<3> world_force_direction, float force_magnitude)
{
	// Resources:
	// https://en.wikipedia.org/wiki/Momentum
	// https://gafferongames.com/post/physics_in_3d/
	// https://github.com/DanielChappuis/reactphysics3d/blob/master/src/engine/DynamicsWorld.cpp

	// ATTEMPT 3:

	// imagine the force is an incoming bullet
	// world_impact_point    = the impact point of a force
	// world_force_direction = the direction of the bullet (NOT the collision normal or negative collision normal!)
	// force_magnitude       = the magnitude of the force of the bullet

	if (world_force_direction.Len2() <= 0.0f) { return; }
	world_force_direction.NormalizeIf();

	const mmlVector<3> impact_vector = world_impact_point - GetCenterOfMass();
	const float impact_radius = impact_vector.Len();
	const mmlVector<3> x_axis = impact_vector / impact_radius; // normal pointing from center of mass to impact point
	const mmlVector<3> z_axis = world_force_direction;         // direction of the incoming force
	const mmlVector<3> y_axis = mmlCross(x_axis, z_axis);

	force_magnitude = force_magnitude /* * m_restitution */ * m_inv_mass; // alter force magnitude based on body's properties

	// The spin
	const float s_ratio = y_axis.Len(); // since both x_axis and z_axis are unit length, this will be a fraction/ratio (0-1)
	if (s_ratio > 0.0f) {
		const float revolutions = force_magnitude / (2.0f * mmlPI * impact_radius);
		m_angular_velocity += y_axis * revolutions; // NOTE: s_ratio should not be a part of this calculation since y_axis is already scaled...
	}

	// The translation
	const float t_ratio = 1.0f - s_ratio;
	if (t_ratio > 0.0f) {
		m_linear_velocity -= x_axis * (t_ratio * force_magnitude); // NOTE: x_axis is a unit length vector, so need to multiply by t_ratio to scale it properly unlike y_axis
	}
}

void retro3d::PhysicsProperties::NullifyVelocity( void )
{
	m_linear_velocity = mmlVector<3>::Fill(0.0f);
	m_angular_velocity = mmlVector<3>::Fill(0.0f);
}

void retro3d::PhysicsProperties::NullifyLinearVelocity( void )
{
	m_linear_velocity = mmlVector<3>::Fill(0.0f);
}

void retro3d::PhysicsProperties::NullifyAngularVelocity( void )
{
	m_angular_velocity = mmlVector<3>::Fill(0.0f);
}

mmlVector<3> retro3d::PhysicsProperties::GetVelocity(const mmlVector<3> &world_point) const
{
//	return (m_linear_velocity + (m_angular_velocity * world_point)); // works if m_angular_velocity is a quaternion (if quaternion is implemented properly that is)
	// NOTE: Resource: https://gafferongames.com/post/physics_in_3d/
	return m_linear_velocity + mmlCross(GetAngularVelocity(), world_point - GetCenterOfMass());
}

mmlVector<3> retro3d::PhysicsProperties::GetLinearVelocity( void ) const
{
	return m_linear_velocity;
}

mmlVector<3> retro3d::PhysicsProperties::GetAngularVelocity( void ) const
{
	return m_angular_velocity;
}

mmlVector<3> retro3d::PhysicsProperties::GetMomentum(const mmlVector<3> &world_point) const
{
	return GetVelocity(world_point) * m_mass;
}

mmlVector<3> retro3d::PhysicsProperties::GetLinearMomentum( void ) const
{
	return GetLinearVelocity() * m_mass;
}

mmlVector<3> retro3d::PhysicsProperties::GetAngularMomentum( void ) const
{
	return GetAngularVelocity() * m_mass;
}

void retro3d::PhysicsProperties::SetMass(float mass)
{
	m_mass = mass > 0.0f ? mass : 0.001f;
	m_inv_mass = 1.0f / m_mass;
}
float retro3d::PhysicsProperties::GetMass( void ) const
{
	return m_mass;
}

float retro3d::PhysicsProperties::GetInverseMass( void ) const
{
	return m_inv_mass;
}

mmlVector<3> retro3d::PhysicsProperties::GetCenterOfMass( void ) const
{
	return m_transform->GetPosition() + m_transform->TransformVector(m_center_of_mass_offset);
}

void retro3d::PhysicsProperties::StepSimulation(float time_delta)
{
	retro3d::Transform *t = m_transform.GetShared();
	t->SetPosition(t->GetPosition() + m_linear_velocity * time_delta);
	const float len2 = m_angular_velocity.Len2();
	if (mmlIsApproxZero(len2) == false) {
		const float len = mmlSqrt(len2); // NOTE: 'len' is not an angle, but a distance. Need to convert to radians (see below).
		t->SetBasis(mmlOrthoNormalize(mmlAxisAngle(m_angular_velocity / len, len * 2.0f * mmlPI * time_delta) * t->GetBasis()));
	}

	// NOTE: quaternion is multiplied by 0.5. Why?
	// Same in resource https://gafferongames.com/post/physics_in_3d/

	// ReactPhysics3D
//	mConstrainedPositions[indexArray] = currentPosition + newLinVelocity * mTimeStep;
//	mConstrainedOrientations[indexArray] = currentOrientation +
//		Quaternion(0, newAngVelocity) *
//		currentOrientation * decimal(0.5) * mTimeStep;

}

static const float WorldSize = 8.0f;

static int32_t WorldSpringsActive = 1;
static const float Kws = 0.7f; // Hooke's spring constant
static const float Kwd = 0.1f; // daming constant


struct WorldSpring
{
	uint32_t body_index;
	uint32_t vertex_index;
	mmlVector<3> anchor;
};

static WorldSpring WorldSprings[2] = {
	{ 0, 2, mmlVector<3>(-1.5, 0.0, 0.0) },
	{ 5, 2, mmlVector<3>(1.5, 0.0, 0.0) }
};

static const uint32_t NumberOfWorldSprings = sizeof(WorldSprings) / sizeof(WorldSpring);

static int32_t BodySpringsActive = 1;
static const float Kbs = 0.6f; // Hooke's spring constant
static const float Kbd = 0.1f; // daming constant

struct BodySpring
{
	uint32_t body_0_index;
	uint32_t body_0_vertex_index;
	uint32_t body_1_index;
	uint32_t body_1_vertex_index;
};

static BodySpring BodySprings[5] = {
	{ 0, 6, 1, 2 },
	{ 1, 6, 2, 2 },
	{ 2, 6, 3, 2 },
	{ 3, 7, 4, 2 },
	{ 4, 6, 5, 0 }
};

static const uint32_t NumberOfBodySprings = sizeof(BodySprings) / sizeof(BodySpring);


static int32_t GravityActive = 1;
static mmlVector<3> Gravity(0.0, -10.0, 0.0);

static int32_t DampingActive = 0;
static const float NoKdl = 0.002f; // the no-damping linear daming factor
static const float NoKda = 0.001f; // the no-damping angular damping factor
static const float Kdl = 0.04f; // linear daming factor
static const float Kda = 0.01f; // angular daming factor

#define TOGGLE(X) (X = X ? 0 : 1)

#include "../common/MiniLib/MML/mmlRandom.h"
static mmlRandom rnd;

float GenerateUnitRandomReal( void )
{
	return rnd.GetFloat();
}

float GenerateReasonableRandomReal( void )
{
	return 0.1f + 0.2f * GenerateUnitRandomReal();
}

void retro3d::PhysicsIntegrator::Initialize( void )
{
	for(int BodyIndex = 0; BodyIndex < NumberOfBodies; BodyIndex++)
	{
		// initialize rigid bodies by randomly generating boxes

		retro3d::RigidBody &Body = bodies[BodyIndex];

		// 1/2 the dimensions of the box
		double dX2 = double(GenerateReasonableRandomReal());
		double dY2 = double(GenerateReasonableRandomReal());
		double dZ2 = double(GenerateReasonableRandomReal());

		double Density = 0.4;
		double Mass = 8.0 * Density * dX2*dY2*dZ2;
		assert(Mass > 0.0);

		Body.mass = float(Mass);
		Body.inv_mass = float(1.0 / Mass);
		Body.inv_body_inertia_tensor = mmlMatrix<3,3>::Identity();
		Body.inv_body_inertia_tensor[0][0] = float(3.0 / (Mass*(dY2*dY2+dZ2*dZ2)));
		Body.inv_body_inertia_tensor[1][1] = float(3.0 / (Mass*(dX2*dX2+dZ2*dZ2)));
		Body.inv_body_inertia_tensor[2][2] = float(3.0 / (Mass*(dX2*dX2+dY2*dY2)));

		Body.restitution_coef = 1.0f;

		// Body.aConfigurations[0].CMPosition;

		// initialize geometric quantities
		// we'll use the body index+1 for the display list id

		// generate the body-space bounding volume vertices

		assert(Body.MaxNumberOfBoundingVertices > 8);
		Body.number_of_bounding_vertices = 8;
		Body.body_bounding_verts[0] = mmlVector<3>( dX2, dY2, dZ2);
		Body.body_bounding_verts[1] = mmlVector<3>( dX2, dY2,-dZ2);
		Body.body_bounding_verts[2] = mmlVector<3>( dX2,-dY2, dZ2);
		Body.body_bounding_verts[3] = mmlVector<3>( dX2,-dY2,-dZ2);
		Body.body_bounding_verts[4] = mmlVector<3>(-dX2, dY2, dZ2);
		Body.body_bounding_verts[5] = mmlVector<3>(-dX2, dY2,-dZ2);
		Body.body_bounding_verts[6] = mmlVector<3>(-dX2,-dY2, dZ2);
		Body.body_bounding_verts[7] = mmlVector<3>(-dX2,-dY2,-dZ2);
	}
}

retro3d::PhysicsIntegrator::PhysicsIntegrator(float world_x, float world_y, float world_z) : source_config_index(0), target_config_index(1)
{
	Initialize();

	// initialize walls
	walls[0].normal = mmlVector<3>(0.0, -1.0, 0.0);
	walls[0].d = world_y / 2.0f;
	walls[1].normal = mmlVector<3>(0.0, 1.0, 0.0);
	walls[1].d = world_y / 2.0f;

	walls[2].normal = mmlVector<3>(-1.0, 0.0, 0.0);
	walls[2].d = world_x / 2.0f;
	walls[3].normal = mmlVector<3>(1.0, 0.0, 0.0);
	walls[3].d = world_x / 2.0f;

	walls[4].normal = mmlVector<3>(0.0, 0.0 , -1.0);
	walls[4].d = world_z / 2.0f;
	walls[5].normal = mmlVector<3>(0.0, 0.0, 1.0);
	walls[5].d = world_z / 2.0f;

	CalculateVertices(0);
}

void retro3d::PhysicsIntegrator::Simulate(float delta_time)
{
	float current_time = 0.0f;
	float target_time = delta_time;

	while (current_time < target_time) {
		ComputeForces(source_config_index);
		Integrate(target_time - current_time);
		CalculateVertices(target_config_index);
		CheckForCollisions(target_config_index);

		if (collision_state == Penetrating) {

			// we simulated too far, so subdivide time and try again
			target_time = (current_time + target_time) / 2.0f;

			if (mmlIsApproxZero(target_time - current_time)) {
				break;
			}

		} else {
			if (collision_state == Colliding) {
				int32_t counter = 0;
				do {
						ResolveCollisions(target_config_index);
						++counter;
				} while (CheckForCollisions(target_config_index) == Colliding && counter < 100);

				if (counter >= 100) { break; }
			}

			// we made a successful step, so swap configurations to save the data for the next step

			current_time = target_time;
			target_time = delta_time;

			TOGGLE(source_config_index);
			TOGGLE(target_config_index);
		}
	}
}

void retro3d::PhysicsIntegrator::ComputeForces(int config_index)
{
	int32_t counter;
	for (counter = 0; counter < NumberOfBodies; ++counter) {
		retro3d::RigidBody body = bodies[counter];
		retro3d::RigidBody::Configuration &config = body.configs[config_index];

		// clear forces
		config.torque = mmlVector<3>::Fill(0.0f);
		config.cm_force = mmlVector<3>::Fill(0.0f);

		if (GravityActive) {
			config.cm_force += Gravity * body.mass;
		}

		if (DampingActive) {
			config.cm_force += -Kdl * config.cm_velocity;
			config.torque += -Kda * config.angular_velocity;
		} else {
			// there's always a little damping because our integrator sucks
			config.cm_force += -NoKdl * config.cm_velocity;
			config.torque += -NoKda * config.angular_velocity;
		}
	}

	if (BodySpringsActive) {
		for (int i = 0; i < NumberOfBodySprings; ++i) {
			BodySpring &spring_struct = BodySprings[i];
			retro3d::RigidBody &body0 = bodies[spring_struct.body_0_index];
			retro3d::RigidBody::Configuration &config0 = body0.configs[config_index];
			mmlVector<3> position0 = config0.bounding_verts[spring_struct.body_0_vertex_index];
			mmlVector<3> U0 = position0 - config0.cm_position;
			mmlVector<3> VU0 = config0.cm_velocity + mmlCross(config0.angular_velocity, U0);

			retro3d::RigidBody &body1 = bodies[spring_struct.body_1_index];
			retro3d::RigidBody::Configuration &config1 = body1.configs[config_index];
			mmlVector<3> position1 = config1.bounding_verts[spring_struct.body_1_vertex_index];
			mmlVector<3> U1 = position1 - config1.cm_position;
			mmlVector<3> VU1 = config1.cm_velocity + mmlCross(config1.angular_velocity, U1);

			// spring goes from 0 to 1

			mmlVector<3> spring_vec = position1 - position0;
			mmlVector<3> spring = -Kbs * spring_vec;

			mmlVector<3> rel_vel = VU1 - VU0;
			// project velocity onto spring to get daming vector
			// this is basicallt a Gram-Schmidt projection
			mmlVector<3> damping_force = -Kbd * (mmlDot(rel_vel, spring_vec) / mmlDot(spring_vec, spring_vec)) * spring_vec;

			spring += damping_force;

			config0.cm_force -= spring;
			config0.torque -= mmlCross(U0, spring);

			config1.cm_force += spring;
			config1.torque += mmlCross(U1, spring);
		}
	}

	if (WorldSpringsActive) {
		for (int i = 0; i < NumberOfWorldSprings; ++i) {
			WorldSpring &spring_struct = WorldSprings[i];

			retro3d::RigidBody &body = bodies[spring_struct.body_index];
			retro3d::RigidBody::Configuration &config = body.configs[config_index];

			mmlVector<3> position = config.bounding_verts[spring_struct.vertex_index];
			mmlVector<3> U = position - config.cm_position;
			mmlVector<3> VU = config.cm_velocity + mmlCross(config.angular_velocity, U);

			mmlVector<3> spring = -Kws * (position - spring_struct.anchor);
			// project velocity onto spring to get daming vector
			// this is basically Gram-Schmidt projection
			mmlVector<3> daming_force = -Kwd * (mmlDot(VU, spring)/mmlDot(spring, spring)) * spring;

			spring += daming_force;

			config.cm_force += spring;
			config.torque += mmlCross(U, spring);
		}
	}
}

void retro3d::PhysicsIntegrator::CalculateVertices(int config_index)
{
	for (int counter = 0; counter < NumberOfBodies; ++counter) {
		retro3d::RigidBody &body = bodies[counter];
		retro3d::RigidBody::Configuration &config = body.configs[config_index];

		const mmlMatrix<3,3> &A = config.orientation;
		const mmlVector<3> &R = config.cm_position;

		for (uint32_t i = 0; i < body.number_of_bounding_vertices; ++i) {
			config.bounding_verts[i] = R + body.body_bounding_verts[i] * A;
		}
	}
}

void retro3d::PhysicsIntegrator::Integrate(float delta_time)
{
	for (int counter = 0; counter < NumberOfBodies; ++counter) {
		retro3d::RigidBody::Configuration &source = bodies[counter].configs[source_config_index];
		retro3d::RigidBody::Configuration &target = bodies[counter].configs[target_config_index];

		// integrate primary quantities

		target.cm_position = source.cm_position + delta_time * source.cm_velocity;

		target.orientation = source.orientation + delta_time * mmlSkew(source.angular_velocity) * source.orientation;

		target.cm_velocity = source.cm_velocity + (delta_time * bodies[counter].inv_mass) * source.cm_force;

		target.angular_momentum = source.angular_momentum + delta_time * source.torque;

		target.orientation = mmlOrthoNormalize(target.orientation);

		// compute auxilary quantities

		target.inv_world_inertia_tensor = target.orientation * bodies[counter].inv_body_inertia_tensor * mmlTransp(target.orientation);

		target.angular_velocity = target.angular_momentum * target.inv_world_inertia_tensor;
	}
}

retro3d::PhysicsIntegrator::CollisionState retro3d::PhysicsIntegrator::CheckForCollisions(int config_index)
{
	collision_state = Clear;
	const float DepthEpsilon = 0.001f;

	for (int32_t BodyIndex = 0; BodyIndex < NumberOfBodies && collision_state != Penetrating; ++BodyIndex) {
		retro3d::RigidBody &body = bodies[BodyIndex];
		retro3d::RigidBody::Configuration &config = body.configs[config_index];

		for (uint32_t counter = 0; counter < body.number_of_bounding_vertices && collision_state != Penetrating; ++counter) {
			mmlVector<3> position = config.bounding_verts[counter];
			mmlVector<3> U = position - config.cm_position;

			mmlVector<3> vel = config.cm_velocity + mmlCross(config.angular_velocity, U);

			for (int32_t WallIndex = 0; WallIndex < NumberOfWalls && collision_state != Penetrating; ++WallIndex) {
				Wall &wall = walls[WallIndex];
				float axbycxd = mmlDot(position, wall.normal) + wall.d;
				if (axbycxd < -DepthEpsilon) { // HACK: epsilon
					collision_state = Penetrating;
				} else if (axbycxd < DepthEpsilon) { // HACK: epsilon
					float rel_vel = mmlDot(wall.normal, vel);

					if (rel_vel < 0.0f) {
						collision_state = Colliding;
						collision_normal = wall.normal;
						colliding_corner_index = counter;
						colliding_body_index = BodyIndex;
					}
				}
			}
		}
	}

	return collision_state;
}

void retro3d::PhysicsIntegrator::ResolveCollisions(int config_index)
{
	retro3d::RigidBody &body = bodies[colliding_body_index];
	retro3d::RigidBody::Configuration &config = body.configs[config_index];

	mmlVector<3> position = config.bounding_verts[colliding_corner_index];

	mmlVector<3> R = position - config.cm_position;

	mmlVector<3> vel = config.cm_velocity + mmlCross(config.angular_velocity, R);

	float impulse_numerator = -(1.0f + body.restitution_coef) * mmlDot(vel, collision_normal);

	float impulse_denominator = body.inv_mass + mmlDot(mmlCross(mmlCross(R, collision_normal) * config.inv_world_inertia_tensor, R), collision_normal);

	mmlVector<3> impulse = (impulse_numerator / impulse_denominator) * collision_normal;

	// apply impulse to primary quantities
	config.cm_velocity += body.inv_mass * impulse;
	config.angular_momentum += mmlCross(R, impulse);

	// compute affected auziliary quantities
	config.angular_velocity = config.angular_momentum * config.inv_world_inertia_tensor;
}
