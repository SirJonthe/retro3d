#ifndef RETRO3D_COLLIDER_COMPONENT_H
#define RETRO3D_COLLIDER_COMPONENT_H

#include "../../physics/retro_collider.h"
#include "../retro_component.h"

namespace retro3d
{

retro_component(ColliderComponent)
{
private:
	mtlShared<retro3d::Collider> m_collider;
	uint64_t                     m_filter_flags;
	bool                         m_is_static;
	bool                         m_reinsert;

protected:
	void OnSpawn( void );

public:
	ColliderComponent( void );

	Collider &GetCollider( void );
	const Collider &GetCollider( void ) const;
	template < typename collider_t >
	collider_t *CreateCollider( void );

	uint64_t GetFilterFlags( void ) const;
	bool     GetFilterFlag(uint32_t flag_index) const;
	void     SetFilterFlags(uint64_t filter_flags);
	void     SetFilterFlag(uint32_t flag_index, bool state);

	bool IsStatic( void ) const;
	void ToggleStatic( void );

	bool ShouldReinsert( void ) const;
	void ToggleReinsert( void );
};

}

template < typename collider_t >
collider_t *retro3d::ColliderComponent::CreateCollider( void )
{
	mtlShared<collider_t> n = mtlShared<collider_t>::Create();
	m_collider = n;
	m_reinsert = true;
	return n.GetShared();
}

#endif
