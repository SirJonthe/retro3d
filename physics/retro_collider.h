#ifndef RETRO_COLLIDER_H
#define RETRO_COLLIDER_H

#include "../common/MiniLib/MML/mmlVector.h"
#include "../common/MiniLib/MML/mmlMatrix.h"
#include "../common/MiniLib/MGL/mglCollision.h"
#include "../graphics/retro_model.h"
#include "../common/retro_transform.h"

// https://gamedev.stackexchange.com/questions/25950/how-do-i-make-a-collision-between-aabbs-slide

// For basic collision resolution
// http://noonat.github.io/intersect/

namespace retro3d
{

class AABBCollider;
class PolyCollider;

struct Ray
{
	mmlVector<3> origin;
	mmlVector<3> direction;
	float        length;

	typedef mglRayCollision3D Contact;

	Ray( void );
	Ray(const mmlVector<3> &p_origin, const mmlVector<3> &p_direction, float p_length = -1.0f);

	Ray ApplyTransform(const retro3d::Transform &transform) const;
};

class Collider
{
	friend class AABBCollider;
	friend class PolyCollider;

public:
	class Contact
	{
	public:
		mmlVector<3> point = mmlVector<3>::Fill(0.0f);
		mmlVector<3> normal = mmlVector<3>::Fill(0.0f);
		float        depth = 0.0f;

	private:
		struct Cache
		{
			// information about the last collision
			// used to speed up subsequent tests between the same collider pairs
		};

		Cache m_cache;
	};

protected:
	retro3d::SharedTransform m_transform; // from object to world space
	retro3d::AABB            m_obj_aabb; // object space AABB

protected: // double dispatch functions
	virtual bool DD_IsColliding(const retro3d::AABBCollider &c, retro3d::Collider::Contact *out) const = 0;
	virtual bool DD_IsColliding(const retro3d::PolyCollider &c, retro3d::Collider::Contact *out) const = 0;

public:
	static bool AABB_DetectCollision(const retro3d::Collider &a, const retro3d::Ray &r, retro3d::Ray::Contact *contact_info = nullptr);
	static bool AABB_DetectCollision(const retro3d::Collider &a, const retro3d::Collider &b);
	static bool GJK_DetectCollision(const retro3d::Collider &a, const retro3d::Collider &b, retro3d::Collider::Contact *contact_info = nullptr); // Slower than specialized functions, but works for *all* shapes that implement FindExtremePoint, avoiding a n² coding problem between contacts between two types of colliders.

public:
	Collider( void );
	virtual ~Collider( void );

	void                      AttachTransform(retro3d::SharedTransform transform);
	void                      SetTransform(const retro3d::Transform &transform);
	const retro3d::Transform &GetTransform( void ) const;
	retro3d::Transform       &GetTransform( void );
	retro3d::SharedTransform  GetSharedTransform( void ) const;
	retro3d::AABB             GetLocalAABB( void ) const;
	void                      SetLocalAABB(const retro3d::AABB &obj_aabb);
	retro3d::AABB             GetAABB( void ) const;

	// single dispatch
	virtual bool IsColliding(const retro3d::Ray &world_ray, retro3d::Ray::Contact *out) const = 0;
	// double dispatch
	virtual bool IsColliding(const retro3d::Collider &c, retro3d::Collider::Contact *out) const = 0;

	virtual mmlVector<3> FindExtremePoint(const mmlVector<3> &world_search_direction) const = 0;
};

class AABBCollider : public Collider
{
protected: // double dispatch functions
	bool DD_IsColliding(const retro3d::AABBCollider &c, retro3d::Collider::Contact *out) const override;
	bool DD_IsColliding(const retro3d::PolyCollider &c, retro3d::Collider::Contact *out) const override;

public:
	// single dispatch
	bool IsColliding(const retro3d::Ray &world_ray, retro3d::Ray::Contact *out) const override;
	// double dispatch
	bool IsColliding(const retro3d::Collider &c, retro3d::Collider::Contact *out) const override;

	mmlVector<3> FindExtremePoint(const mmlVector<3> &world_search_direction) const override;
};

//class SphereCollider : public Collider
//{};

class PolyCollider : public Collider
{
protected:
	retro3d::Array< mmlVector<3> >        m_vert;
	retro3d::Array< retro3d::FaceIndexV > m_face;

private:
	bool IsCollidingRay(const retro3d::Ray &world_ray, retro3d::Ray::Contact *out) const;

protected: // double dispatch functions
	bool DD_IsColliding(const retro3d::AABBCollider &c, retro3d::Collider::Contact *out) const override;
	bool DD_IsColliding(const retro3d::PolyCollider &c, retro3d::Collider::Contact *out) const override;

public:
	// single dispatch
	bool IsColliding(const retro3d::Ray &world_ray, retro3d::Ray::Contact *out) const override;
	// double dispatch
	bool IsColliding(const retro3d::Collider &c, retro3d::Collider::Contact *out) const override;

	void SetGeometry(const retro3d::Array< mmlVector<3> > &vert, const retro3d::Array< retro3d::FaceIndexV > &faces);
	void SetGeometry(const retro3d::Array< mmlVector<3> > &vert, const retro3d::Array< retro3d::FaceIndexVN > &faces);
	void SetGeometry(const retro3d::Array< mmlVector<3> > &vert, const retro3d::Array< retro3d::FaceIndex > &faces);

	mmlVector<3> FindExtremePoint(const mmlVector<3> &world_search_direction) const override;
};

}

#endif // RETRO_COLLIDER_H
