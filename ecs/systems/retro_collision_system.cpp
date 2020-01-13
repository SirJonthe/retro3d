#include "retro_collision_system.h"
#include "../../ecs/components/retro_physics_component.h"
#include "../../retro3d.h"

void retro3d::CollisionSystem::OnSpawn(retro3d::ColliderComponent &c)
{
	// Add component to collider tree
	m_collider_tree.Insert(c.GetObject(), &c.GetCollider(), c.IsStatic(), c.GetFilterFlags());
	if (c.ShouldReinsert() == true) {
		c.ToggleReinsert();
	}
}

void retro3d::CollisionSystem::OnUpdate(retro3d::ColliderComponent &c)
{
	// Check if the collider needs to update its representation inside the tree
	if (c.ShouldReinsert() == true) {
		m_collider_tree.Remove(c.GetObject());
		m_collider_tree.Insert(c.GetObject(), &c.GetCollider(), c.IsStatic(), c.GetFilterFlags());
		c.ToggleReinsert();
	}
}

void retro3d::CollisionSystem::OnDestroy(retro3d::ColliderComponent &c)
{
	// Remove component from collider tree
	m_collider_tree.Remove(c.GetObject());
}

void SwapCollisionInfo(retro3d::ColliderTree<retro3d::Entity>::Contact &c, retro3d::Collider::Contact &c_info)
{
	mmlSwap(c.a, c.b);
	mmlSwap(c.a_is_static, c.b_is_static);
	mmlSwap(c.a_filter_flags, c.b_filter_flags);
	c_info.normal = -c_info.normal;
}

void retro3d::CollisionSystem::OnUpdate( void )
{
	// Detect actual collisions from a list of potential collisions and resolve
	auto *c_iter = m_collider_tree.GetPotentialContacts().GetFirst();
	while (c_iter != nullptr) {
		auto c = c_iter->GetItem();
		retro3d::Collider::Contact c_info;
		if (c.a.collider->IsColliding(*c.b.collider, &c_info) == true) {
			ResolveContactA(c, c_info);
			SwapCollisionInfo(c, c_info); // Collider A and B have switched place, and collsion normal is flipped.
			ResolveContactA(c, c_info);
		}
		c_iter = c_iter->GetNext();
	}

	// Update tree
	m_collider_tree.Update();
}

void retro3d::CollisionSystem::ResolveContactA(const retro3d::ColliderTree<retro3d::Entity>::Contact &contact_pair, const retro3d::Collider::Contact &contact_info) const
{
	if (contact_pair.a_is_static == false) {
		// ONLY resolve and apply forces to A.
		// B is read-only.

		retro3d::PhysicsComponent       *ap = contact_pair.a.user_data->GetComponent<retro3d::PhysicsComponent>();
		const retro3d::PhysicsComponent *bp = contact_pair.b.user_data->GetComponent<retro3d::PhysicsComponent>();

		float resolve_ratio = 1.0f;
		if (ap != nullptr) {
			mmlVector<3> mov = ap->GetMomentum(contact_info.point);
			if (contact_pair.b_is_static == false && bp != nullptr) {
				resolve_ratio = ap->GetMass() / (ap->GetMass() + bp->GetMass());
				mov -= bp->GetMomentum(contact_info.point);
			}
			ap->ApplyForce(contact_info.point, contact_info.normal, mov.Len());
		}

		contact_pair.a.collider->GetTransform().SetPosition(contact_pair.a.collider->GetTransform().GetPosition() + contact_info.normal * contact_info.depth * resolve_ratio);
	}
}

retro3d::CollisionSystem::CollisionSystem( void ) : mtlInherit(this)
{}

retro3d::Entity *retro3d::CollisionSystem::CastRay(const retro3d::Ray &world_ray, uint64_t filter_flags, retro3d::Ray::Contact *contact_info) const
{
	return m_collider_tree.CastRay(world_ray, filter_flags, contact_info);
}

bool retro3d::CollisionSystem::Contains(const mmlVector<3> &world_point, uint64_t filter_flags, mtlList<retro3d::Entity*> *colliders)
{
	return m_collider_tree.Contains(world_point, filter_flags, colliders);
}

retro3d::Entity *retro3d::CollisionSystem::ApplyForce(const retro3d::Ray &world_force, float force_magnitude, uint64_t filter_flags, retro3d::Ray::Contact *contact_info)
{
	retro3d::Ray::Contact temp;
	retro3d::Ray::Contact *ct = contact_info != nullptr ? contact_info : &temp;
	retro3d::Entity *e = CastRay(world_force, filter_flags, ct);
	if (e != nullptr && e->GetComponent<retro3d::ColliderComponent>()->IsStatic() == false) {
		retro3d::PhysicsComponent *pc = e->GetComponent<retro3d::PhysicsComponent>();
		if (pc != nullptr) {
			pc->ApplyForce(world_force.origin, world_force.direction, force_magnitude);
		}
	}
	return e;
}

void retro3d::CollisionSystem::Debug_PrintTree( void ) const
{
	m_collider_tree.Debug_PrintTree();
}

void retro3d::CollisionSystem::Debug_RenderTree( void )
{
	std::queue<retro3d::ColliderTree<retro3d::Entity>::Debug_AABB> q = m_collider_tree.Debug_GetTree();
	while (q.empty() == false) {
		mmlVector<3> c = mmlVector<3>::Fill(0.0f);
		switch (q.front().type) {
		case retro3d::ColliderTree<retro3d::Entity>::Debug_AABB::Node_Dynamic:
			c[2] = 1.0f;
			break;
		case retro3d::ColliderTree<retro3d::Entity>::Debug_AABB::Node_Static:
			c[0] = 1.0f;
			c[1] = 1.0f;
			break;
		case retro3d::ColliderTree<retro3d::Entity>::Debug_AABB::Leaf_Dynamic:
			c[1] = 1.0f;
			break;
		case retro3d::ColliderTree<retro3d::Entity>::Debug_AABB::Leaf_Static:
			c[0] = 1.0f;
			break;
		}
		GetEngine()->GetRenderer()->RenderAABB(q.front(), mmlMatrix<4,4>::Identity(), c);
		q.pop();
	}
}

const retro3d::ColliderTree<retro3d::Entity>::Contacts &retro3d::CollisionSystem::Debug_GetPotentialContacts( void ) const
{
	return m_collider_tree.GetPotentialContacts();
}
