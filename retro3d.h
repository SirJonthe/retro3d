#ifndef RETRO3D_H
#define RETRO3D_H

#include <unordered_map>
#include "common/MiniLib/MTL/mtlList.h"
#include "common/MiniLib/MTL/mtlPointer.h"
#include "common/MiniLib/MML/mmlRandom.h"
#include "common/retro_time.h"
#include "frontend/retro_render_device.h"
#include "ecs/retro_component.h"
#include "ecs/retro_system.h"
#include "graphics/retro_camera.h"
#include "serial/retro_import.h"
#include "frontend/retro_sound_device.h"
#include "frontend/retro_input_device.h"
#include "frontend/retro_video_device.h"

namespace retro3d
{

class Engine
{
private:
	typedef std::unordered_map<uint64_t, ISystem*>                 Systems;
	typedef std::unordered_map<uint64_t, retro3d::Component*>      ComponentClass; // ALL components in this map are GUARANTEED being the SAME type - uint64_t = Component class ID
	typedef std::unordered_map<uint64_t, ComponentClass>           Components; // all components - uint64_t = Entity instance ID
	typedef std::unordered_map<uint64_t, ComponentClass::iterator> ComponentClassIters;
	typedef std::unordered_map<uint64_t, ComponentClassIters>      ComponentIters;
	// [component_class_id][entity_instance_id] = access component of individual entity (if existing)
	// [component_class_id] = access all components of certain type

private:
	retro3d::RenderDevice       *m_renderer;
	retro3d::SoundDevice        *m_sound;
	retro3d::InputDevice        *m_input;
	retro3d::VideoDevice        *m_video;
	const retro3d::Camera       *m_camera;
	retro3d::Camera              m_default_camera;
	Components                   m_components_pending_init;
	Components                   m_components;
	mtlList< retro3d::Entity* >  m_entities;
	Systems                      m_systems;
	Systems                      m_systems_pending_init;
	retro3d::Time                m_frame_start_time;
	retro3d::Time                m_frame_time;
	retro3d::RealTimeTimer       m_real_timer; // 1 s on clock = 1 s in real life.
	retro3d::RealTimeTimer       m_sim_timer; // 1 s on clock = 1 s in real life, but may scale time down if engine can not keep target Hz.
	retro3d::RealTimeTimer       m_game_timer; // User scaled time, 1 s on clock = 1/m_time_scale s in real life.
	retro3d::Time                m_real_time;
	retro3d::Time                m_sim_time;
	retro3d::Time                m_game_time;
	uint64_t                     m_frame;
	mmlRandom                    m_rand;
	double                       m_delta_time;
	retro3d::Time                m_min_delta_time;
	retro3d::Time                m_max_delta_time;
	bool                         m_is_running;
	bool                         m_quit;

private:
	static void CreateBaseModels( void );

private:
	void SetupTimers( void );
	void AddRequiredSystems( void );
	void Reset( void );
	void UpdateDevices( void );
	void InitSystems( void );
	void InitComponents( void );
	void TickComponents( void );
	void TickSystems( void );
	void TickEntities( void );
	void DestroySystems( void );
	void DestroyComponents( void );
	void DestroyEntities( void );
	void DetectTermination( void );
	void TickTime( void );
	void Tick( void );
	void Cleanup( void );

	template < typename component_t > component_t       *GetFinishedComponent(retro3d::Entity &e);
	template < typename component_t > const component_t *GetFinishedComponent(const retro3d::Entity &e) const;
	template < typename component_t > component_t       *GetPendingComponent(retro3d::Entity &e);
	template < typename component_t > const component_t *GetPendingComponent(const retro3d::Entity &e) const;

	template < typename system_t > system_t       *GetFinishedSystem( void );
	template < typename system_t > const system_t *GetFinishedSystem( void ) const;
	template < typename system_t > system_t       *GetPendingSystem( void );
	template < typename system_t > const system_t *GetPendingSystem( void ) const;

public:
	// Construction.
	Engine( void );
	~Engine( void );

	// Creates a render device of the specified type.
	template < typename render_dev_t > void CreateRenderDevice( void );

	// Creates a sound device of the specified type.
	template < typename sound_dev_t > void CreateSoundDevice( void );

	// Creates an input device of the specified type.
	template < typename input_dev_t > void CreateInputDevice( void );

	// Creates a video device of the specified type.
	template < typename video_dev_t > void CreateVideoDevice( void );

	// Sets a reference to be used the point of view for rendering.
	void SetCamera(const retro3d::Camera *camera);

	// Resets the point of view to the internal (default) camera.
	void DefaultCamera( void );

	// Returns the reference to the currently used camera.
	const retro3d::Camera *GetCamera( void ) const;

	// Returns the renderer.
	retro3d::RenderDevice       *GetRenderer( void );
	const retro3d::RenderDevice *GetRenderer( void ) const;

	// Returns the sound device.
	retro3d::SoundDevice       *GetSoundDevice( void );
	const retro3d::SoundDevice *GetSoundDevice( void ) const;

	// Returns the input device.
	retro3d::InputDevice       *GetInput( void );
	const retro3d::InputDevice *GetInput( void ) const;

	// Returns the video device.
	retro3d::VideoDevice       *GetVideo( void );
	const retro3d::VideoDevice *GetVideo( void ) const;

	// Set the maximum updates that the engine can do. If an update cycle finishes early it rests for the remaining time.
	void SetMaxUpdateFrequency(uint32_t max_hz);

	// Set the maximum simulation time deltas. Does not prevent that DeltaTime returns a larger delta is the user has scaled the time further.
	void SetMaxSimulationTimeDelta(retro3d::Time time_delta);

	// [DEPRECATED] Returns the scaled game time elapsed since Run was called.
//	double   Time( void ) const; // [DEPRECATED] Replaced by RealTime

	// Returns unscaled real (world) time since Run was called.
	retro3d::Time RealTime( void ) const;

	// Returns scaled real (world) time since Run was called, but never ticks more than max time delta.
	retro3d::Time SimulationTime( void ) const;

	// Returns scaled game since Run was called based on simulation time and user-defined time scale.
	retro3d::Time GameTime( void ) const;

	// Returns time of specified type.
	retro3d::Time Time(retro3d::TimerType type) const;

	// Returns the time (excluding sleep) the engine took to loop.
	retro3d::Time GetFrameTime( void ) const;

	// Returns the time (including sleep) the engine took to loop.
	retro3d::Time GetLockedFrameTime( void ) const;

	// Returns the instantaneous number of frames per second that the engine is rendering at, discounting thread sleep. For an accurate count the user may want to average the function over time.
	float GetFramesPerSecond( void ) const;

	// Returns the instantaneous number of frames per second that the engine is rendering at. For an accurate count the user may want to average the function over time.
	float GetLockedFramesPerSecond( void ) const;

	// Returns the index of the frame currently being rendered.
	uint64_t GetFrameCount( void ) const;

	// Create a child timer that scales with the max time delta *at most*.
	retro3d::RealTimeTimer CreateSimulationTimer(uint32_t num_ticks = 1, retro3d::Time over_time = 1_s) const;

	// Create a child timer that scaled with the user defined time scale.
	retro3d::RealTimeTimer CreateGameTimer(uint32_t num_ticks = 1, retro3d::Time over_time = 1_s) const;

	// Create a child timer of specified type.
	retro3d::RealTimeTimer CreateTimer(retro3d::TimerType, uint32_t num_ticks = 1, retro3d::Time over_time = 1_s) const;

	// NOTE: There is no need for a CreateRealTimer since that is what the RealTimeTimer struct constructor already does when set to 1 tick per second.

	// Returns the number of entities currently in the engine.
	uint32_t GetEntityCount( void ) const;

	// Returns the scale used for the game timer.
	double GetTimeScale( void ) const;

	// Sets the scale used for the game timer. Accelerates or decelerates time (speedup or slo-mo).
	void SetTimeScale(double time_scale);

	// Returns a random 32 bit unsigned integer.
	uint32_t GetRandomUint( void );

	// Returns a random 32 bit unsigned integer in the range from min up to, but not including, max.
	uint32_t GetRandomUint(uint32_t min, uint32_t max);

	// Returns a random 32 bit signed integer.
	int32_t GetRandomInt( void );

	// Returns a random 32 bit signed integer in the range from min up to, but not including, max.
	int32_t GetRandomInt(int min, int max);

	// Returns a random 32 bit floating point number in the range from 0 up to, but not including, 1.
	float GetRandomFloat( void );

	// Returns a random 32 bit floating point number in the range from min up to, but not including, max.
	float GetRandomFloat(float min, float max);

	// Starts the main engine loop, and quits only if a entity, system, or component calls the engine Quit function or if there are no valid objects for the engine to work on.
	void Run( void );

	// Breaks out of the main engine loop.
	void Quit( void );

	// Spawn an entity of the given type.
	template < typename entity_t = retro3d::Entity, typename... Args > entity_t *SpawnEntity(Args&&... args);

	// Spawn an entity by C++ class name or prefab name. The user must register names and schematics manually.
	retro3d::Entity *SpawnEntity(const std::string &name);

	// Find entities by tag.
	bool FindEntityByTag(const std::string &tag, mtlList<retro3d::Entity*> &results, bool search_inactive = false);

	// The mode in which to look for entities by filter.
	enum FilterSearchMode
	{
		FILTERSEARCH_EXACT,    // Only return results with exact match.
		FILTERSEARCH_ANY,      // Return results where at least one of the flags match.
		FILTERSEARCH_INCLUSIVE // Return results where all enabled flags match.
	};

	// Find entities by filter given a filter search mode.
	bool FindEntityByFilter(uint64_t filter_flags, mtlList<retro3d::Entity*> &results, FilterSearchMode filter_search = FILTERSEARCH_ANY, bool search_inactive = false);

	// Find entity by UUID
	retro3d::Entity *FindEntityByUUID(uint64_t uuid, bool search_inactive = false);

	// Adds a component to an entity and returns a reference to it, including its actual type.
	template < typename component_t, typename... Args > component_t *AddComponent(retro3d::Entity &e, Args&&... args);

	// Removes a component from an entity regardless if it has it or not.
	template < typename component_t > void RemoveComponent(retro3d::Entity &e);

	// Returns a component from an entity, including its actual type. Null is returned when the entity does not have the requested component.
	template < typename component_t > component_t       *GetComponent(retro3d::Entity &e);
	template < typename component_t > const component_t *GetComponent(const retro3d::Entity &e) const;

	// Adds a system to the engine and returns a reference to it, including its actual type.
	template < typename system_t, typename... Args > system_t *AddSystem(Args&&... args);

	// Removes a system from the engine regardless if it has it or not.
	template < typename system_t > void RemoveSystem( void );

	// Returns a system from the engine, including its actual type. Null is returned when the engine does not have the requested system.
	template < typename system_t > system_t       *GetSystem( void );
	template < typename system_t > const system_t *GetSystem( void ) const;

public:
	// Return a reference to a default model mainly used to display errors.
	static mtlShared<retro3d::Model> DefaultModel( void );

	// Returns a reference to a texture mainly used to display errors.
	static mtlShared<retro3d::Texture> DefaultTexture( void );
};

}

template < typename render_dev_t >
void retro3d::Engine::CreateRenderDevice( void )
{
	// TODO: Do a "hand-over" between old render device and new render device so that the state is preserved
	delete m_renderer;
	m_renderer = new render_dev_t;
	m_renderer->m_engine = this;
}

template < typename sound_dev_t >
void retro3d::Engine::CreateSoundDevice( void )
{
	// TODO: Do a "hand-over" between old sound device and new sound device so that the state is preserved
	delete m_sound;
	m_sound = new sound_dev_t;
	m_sound->m_engine = this;
}

template < typename input_dev_t >
void retro3d::Engine::CreateInputDevice( void )
{
	// TODO: Do a "hand-over" between old input device and new input device so that the state is preserved
	delete m_input;
	m_input = new input_dev_t;
	m_input->m_engine = this;
}

template < typename video_dev_t >
void retro3d::Engine::CreateVideoDevice( void )
{
	delete m_video;
	m_video = new video_dev_t;
	m_video->m_engine = this;
}

#include "ecs/retro_entity.h"

template < typename component_t >
component_t *retro3d::Engine::GetFinishedComponent(retro3d::Entity &e)
{
	Components::iterator i = m_components.find(component_t::GetClassType());
	if (i != m_components.end()) {
		ComponentClass::iterator j = i->second.find(e.GetUUID());
		return j != i->second.end() ? mtlCast<component_t>(j->second) : nullptr;
	}
	return nullptr;
}

template < typename component_t >
const component_t *retro3d::Engine::GetFinishedComponent(const retro3d::Entity &e) const
{
	Components::const_iterator i = m_components.find(component_t::GetClassType());
	if (i != m_components.end()) {
		ComponentClass::const_iterator j = i->second.find(e.GetUUID());
		return j != i->second.end() ? mtlCast<component_t>(j->second) : nullptr;
	}
	return nullptr;
}

template < typename component_t >
component_t *retro3d::Engine::GetPendingComponent(retro3d::Entity &e)
{
	Components::iterator i = m_components_pending_init.find(component_t::GetClassType());
	if (i != m_components_pending_init.end()) {
		ComponentClass::iterator j = i->second.find(e.GetUUID());
		return j != i->second.end() ? mtlCast<component_t>(j->second) : nullptr;
	}
	return nullptr;
}

template < typename component_t >
const component_t *retro3d::Engine::GetPendingComponent(const retro3d::Entity &e) const
{
	Components::const_iterator i = m_components_pending_init.find(component_t::GetClassType());
	if (i != m_components_pending_init.end()) {
		ComponentClass::const_iterator j = i->second.find(e.GetUUID());
		return j != i->second.end() ? mtlCast<component_t>(j->second) : nullptr;
	}
	return nullptr;
}

template < typename system_t >
system_t *retro3d::Engine::GetFinishedSystem( void )
{
	Systems::iterator i = m_systems.find(system_t::GetClassType());
	return i != m_systems.end() ? mtlCast<system_t>(i->second) : nullptr;
}

template < typename system_t >
const system_t *retro3d::Engine::GetFinishedSystem( void ) const
{
	Systems::const_iterator i = m_systems.find(system_t::GetClassType());
	return i != m_systems.end() ? mtlCast<system_t>(i->second) : nullptr;
}

template < typename system_t >
system_t *retro3d::Engine::GetPendingSystem( void )
{
	Systems::iterator i = m_systems_pending_init.find(system_t::GetClassType());
	return i != m_systems_pending_init.end() ? mtlCast<system_t>(i->second) : nullptr;
}

template < typename system_t >
const system_t *retro3d::Engine::GetPendingSystem( void ) const
{
	Systems::const_iterator i = m_systems_pending_init.find(system_t::GetClassType());
	return i != m_systems_pending_init.end() ? mtlCast<system_t>(i->second) : nullptr;
}

template < typename entity_t, typename... Args >
entity_t *retro3d::Engine::SpawnEntity(Args&&... args)
{
	entity_t *e = new entity_t(std::forward<Args>(args)...);
	e->m_engine = this;
	e->m_real_time_spawn = m_real_time;
	e->m_sim_time_spawn = m_sim_time;
	e->m_game_timer.SetParent(&m_game_timer);
//	e->m_delta_time = m_delta_time * e->m_time_scale;
	e->m_delta_time = m_delta_time;
	m_entities.AddLast(e);
	dynamic_cast<retro3d::Entity*>(e)->OnSpawn(); // If entity_t != Entity, then Engine does not have access to protected OnSpawn, so use virtual function in base instead.
	return e;
}

template < typename component_t, typename... Args >
component_t *retro3d::Engine::AddComponent(retro3d::Entity &e, Args&&... args)
{
	component_t *c = GetComponent<component_t>(e);
	if (e.m_should_destroy == false && c == nullptr) {
		c = new component_t(std::forward<Args>(args)...);
		c->m_object = &e;
		m_components_pending_init[component_t::GetClassType()][e.GetUUID()] = c;
	}
	return c;
}

template < typename component_t >
void retro3d::Engine::RemoveComponent(retro3d::Entity &e)
{
	component_t *c = GetComponent<component_t>(e);
	if (c != nullptr) {
		c->Destroy();
	}
}

template < typename component_t >
component_t *retro3d::Engine::GetComponent(retro3d::Entity &e)
{
	component_t *c = GetFinishedComponent<component_t>(e);
	return c != nullptr ? c : GetPendingComponent<component_t>(e);
}

template < typename component_t >
const component_t *retro3d::Engine::GetComponent(const retro3d::Entity &e) const
{
	const component_t *c = GetFinishedComponent<component_t>(e);
	return c != nullptr ? c : GetPendingComponent<component_t>(e);
}

template < typename system_t, typename... Args >
system_t *retro3d::Engine::AddSystem(Args&&... args)
{
	system_t *s = GetSystem<system_t>();
	if (s == nullptr) {
		s = new system_t(std::forward<Args>(args)...);
		s->m_engine = this;
		m_systems_pending_init[system_t::GetClassType()] = s;
	}
	return s;
}

template < typename system_t >
void retro3d::Engine::RemoveSystem( void )
{
	system_t *s = GetSystem<system_t>();
	if (s != nullptr) {
		s->Destroy();
	}
}

template < typename system_t >
system_t *retro3d::Engine::GetSystem( void )
{
	system_t *s = GetFinishedSystem<system_t>();
	return s != nullptr ? s : GetPendingSystem<system_t>();
}

template < typename system_t >
const system_t *retro3d::Engine::GetSystem( void ) const
{
	const system_t *s = GetFinishedSystem<system_t>();
	return s != nullptr ? s : GetPendingSystem<system_t>();
}

#endif
