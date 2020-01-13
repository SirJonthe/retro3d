#ifndef RETRO_LIGHT_H
#define RETRO_LIGHT_H

#include "retro_model.h"
#include "../common/MiniLib/MML/mmlVector.h"
#include "../common/retro_geom.h"
#include "../common/retro_transform.h"
#include "../physics/retro_collider.h"

namespace retro3d
{

struct Light
{
	enum FalloffMode {
		None,          // no falloff
		Linear,        // 1 at d=0, 0 at d=radius
		Distance,      // semi-realistic light falloff (1/d)
		SquareDistance // realistic light falloff (1/d^2)
	};

	enum Type {
		Ambient,
		Direction,
		Point,
		Spot
	};

	mmlVector<3>  origin = mmlVector<3>::Fill(0.0f);
	mmlVector<3>  direction = retro3d::Transform::GetWorldForward();
	mmlVector<3>  color = mmlVector<3>::Fill(0.5f);
	float         radius = 20.0f;
	float         aperture = 1.0f;
	retro3d::AABB activation_aabb = { mmlVector<3>::Fill(0.0f), mmlVector<3>::Fill(20.0f) };
	Type          type = Light::Ambient;
	FalloffMode   falloff = Light::None;

	float CalculateIntensityAtVertex(const mmlVector<3> &point, const mmlVector<3> &normal) const;
	mmlVector<3> GetLightDirection(const mmlVector<3> &point) const;
	retro3d::Light ApplyTransform(const retro3d::Transform &transform) const;
};

class LightScene
{
private:
	struct TargetModel
	{
		const retro3d::Array< mmlVector<3> >       *verts;
		const retro3d::Array< mmlVector<2> >       *lightmap_uvs;
		const retro3d::Array< mmlVector<3> >       *normals;
		const retro3d::Array< retro3d::FaceIndex > *faces;
		retro3d::PolyCollider                       collider;
		retro3d::Transform                          transform;
	};

	struct Vertex
	{
		mmlVector<3> v;
		mmlVector<2> t;
		mmlVector<3> n;
	};

private:
	TargetModel               m_target;
	mtlList< PolyCollider >   m_geometry;
	mtlList< retro3d::Light > m_lights;
	float                     m_bias;
	bool                      m_has_target;
	bool                      m_use_dither;

private:
	Vertex ToVertex(const retro3d::IndexVTN &i, float u_scale, float v_scale) const;
	bool   IsIn(const mmlVector<3> &bcoord) const;

public:
	LightScene( void );

	void AddLight(const retro3d::Light &world_light);
	void AddGeometry(const retro3d::Array< mmlVector<3> > &v, const retro3d::Array< retro3d::FaceIndex > &f, const retro3d::Transform &obj_to_world);
	void SetTargetGeometry(const retro3d::Array< mmlVector<3> > *v, const retro3d::Array< mmlVector<2> > *t, const retro3d::Array< mmlVector<3> > *n, const retro3d::Array< retro3d::FaceIndex > *f, const retro3d::Transform &obj_to_world);

	void RenderLightmap(tiny3d::Image &out) const;
	void ClearScene( void );

	void SetShadowBias(float bias);
	bool DitheringEnabled( void ) const;
	void ToggleDithering( void );
};

}

#endif // RETRO_LIGHT_H
