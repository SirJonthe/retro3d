#ifndef RETRO_SCENE_H
#define RETRO_SCENE_H

#include "retro_light.h"
#include "../retro_object.h"
#include "../common/retro_geom.h"
#include "../common/retro_transform.h"
#include "../common/MiniLib/MTL/mtlPointer.h"

namespace retro3d
{

class Scene
{
private:
	struct DispalyObject
	{
		retro3d::Model         *model; // use BSP model instead
		mtlShared< Transform >  transform;
		// how to render (point, line, triangle)
		// override materials
		// disregard light
	};

	struct Sector
	{
		tiny3d::Array< Sector* >         connected; // sectors connected to the current sector.
		tiny3d::Array< Sector* >         visible;   // sectors visible from the current sector.
		tiny3d::Array< DispalyObject* >  objects;   // list of all objects in the currect sector.
		tiny3d::Array< retro3d::Light >  lights;    // every sector has its own list of lights in the entire scene (can be used to simplify lights far away from camera)
		retro3d::AABB                    aabb;      // the bounding box of the sector for fast culling against camera.
	};

	// BSP model of the scene
	// list of sectors
	// list of lights
	// camera
	// the renderer basically only renders a scene, and traverses scene structure, rendering scene geometry and Objects as it goes using the light information in the leaf (not the)

};



}

#endif // RETRO_SCENE_H
