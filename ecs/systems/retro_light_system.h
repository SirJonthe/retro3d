#ifndef RETRO_LIGHT_SYSTEM_H
#define RETRO_LIGHT_SYSTEM_H

#include "../retro_system.h"
#include "../components/retro_light_component.h"
#include "../../physics/retro_collider_tree.h"

namespace retro3d
{

retro_system(LightSystem, retro3d::LightComponent)
{
private:
	retro3d::ColliderTree<retro3d::Entity> m_light_tree;

protected:
	void OnSpawn(retro3d::LightComponent &c) override;
	void OnUpdate(retro3d::LightComponent &c) override;
	void OnDestroy(retro3d::LightComponent &c) override;

public:
	LightSystem( void );

	void Debug_PrintTree( void ) const;
	void Debug_RenderTree( void );

	void UpdateLightmap( void );
};

}

#endif // RETRO_LIGHT_SYSTEM_H
