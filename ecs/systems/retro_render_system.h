#ifndef RETRO_RENDER_SYSTEM_H
#define RETRO_RENDER_SYSTEM_H

#include "../../frontend/retro_render_device.h"
#include "../retro_system.h"
#include "../components/retro_render_component.h"
#include "../../physics/retro_collider_tree.h"

// NOTE: CONSIDER SPLITTING UP GEOMETRY SYSTEM FROM RENDER SYSTEM
// Geometry system polls occlusion queries and inserts visible geometry into scene (at appropriate LOD).
// Render system simply traverses scene/scenes and renders contents (in appropriate order).

namespace retro3d
{

retro_system(RenderSystem, retro3d::RenderComponent)
{
private:
	retro3d::ColliderTree<retro3d::RenderComponent> m_view_hierarchy;
	retro3d::ColliderTree<retro3d::Light>           m_light_hierarchy;
	retro3d::Frustum                                m_view_frustum;
	// Shared pointer to SCENE data structure

protected:
	void OnSpawn(retro3d::RenderComponent &c) override;
	void OnUpdate(retro3d::RenderComponent &c) override;
	void OnDestroy(retro3d::RenderComponent &c) override;
	void OnUpdate( void ) override;

public:
	RenderSystem( void );
};

}

#endif // RETRO_RENDER_SYSTEM_H
