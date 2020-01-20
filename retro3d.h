#ifndef RETRO3D_H
#define RETRO3D_H

#include <unordered_map>
#include "common/MiniLib/MTL/mtlList.h"
#include "common/MiniLib/MTL/mtlPointer.h"
#include "common/MiniLib/MML/mmlRandom.h"
#include "common/retro_timer.h"
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
	typedef std::unordered_map<uint64_t, ISystem*>            Systems;
	typedef std::unordered_map<uint64_t, retro3d::Component*> ComponentClass; // ALL components in this map are GUARANTEED being the SAME type - uint64_t = Component class ID
	typedef std::unordered_map<uint64_t, ComponentClass>      Components; // all components - uint64_t = Entity instance ID
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
	retro3d::Timer               m_timer;
	uint64_t                     m_frame;
	mmlRandom                    m_rand;
	double                       m_time; // should not be floating point...
	double                       m_delta_time;
	double                       m_min_delta_time;
	double                       m_max_delta_time;
	double                       m_time_scale;
	bool                         m_is_running;
	bool                         m_quit;

private:
	static void CreateBaseModels( void );

private:
	void AddRequiredSystems( void );
	void Reset( void );
	void UpdateDevices( void );
	void TickEntityTime( void );
	void InitSystems( void );
	void InitComponents( void );
	void TickComponents( void );
	void TickSystems( void );
	void DestroySystems( void );
	void DestroyComponents( void );
	void DestroyEntities( void );
	void TickGameTime( void );
	void DetectTermination( void );
	void Sleep( void );
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
	Engine( void );
	~Engine( void );

	template < typename render_dev_t > void CreateRenderDevice( void );
	template < typename sound_dev_t >  void CreateSoundDevice( void );
	template < typename input_dev_t >  void CreateInputDevice( void );
	template < typename video_dev_t >  void CreateVideoDevice( void );

	void                   SetCamera(const retro3d::Camera *camera);
	void                   DefaultCamera( void );
	const retro3d::Camera *GetCamera( void ) const;

	retro3d::RenderDevice       *GetRenderer( void );
	const retro3d::RenderDevice *GetRenderer( void ) const;
	retro3d::SoundDevice        *GetSoundDevice( void );
	const retro3d::SoundDevice  *GetSoundDevice( void ) const;
	retro3d::InputDevice        *GetInput( void );
	const retro3d::InputDevice  *GetInput( void ) const;
	retro3d::VideoDevice        *GetVideo( void );
	const retro3d::VideoDevice  *GetVideo( void ) const;

	double   Time( void ) const;
	double   DeltaTime( void ) const;
	uint64_t ProgramTime( void ) const;
	float    GetFramesPerSecond( void ) const;
	uint64_t GetFrameCount( void ) const;

	uint32_t GetEntityCount( void ) const;

	double GetTimeScale( void ) const;
	void   SetTimeScale(double time_scale);

	uint32_t GetRandomUint( void );
	uint32_t GetRandomUint(uint32_t min, uint32_t max);
	int32_t  GetRandomInt( void );
	int32_t  GetRandomInt(int min, int max);
	float    GetRandomFloat( void );
	float    GetRandomFloat(float min, float max);

	void Run( void );
	void Quit( void );

	template < typename entity_t = retro3d::Entity > entity_t *SpawnEntity( void );
	bool FindEntity(const std::string &name, mtlList<retro3d::Entity*> &results, bool search_inactive = false);

	template < typename component_t > component_t       *AddComponent(retro3d::Entity &e);
	template < typename component_t > void               RemoveComponent(retro3d::Entity &e);
	template < typename component_t > component_t       *GetComponent(retro3d::Entity &e);
	template < typename component_t > const component_t *GetComponent(const retro3d::Entity &e) const;

	template < typename system_t > system_t       *AddSystem( void );
	template < typename system_t > void            RemoveSystem( void );
	template < typename system_t > system_t       *GetSystem( void );
	template < typename system_t > const system_t *GetSystem( void ) const;

public:
	static mtlShared<retro3d::Model>   DefaultModel( void );
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
		ComponentClass::iterator j = i->second.find(e.GetInstanceID());
		return j != i->second.end() ? mtlCast<component_t>(j->second) : nullptr;
	}
	return nullptr;
}

template < typename component_t >
const component_t *retro3d::Engine::GetFinishedComponent(const retro3d::Entity &e) const
{
	Components::const_iterator i = m_components.find(component_t::GetClassType());
	if (i != m_components.end()) {
		ComponentClass::const_iterator j = i->second.find(e.GetInstanceID());
		return j != i->second.end() ? mtlCast<component_t>(j->second) : nullptr;
	}
	return nullptr;
}

template < typename component_t >
component_t *retro3d::Engine::GetPendingComponent(retro3d::Entity &e)
{
	Components::iterator i = m_components_pending_init.find(component_t::GetClassType());
	if (i != m_components_pending_init.end()) {
		ComponentClass::iterator j = i->second.find(e.GetInstanceID());
		return j != i->second.end() ? mtlCast<component_t>(j->second) : nullptr;
	}
	return nullptr;
}

template < typename component_t >
const component_t *retro3d::Engine::GetPendingComponent(const retro3d::Entity &e) const
{
	Components::const_iterator i = m_components_pending_init.find(component_t::GetClassType());
	if (i != m_components_pending_init.end()) {
		ComponentClass::const_iterator j = i->second.find(e.GetInstanceID());
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

template < typename entity_t >
entity_t *retro3d::Engine::SpawnEntity( void )
{
	entity_t *e = new entity_t;
	e->m_engine = this;
	e->m_delta_time = m_delta_time * e->m_time_scale;
	m_entities.AddLast(e);
	e->OnSpawn();
	return e;
}

template < typename component_t >
component_t *retro3d::Engine::AddComponent(retro3d::Entity &e)
{
	component_t *c = GetComponent<component_t>(e);
	if (e.m_should_destroy == false && c == nullptr) {
		c = new component_t;
		c->m_object = &e;
		m_components_pending_init[component_t::GetClassType()][e.GetInstanceID()] = c;
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

template < typename system_t >
system_t *retro3d::Engine::AddSystem( void )
{
	system_t *s = GetSystem<system_t>();
	if (s == nullptr) {
		s = new system_t;
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
