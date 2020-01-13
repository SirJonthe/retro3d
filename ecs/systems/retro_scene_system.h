#ifndef RETRO_SCENE_SYSTEM_H
#define RETRO_SCENE_SYSTEM_H

#include <cstdint>
#include "../retro_system.h"
#include "../components/retro_scene_component.h"

namespace retro3d
{

retro_system(SceneSystem, retro3d::SceneComponent)
{
private:
	SceneComponent *m_current_scene;
	uint32_t        m_num_scenes;

protected:
	void OnSpawn(retro3d::SceneComponent &scene) override;
	void OnUpdate(retro3d::SceneComponent &scene) override;
	void OnDestroy(retro3d::SceneComponent &scene) override;

public:
	SceneSystem( void );
};

}

#endif // RETRO_SCENE_SYSTEM_H
