#include "retro3d.h"
#include "retro_object.h"

void retro3d::Object::UpdatePhysics(const mmlVector<3> &gravity)
{
	if (m_has_physics == true) {
//		const float delta_time = float(GetEngine()->DeltaTime());
//		SetTransform(m_physics.GetNextTransform());
//		m_physics.ApplyForceAtCenter(gravity, delta_time);
	}
}

void retro3d::Object::OnSpawn( void )
{}

void retro3d::Object::OnUpdate( void )
{}

void retro3d::Object::OnCollision(retro3d::Object&)
{}

void retro3d::Object::OnDestroy( void )
{}

retro3d::Object::Object( void ) :
	mtlBase(this),
	m_name("Object"),
	m_model(), m_collider(&m_default_collider),
	m_transform(), m_old_transform(mmlMatrix<4,4>::Identity()), m_delta_transform(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
	m_default_collider(),
	m_voice(),
	m_physics(),
	m_time(0.0), m_delta_time(0.0), m_time_scale(1.0),
	m_is_active(1),
	m_is_visible(true),
	m_is_collideable(true), m_has_physics(false),
	m_is_mute(false),
	m_debug_aabb(false),
	m_should_destroy(false)
{
	m_transform.New();
	*m_transform.GetShared() = mmlMatrix<4,4>::Identity();
	m_default_collider.AttachTransform(m_transform);
	m_physics.AttachTransform(m_transform);
}

retro3d::Object::~Object( void )
{}

void retro3d::Object::Destroy( void )
{
	m_should_destroy = true;
}

retro3d::Transform retro3d::Object::GetTransform( void ) const
{
	return *m_transform.GetShared();
}

mmlMatrix<4,4> retro3d::Object::GetNormalizedTransform( void ) const
{
	mmlMatrix<3,3> r = GetRotation();
	for (int i = 0; i < r.Rows; ++i) {
		r[i].Normalize();
	}
	return mmlTransform(r, GetPosition());
}

mmlMatrix<3,3> retro3d::Object::GetRotation( void ) const
{
	return mmlBasis(*m_transform.GetShared());
}

mmlVector<3> retro3d::Object::GetPosition( void ) const
{
	return mmlTranslation(*m_transform.GetShared());
}

mmlMatrix<4,4> retro3d::Object::GetDeltaTransform( void ) const
{
	return m_delta_transform;
}

mmlMatrix<3,3> retro3d::Object::GetDeltaRotation( void ) const
{
	return mmlBasis(m_delta_transform);
}

mmlVector<3> retro3d::Object::GetDeltaPosition( void ) const
{
	return mmlTranslation(m_delta_transform);
}

const std::string &retro3d::Object::GetName( void ) const
{
	return m_name;
}

void retro3d::Object::SetTransform(const mmlMatrix<4,4> &transform)
{
	*m_transform.GetShared() = transform;
}

void retro3d::Object::SetTransform(const mmlMatrix<3,3> &rotation, const mmlVector<3> &position)
{
	SetTransform(mmlTransform(rotation, position));
}

void retro3d::Object::SetRotation(const mmlMatrix<3,3> &rotation)
{
	SetTransform(rotation, GetPosition());
}

void retro3d::Object::SetPosition(const mmlVector<3> &position)
{
	SetTransform(GetRotation(), position);
}

void retro3d::Object::SetName(const std::string &name)
{
	m_name = name;
}

retro3d::Engine *retro3d::Object::GetEngine( void ) const
{
	return m_engine;
}

mtlShared<retro3d::Model> retro3d::Object::GetModel( void ) const
{
	return m_model;
}

retro3d::Collider *retro3d::Object::GetCollider( void ) const
{
	return m_collider;
}

double retro3d::Object::GetTimeScale( void ) const
{
	return m_time_scale;
}

void retro3d::Object::SetTimeScale(double time_scale)
{
	m_time_scale = time_scale;
}

void retro3d::Object::SetModel(mtlShared<retro3d::Model> model)
{
	m_model = model;

	if (m_collider == &m_default_collider) {
		if (m_model.IsNull() == false) {
			m_default_collider.SetLocalAABB(m_model->aabb);
		} else {
			m_default_collider.SetLocalAABB(retro3d::AABB());
		}
	}
}

void retro3d::Object::SetCollider(retro3d::Collider *collider)
{
	if (collider != nullptr) {
		m_collider = collider;
	} else {
		m_collider = &m_default_collider;
		if (m_model.IsNull() == false) {
			m_default_collider.SetLocalAABB(m_model->aabb);
		} else {
			m_default_collider.SetLocalAABB(retro3d::AABB());
		}
	}
	m_collider->AttachTransform(m_transform);
}

void retro3d::Object::EnableGraphics( void )
{
	m_is_visible = true;
}

void retro3d::Object::DisableGraphics( void )
{
	m_is_visible = false;
}

void retro3d::Object::ToggleGraphics( void )
{
	m_is_visible = !m_is_visible;
}

void retro3d::Object::EnableCollisions( void )
{
	m_is_collideable = true;
}

void retro3d::Object::DisableCollisions( void )
{
	m_is_collideable = false;
}

void retro3d::Object::ToggleCollisions( void )
{
	m_is_collideable = !m_is_collideable;
}

void retro3d::Object::EnablePhysics( void )
{
	m_has_physics = true;
}

void retro3d::Object::DisablePhysics( void )
{
	m_has_physics = false;
}

void retro3d::Object::TogglePhysics( void )
{
	m_has_physics = !m_has_physics;
}

void retro3d::Object::EnableDebugAABB( void )
{
	m_debug_aabb = true;
}

void retro3d::Object::DisableDebugAABB( void )
{
	m_debug_aabb = false;
}

void retro3d::Object::ToggleDebugAABB( void )
{
	m_debug_aabb = !m_debug_aabb;
}

void retro3d::Object::ApplyForce(const mmlVector<3> &at, const mmlVector<3> &force)
{
//	if (m_has_physics == true) {
//		m_physics.ApplyForce(at, force);
//	}
}

bool retro3d::Object::IsDestroyed( void ) const
{
	return m_should_destroy == true;
}

bool retro3d::Object::IsActive( void ) const
{
	return !IsDestroyed() && m_is_active > 0;
}

void retro3d::Object::Deactivate( void )
{
	--m_is_active;
}

void retro3d::Object::Activate( void )
{
	m_is_active = mmlMin(1, m_is_active + 1);
}

double retro3d::Object::Time( void ) const
{
	return m_time;
}

double retro3d::Object::DeltaTime( void ) const
{
	return m_delta_time;
}

mtlShared<retro3d::Model> retro3d::Object::DefaultModel( void )
{
	return retro3d::Engine::DefaultModel();
}

mtlShared<retro3d::Texture> retro3d::Object::DefaultTexture( void )
{
	return retro3d::Engine::DefaultTexture();
}
