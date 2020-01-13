#ifndef RETRO_COMPONENT_H
#define RETRO_COMPONENT_H

#include <cstdint>
#include "../serial/retro_serialize.h"

namespace retro3d
{

class Entity;
class Engine;
class RenderDevice;
class SoundDevice;
class InputDevice;

class Component : public mtlInherit<retro3d::Serializeable, Component>
{
	friend class Engine;

private:
	retro3d::Entity  *m_object;
	int32_t           m_is_active;
	bool              m_should_destroy;

protected:
	virtual void OnSpawn( void );
	virtual void OnUpdate( void );
	virtual void OnCollision(retro3d::Entity&);
	virtual void OnDestroy( void );

public:
	Component( void );
	virtual ~Component( void );

	Component(const Component&) = delete;
	Component &operator=(const Component&) = delete;

	void Destroy( void );
	bool IsDestroyed( void ) const;

	bool IsActive( void ) const;
	void Deactivate( void );
	void Activate( void );

	retro3d::Entity       *GetObject( void );
	const retro3d::Entity *GetObject( void ) const;

	retro3d::Engine       *GetEngine( void );
	const retro3d::Engine *GetEngine( void ) const;

	retro3d::RenderDevice       *GetRenderer( void );
	const retro3d::RenderDevice *GetRenderer( void ) const;
	retro3d::SoundDevice        *GetSoundDevice( void );
	const retro3d::SoundDevice  *GetSoundDevice( void ) const;
	retro3d::InputDevice        *GetInput( void );
	const retro3d::InputDevice  *GetInput( void ) const;

//	template < typename component_t > component_t       *AddComponent( void );
//	template < typename component_t > void               RemoveComponent( void );
//	template < typename component_t > component_t       *GetComponent( void );
//	template < typename component_t > const component_t *GetComponent( void ) const;
};

}

// Recursive dependency makes this impossible without refactoring...
/*#include "retro_entity.h"

template < typename component_t >
component_t *retro3d::Component::AddComponent( void )
{
	return GetObject()->AddComponent<component_t>();
}

template < typename component_t >
void retro3d::Component::RemoveComponent( void )
{
	return GetObject()->RemoveComponent();
}

template < typename component_t >
component_t *retro3d::Component::GetComponent( void )
{
	return GetObject()->GetComponent<component_t>();
}

template < typename component_t >
const component_t *retro3d::Component::GetComponent( void ) const
{
	return GetObject()->GetComponent<component_t>();
}*/

#define retro_component(component_class_name) class component_class_name : public mtlInherit< retro3d::Component, component_class_name >

#endif // RETRO_COMPONENT_H
