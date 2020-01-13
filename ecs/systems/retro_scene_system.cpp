#include "retro_scene_system.h"

void retro3d::SceneSystem::OnSpawn(retro3d::SceneComponent &scene)
{
	++m_num_scenes;
	if (m_num_scenes > 1) {
		std::cout << "[Warning] Multiple SceneComponents in SceneSystem." << std::endl;
	}
	if (m_current_scene == nullptr) {
		m_current_scene = &scene;

		// TODO: split geometry into sectors
		// Idea:
			// Generate a convex hull (C) based on the input hull (I)
			// If there are points that are not included on C (i.e. points inside C) do:
				// >= 3 points inside C: use 3 points to generate a splitting plane and split I (or maybe C?).
				// < 3 points inside C: use available points inside C and together point(s) on C to generate splitting plane and split I (or maybe C?)
			// Apply algorithm recursively to the two new parts of I.
	}
}

void retro3d::SceneSystem::OnUpdate(retro3d::SceneComponent &scene)
{
	if (m_current_scene == nullptr) {
		m_current_scene = &scene;
	}
}

void retro3d::SceneSystem::OnDestroy(retro3d::SceneComponent &scene)
{
	--m_num_scenes;
	if (m_current_scene == &scene) {
		m_current_scene = nullptr;
	}
}

retro3d::SceneSystem::SceneSystem( void ) : mtlInherit(this), m_current_scene(nullptr), m_num_scenes(0)
{}
