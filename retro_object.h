#ifndef RETRO_OBJECT_H
#define RETRO_OBJECT_H

#include <cstdint>
#include "common/MiniLib/MTL/mtlString.h"
#include "common/MiniLib/MTL/mtlPointer.h"
#include "common/MiniLib/MTL/mtlType.h"
#include "graphics/retro_model.h"
#include "physics/retro_collider.h"
#include "audio/retro_voice.h"
#include "physics/retro_physics.h"

namespace retro3d
{

class Engine;

class Object : public mtlBase
{
	friend class Engine;

private:
	std::string                 m_name;
	retro3d::Engine            *m_engine;
	mtlShared<retro3d::Model>   m_model;
	retro3d::Collider          *m_collider;
	retro3d::SharedTransform    m_transform; // object-to-world transform
	mmlMatrix<4,4>              m_old_transform;
	mmlMatrix<4,4>              m_delta_transform;
	retro3d::AABBCollider       m_default_collider;
	retro3d::Voice              m_voice;
	retro3d::PhysicsProperties  m_physics;
	double                      m_time;
	double                      m_delta_time;
	double                      m_time_scale;
	int32_t                     m_is_active;
	bool                        m_is_visible;
	bool                        m_is_collideable;
	bool                        m_has_physics;
	bool                        m_is_mute;
	bool                        m_debug_aabb;
	bool                        m_should_destroy;

private:
	void UpdatePhysics(const mmlVector<3> &gravity);

protected:
	virtual void OnSpawn( void );
	virtual void OnUpdate( void );
	virtual void OnCollision(Object&);
	virtual void OnDestroy( void );

public:
	Object( void );
	virtual ~Object( void );

	void Destroy( void );

	retro3d::Transform         GetTransform( void ) const;
	mmlMatrix<4,4>             GetNormalizedTransform( void ) const;
	mmlMatrix<3,3>             GetRotation( void ) const;
	mmlVector<3>               GetPosition( void ) const;
	mmlMatrix<4,4>             GetDeltaTransform( void ) const;
	mmlMatrix<3,3>             GetDeltaRotation( void ) const;
	mmlVector<3>               GetDeltaPosition( void ) const;
	const std::string         &GetName( void ) const;
	void                       SetTransform(const mmlMatrix<4,4> &transform);
	void                       SetTransform(const mmlMatrix<3,3> &rotation, const mmlVector<3> &position);
	void                       SetRotation(const mmlMatrix<3,3> &rotation);
	void                       SetPosition(const mmlVector<3> &position);
	void                       SetName(const std::string &name);
	retro3d::Engine           *GetEngine( void ) const;
	mtlShared<retro3d::Model>  GetModel( void ) const;
	retro3d::Collider         *GetCollider( void ) const;
	double                     GetTimeScale( void ) const;
	void                       SetTimeScale(double time_scale);

	void SetModel(mtlShared<retro3d::Model> model);
	void SetCollider(retro3d::Collider *collider);
	void EnableGraphics( void );
	void DisableGraphics( void );
	void ToggleGraphics( void );
	void EnableCollisions( void );
	void DisableCollisions( void );
	void ToggleCollisions( void );
	void EnablePhysics( void );
	void DisablePhysics( void );
	void TogglePhysics( void );
	void EnableDebugAABB( void );
	void DisableDebugAABB( void );
	void ToggleDebugAABB( void );

	void ApplyForce(const mmlVector<3> &at, const mmlVector<3> &force);

	bool IsDestroyed( void ) const;

	bool IsActive( void ) const;
	void Deactivate( void );
	void Activate( void );

	double Time( void ) const;
	double DeltaTime( void ) const;

public:
	static mtlShared<retro3d::Model>   DefaultModel( void );
	static mtlShared<retro3d::Texture> DefaultTexture( void );
};

}

#endif // RETRO_OBJECT_H
