#ifndef RETRO_SYSTEM_H
#define RETRO_SYSTEM_H

#include <cstdint>
#include "../common/MiniLib/MTL/mtlType.h"
#include "retro_component.h"

namespace retro3d
{

class Engine;

class ISystem : public mtlBase
{
	friend class Engine;

private:
	retro3d::Engine *m_engine;
	int32_t          m_is_active;
	bool             m_should_destroy;

protected:
	virtual void OnSpawn( void ) = 0;
	virtual void OnUpdate( void ) = 0;
	virtual void OnDestroy( void ) = 0;

	virtual void OnSpawn(retro3d::Component *c) = 0;
	virtual void OnUpdate(retro3d::Component *c) = 0;
	virtual void OnDestroy(retro3d::Component *c) = 0;

public:
	ISystem( void );
	virtual ~ISystem( void );

	ISystem(const ISystem&) = delete;
	ISystem &operator=(const ISystem&) = delete;

	void Destroy( void );
	bool IsDestroyed( void ) const;

	bool IsActive( void ) const;
	void Deactivate( void );
	void Activate( void );

	retro3d::Engine       *GetEngine( void );
	const retro3d::Engine *GetEngine( void ) const;

	virtual uint64_t GetComponentClassType( void ) const = 0;
};

template < typename component_t >
class System : public mtlInherit< ISystem, System<component_t> >
{
protected:
	void         OnSpawn( void ) override;
	void         OnUpdate( void ) override;
	void         OnDestroy( void ) override;

	void         OnSpawn(retro3d::Component *c) override;
	void         OnUpdate(retro3d::Component *c) override;
	void         OnDestroy(retro3d::Component *c) override;

	virtual void OnSpawn(component_t &c);
	virtual void OnUpdate(component_t &c);
	virtual void OnDestroy(component_t &c);

public:
	System( void );

	uint64_t     GetComponentClassType( void ) const override;
};

template < typename component_t >
retro3d::System<component_t>::System( void ) :
	mtlInherit< ISystem, System<component_t> >(this)
{}

template < typename component_t >
void retro3d::System<component_t>::OnSpawn( void )
{}

template < typename component_t >
void retro3d::System<component_t>::OnUpdate( void )
{}

template < typename component_t >
void retro3d::System<component_t>::OnDestroy( void )
{}

template < typename component_t >
void retro3d::System<component_t>::OnSpawn(retro3d::Component *c)
{
	OnSpawn(*mtlCast<component_t>(c));
}

template < typename component_t >
void retro3d::System<component_t>::OnUpdate(retro3d::Component *c)
{
	OnUpdate(*mtlCast<component_t>(c));
}

template < typename component_t >
void retro3d::System<component_t>::OnDestroy(retro3d::Component *c)
{
	OnDestroy(*mtlCast<component_t>(c));
}

template < typename component_t >
void retro3d::System<component_t>::OnSpawn(component_t&)
{}

template < typename component_t >
void retro3d::System<component_t>::OnUpdate(component_t&)
{}

template < typename component_t >
void retro3d::System<component_t>::OnDestroy(component_t&)
{}

template < typename component_t >
uint64_t retro3d::System<component_t>::GetComponentClassType( void ) const
{
	return component_t::GetClassType();
}

}

#define retro_system(system_class_name, component_class_name) class system_class_name : public mtlInherit< retro3d::System<component_class_name>, system_class_name >

#endif // RETRO_SYSTEM_H
