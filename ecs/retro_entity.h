#ifndef RETRO_ENTITY_H
#define RETRO_ENTITY_H

#include <cstdint>
#include <string>
#include "../common/retro_time.h"
#include "../common/MiniLib/MTL/mtlPointer.h"
#include "../serial/retro_serialize.h"

namespace retro3d
{

class Engine;
class RenderDevice;
class SoundDevice;
class InputDevice;

class Entity : public mtlInherit< retro3d::Serializeable, Entity >
{
	friend class Engine;

private:
	std::string             m_tag;
	uint64_t                m_filter_flags;
	const uint64_t          m_uuid;
	Engine                 *m_engine;
	retro3d::RealTimeTimer  m_game_timer;
	retro3d::Time           m_real_time_spawn;
	retro3d::Time           m_sim_time_spawn;
	retro3d::Time           m_game_time;
	double                  m_delta_time;
	int32_t                 m_is_active;
	bool                    m_should_destroy;

protected:
	// Called immediately after the engine has initialized the entity.
	virtual void OnSpawn( void );

	// Called periodically every engine loop.
	virtual void OnUpdate( void );

	// Called when two objects with collider components overlap.
	virtual void OnCollision(retro3d::Entity&);

	// Called on the engine's discretion when the engine destroys an entity marked for destruction.
	virtual void OnDestroy( void );

public:
	// Construction.
	Entity( void );
	Entity(const Entity&) = delete;
	Entity &operator=(const Entity&) = delete;

	// Marks the object (and all its components) for destruction. Engine decides when to actually free up the memory and invalidate references.
	void Destroy( void );

	// Determines if the entity is marked for destruction.
	bool IsDestroyed( void ) const;

	// Determines if the entity is active.
	bool IsActive( void ) const;

	// Recursively deactivates entity.
	void Deactivate( void );

	// Recursively activates entity.
	void Activate( void );

	// Returns this entity's unique instance ID.
	uint64_t GetUUID( void ) const;

	// Gets the parent engine.
	const retro3d::Engine *GetEngine( void ) const;
	retro3d::Engine       *GetEngine( void );

	// Gets the parent renderer.
	retro3d::RenderDevice       *GetRenderer( void );
	const retro3d::RenderDevice *GetRenderer( void ) const;

	// Gets the parent sound device.
	retro3d::SoundDevice       *GetSoundDevice( void );
	const retro3d::SoundDevice *GetSoundDevice( void ) const;

	// Gets the parent input device.
	retro3d::InputDevice       *GetInput( void );
	const retro3d::InputDevice *GetInput( void ) const;

	// Get and set the customized tag of the entity.
	const std::string &GetTag( void ) const;
	void               SetTag(const std::string &name);

	// Returns the unscaled lifetime this object has existed.
	retro3d::Time RealLifeTime( void ) const;

	// Returns the scaled simulated lifetime this object has existed.
	retro3d::Time SimulationLifeTime( void ) const;

	// Returns the user-scaled time this object has existed.
	retro3d::Time GameLifeTime( void ) const;

	// Returns the specified type of time.
	retro3d::Time LifeTime(retro3d::TimerType type) const;

	// Returns the scaled time delta (in seconds) since last update.
	double DeltaTime( void ) const;

	// Get and set the time scale for this object.
	void   SetTimeScale(double time_scale);
	double GetTimeScale( void ) const;

	// Creates a child timer of the entity game timer.
	retro3d::RealTimeTimer CreateGameTimer(uint32_t num_ticks = 1, retro3d::Time over_time = 1_s) const;

	// Creates a child timer of the specified type.
	retro3d::RealTimeTimer CreateTimer(retro3d::TimerType type, uint32_t num_ticks = 1, retro3d::Time over_time = 1_s) const;

	// Return the filter flags for the entity.
	uint64_t GetFilterFlags( void ) const;

	// Return the state of a single filter flag of the entity.
	bool GetFilterFlag(uint32_t flag_index) const;

	// Set the complete state of the filter flags of the entity.
	void SetFilterFlags(uint64_t filter_flags);

	// Set the state of a single filter flag of the entity.
	void SetFilterFlag(uint32_t flag_index, bool state);

	// Adds a component to the entity and returns it.
	template < typename component_t, typename... Args > component_t *AddComponent(Args&&... args);

	// Removes a component from the entity.
	template < typename component_t > void RemoveComponent( void );

	// Get an existing component. Null if component was never added to the object (or has since been removed).
	template < typename component_t > component_t       *GetComponent( void );
	template < typename component_t > const component_t *GetComponent( void ) const;
};

}

#include "../retro3d.h"

template < typename component_t, typename... Args >
component_t *retro3d::Entity::AddComponent(Args&&... args)
{
	return m_engine->AddComponent<component_t>(*this, std::forward<Args>(args)...);
}

template < typename component_t >
void retro3d::Entity::RemoveComponent( void )
{
	m_engine->RemoveComponent<component_t>(*this);
}

template < typename component_t >
component_t *retro3d::Entity::GetComponent( void )
{
	return m_engine->GetComponent<component_t>(*this);
}

template < typename component_t >
const component_t *retro3d::Entity::GetComponent( void ) const
{
	return m_engine->GetComponent<component_t>(*this);
}

#endif // RETRO_ENTITY_H
