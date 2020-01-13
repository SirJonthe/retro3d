#ifndef RETRO_SCENE_COMPONENT_H
#define RETRO_SCENE_COMPONENT_H

#include "../retro_component.h"
#include "../../graphics/retro_model.h"
#include "../../graphics/retro_light.h"
#include "../../physics/retro_collider_tree.h"

namespace retro3d
{

retro_component(SceneComponent)
{
private:
	// divvied up into sectors
	// each sector contains info
	// a stage for dynamic game objects
	// a stage for static game objects
	// a stage for lights

protected:
	void OnSpawn( void ) override;

public:
	SceneComponent( void );
};

}

#endif // RETRO_SCENE_COMPONENT_H
