#ifndef RETRO_SCENE_SYSTEM_H
#define RETRO_SCENE_SYSTEM_H

#include <cstdint>
#include "../retro_system.h"
#include "../components/retro_scene_component.h"
#include "../components/retro_render_component.h"

namespace retro3d
{

retro_system(SceneSystem, retro3d::SceneComponent)
{
private:
	struct Sector;

	struct Neighbor
	{
		retro3d::Frustum  portal;
		Sector           *sector;
	};
	struct Sector
	{
		retro3d::ColliderTree<retro3d::Light>           lights;
		retro3d::ColliderTree<retro3d::RenderComponent> visibles;
		retro3d::AABB                                   aabb;
		mtlList<Neighbor>                               neighbors;
	};

public:
	struct PV_Object
	{
		retro3d::RenderComponent *render_component;
		retro3d::Light           *l_point;
		retro3d::Light           *l_dir;
		retro3d::Light           *l_amb;
	};
	struct PV_Sector
	{
		mtlList<PV_Object> pv_objects; // sorted in
	};
	struct PV_Set
	{
		mtlList<PV_Sector> pv_sectors;
	};

private:
	SceneComponent   *m_current_scene;
	uint32_t          m_num_scenes;
	retro3d::Frustum  m_view_frustum;

protected:
	void OnSpawn(retro3d::SceneComponent &scene) override;
	void OnUpdate(retro3d::SceneComponent &scene) override;
	void OnDestroy(retro3d::SceneComponent &scene) override;

public:
	SceneSystem( void );

//	void ProcessSceneModel(const retro3d::Model &model); // Converts to sectors, converts each sector into BSP tree
//	void SetViewFrustum(const retro3d::Frustum &frustum);
//	const retro3d::Frustum &GetViewFrustum( void ) const;
//	void BuildPotentiallyVisibleSet(SceneSystem::PV_Set &out);
};

}

#endif // RETRO_SCENE_SYSTEM_H
