#ifndef RETRO_ENTITY_H
#define RETRO_ENTITY_H

#include <cstdint>
#include <string>
#include "../common/MiniLib/MTL/mtlPointer.h"
#include "../serial/retro_serialize.h"

namespace retro3d
{

class Engine;
class RenderDevice;
class SoundDevice;
class InputDevice;
class ColliderComponent;
class LightComponent;
class RenderComponent;
class TransformComponent;

class Entity : public mtlInherit< retro3d::Serializeable, Entity >
{
	friend class Engine;

private:
	std::string     m_name;
	const uint64_t  m_uuid;
	Engine         *m_engine;
	double          m_time;
	double          m_delta_time;
	double          m_time_scale;
	int32_t         m_is_active;
	bool            m_should_destroy;

protected:
	virtual void OnSpawn( void );
	virtual void OnUpdate( void );
	virtual void OnCollision(retro3d::Entity&);
	virtual void OnDestroy( void );

public:
	Entity( void );
	Entity(const Entity&) = delete;
	Entity &operator=(const Entity&) = delete;

	void Destroy( void );
	bool IsDestroyed( void ) const;

	bool IsActive( void ) const;
	void Deactivate( void );
	void Activate( void );

	uint64_t GetInstanceID( void ) const;

	const retro3d::Engine       *GetEngine( void ) const;
	retro3d::Engine             *GetEngine( void );
	retro3d::RenderDevice       *GetRenderer( void );
	const retro3d::RenderDevice *GetRenderer( void ) const;
	retro3d::SoundDevice        *GetSoundDevice( void );
	const retro3d::SoundDevice  *GetSoundDevice( void ) const;
	retro3d::InputDevice        *GetInput( void );
	const retro3d::InputDevice  *GetInput( void ) const;

	const std::string     &GetName( void ) const;
	void                   SetName(const std::string &name);

	double Time( void ) const;
	double DeltaTime( void ) const;
	void   SetTimeScale(double scale);
	double GetTimeScale( void ) const;

	template < typename component_t > component_t       *AddComponent( void );
	template < typename component_t > void               RemoveComponent( void );
	template < typename component_t > component_t       *GetComponent( void );
	template < typename component_t > const component_t *GetComponent( void ) const;
};

}

#include "../retro3d.h"

template < typename component_t >
component_t *retro3d::Entity::AddComponent( void )
{
	return m_engine->AddComponent<component_t>(*this);
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
