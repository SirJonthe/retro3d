#ifndef RETRO_LIGHT_COMPONENT_H
#define RETRO_LIGHT_COMPONENT_H

#include "../../graphics/retro_light.h"
#include "../retro_component.h"

namespace retro3d
{

retro_component(LightComponent), private retro3d::Light
{
private:
	retro3d::AABBCollider m_collider;
	bool                  m_is_static;
	bool                  m_reinsert;
	uint64_t              m_filter_flags;

protected:
	void OnSpawn( void ) override;

public:
	LightComponent( void );

	bool IsStatic( void ) const;
	void ToggleStatic( void );

	uint64_t GetFilterFlags( void ) const;
	void SetFilterFlags(uint64_t filter_flags);

	bool ShouldReinsert( void ) const;
	void ToggleReinsert( void );

	retro3d::AABBCollider &GetActivationBounds( void );
};

}

#endif // RETRO_LIGHT_COMPONENT_H
