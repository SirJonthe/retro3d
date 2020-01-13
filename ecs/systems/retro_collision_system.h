#ifndef RETRO_COLLISION_SYSTEM_H
#define RETRO_COLLISION_SYSTEM_H

#include "../retro_system.h"
#include "../components/retro_collider_component.h"
#include "../../physics/retro_collider_tree.h"
#include "../../physics/retro_physics.h"

namespace retro3d
{

retro_system(CollisionSystem, retro3d::ColliderComponent)
{
private:
	retro3d::ColliderTree<retro3d::Entity> m_collider_tree;

protected:
	void OnSpawn(retro3d::ColliderComponent &c) override;
	void OnUpdate(retro3d::ColliderComponent &c) override;
	void OnDestroy(retro3d::ColliderComponent &c) override;
	void OnUpdate( void ) override;

private:
	void ResolveContactA(const retro3d::ColliderTree<retro3d::Entity>::Contact &contact_pair, const retro3d::Collider::Contact &contact_info) const;

public:
	CollisionSystem( void );

	retro3d::Entity *CastRay(const retro3d::Ray &world_ray, uint64_t filter_flags, retro3d::Ray::Contact *contact_info = nullptr) const;
	bool Contains(const mmlVector<3> &world_point, uint64_t filter_flags, mtlList<retro3d::Entity*> *colliders = nullptr);

	// TODO: Physics commands that only apply to objects with physics components
	retro3d::Entity *ApplyForce(const retro3d::Ray &world_force, float force_magnitude, uint64_t filter_flags, retro3d::Ray::Contact *contact_info = nullptr);

	void Debug_PrintTree( void ) const;
	void Debug_RenderTree( void );
	const retro3d::ColliderTree<retro3d::Entity>::Contacts &Debug_GetPotentialContacts( void ) const;
};

}

#endif // RETRO_COLLISION_SYSTEM_H
