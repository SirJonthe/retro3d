#include "retro_light.h"
#include "retro_rasterizer.h"

float retro3d::Light::CalculateIntensityAtVertex(const mmlVector<3> &point, const mmlVector<3> &normal) const
{
	float dist = 0.0f;
	if (falloff != None) {
		switch (type) {
		case Direction:
			dist = mmlMax(std::numeric_limits<float>::epsilon(), retro3d::Plane(origin, direction).GetDistance(point));
			break;
		case Ambient:
		case Point:
		case Spot:
			dist = mmlMax(std::numeric_limits<float>::epsilon(), mmlDist(origin, point));
			break;
		}
	} else {
		dist = -1.0f;
	}

	float f = 0.0f;
	if (dist <= radius) {
		switch (falloff) {
		case None:
			f = 1.0f;
			break;
		case Linear:
			f = 1.0f - (dist / radius);
			break;
		case Distance:
			f = 1.0f / dist;
			break;
		case SquareDistance:
			f = 1.0f / (dist*dist);
			break;
		}
	} else {
		return 0.0f;
	}

	float i = 0.0f;
	switch (type) {
	case retro3d::Light::Ambient:
		i = 1.0f;
		break;
	case retro3d::Light::Direction:
		i = -mmlDot(direction, normal);
		break;
	case retro3d::Light::Point:
		i = mmlDot(mmlIsApproxEqual(dist, 0.0f) ? direction : (mmlIsApproxEqual(dist, -1.0f) ? retro3d::NormalFromAToB(point, origin) : (retro3d::VectorFromAToB(point, origin) / dist)), normal);
		break;
	case retro3d::Light::Spot:
		{
			i = mmlMax(0.0f, -mmlDot(direction, normal)) * mmlDot(mmlIsApproxEqual(dist, 0.0f) ? direction : (mmlIsApproxEqual(dist, -1.0f) ? retro3d::NormalFromAToB(point, origin) : (retro3d::VectorFromAToB(point, origin) / dist)), normal) / aperture;
		}
		break;
	default:
		i = 0.0f;
		break;
	}
	return mmlClamp(0.0f, i * f, 1.0f);
}

mmlVector<3> retro3d::Light::GetLightDirection(const mmlVector<3> &point) const
{
	mmlVector<3> v;
	switch (type) {
	case retro3d::Light::Ambient:
		v = retro3d::NormalFromAToB(origin, point);
		break;
	case retro3d::Light::Direction:
		v = direction;
		break;
	case retro3d::Light::Point:
		v = retro3d::NormalFromAToB(origin, point);
		break;
	case retro3d::Light::Spot:
		v = direction;
		break;
	}
	return v;
}

retro3d::Light retro3d::Light::ApplyTransform(const retro3d::Transform &transform) const
{
	retro3d::Light l = *this;
	l.origin *= transform;
	l.direction *= transform.GetRotation();
	l.activation_aabb.ApplyTransform(transform);
	return l;
}

retro3d::LightScene::Vertex retro3d::LightScene::ToVertex(const retro3d::IndexVTN &i, float u_scale, float v_scale) const
{
	return Vertex{
		(*m_target.verts)[i.v] * m_target.transform,
		(*m_target.lightmap_uvs)[i.t] * mmlVector<2>(double(u_scale), double(v_scale)),
		(*m_target.normals)[i.n] * m_target.transform.GetRotation()
	};
}

bool retro3d::LightScene::IsIn(const mmlVector<3> &bcoord) const
{
	return
		(bcoord[0] >= 0.0f && bcoord[0] <= 1.0f) &&
		(bcoord[1] >= 0.0f && bcoord[1] <= 1.0f) &&
		(bcoord[2] >= 0.0f && bcoord[2] <= 1.0f);
}

retro3d::LightScene::LightScene( void ) : m_geometry(), m_lights(), m_bias(0.01f), m_has_target(false), m_use_dither(false)
{}

void retro3d::LightScene::AddLight(const retro3d::Light &world_light)
{
	m_lights.AddLast(world_light);
}

void retro3d::LightScene::AddGeometry(const retro3d::Array< mmlVector<3> > &verts, const retro3d::Array< retro3d::FaceIndex > &faces, const retro3d::Transform &obj_to_world)
{
	retro3d::PolyCollider *m = &m_geometry.AddLast();
	m->SetGeometry(verts, faces);
	m->SetTransform(obj_to_world);
}

void retro3d::LightScene::SetTargetGeometry(const retro3d::Array< mmlVector<3> > *verts, const retro3d::Array< mmlVector<2> > *lightmap_uvs, const retro3d::Array< mmlVector<3> > *normals, const retro3d::Array< retro3d::FaceIndex > *faces, const retro3d::Transform &obj_to_world)
{
	m_target.verts        = verts;
	m_target.lightmap_uvs = lightmap_uvs;
	m_target.normals      = normals;
	m_target.faces        = faces;
	m_target.transform    = obj_to_world;
	m_target.collider.SetGeometry(*verts, *faces);
	m_target.collider.SetTransform(obj_to_world);

	m_has_target = true;
}

void retro3d::LightScene::RenderLightmap(tiny3d::Image &out) const
{
	std::cout << "[LightScene::RenderLightmap] Progress..." << std::flush;
	if (m_has_target == false || out.GetWidth() == 0 || out.GetHeight() == 0) {
		std::cout << "aborted" << std::endl;
		return;
	}

	const float        TU_SCALE         = float(out.GetWidth());
	const float        TV_SCALE         = float(out.GetHeight());
	const mmlVector<3> V_ZERO           = mmlVector<3>::Fill(0.0f);
	const mmlVector<3> V_ONE            = mmlVector<3>::Fill(1.0f);
	const float        PROG_DELTA       = 1.0f / m_target.faces->GetSize();
	float              progress         = 0.0f;
	float              last_prog_report = 0.0f;

	for (int f = 0; f < m_target.faces->GetSize(); ++f) {
		const retro3d::FaceIndex *face_index = &(*m_target.faces)[f];
		for (int v = 1; v < face_index->GetSize() - 1; ++v) {
			const LightScene::Vertex a = ToVertex((*face_index)[0], TU_SCALE, TV_SCALE);
			const LightScene::Vertex b = ToVertex((*face_index)[v], TU_SCALE, TV_SCALE);
			const LightScene::Vertex c = ToVertex((*face_index)[v+1], TU_SCALE, TV_SCALE);

			// TODO: floor and ceil UV values to extend rendering somewhat beyond triangle area (prevents lighting halos at edges).
			// TODO: Maybe make a greedy 2D rasterizer for this purpose...

			const tiny3d::UInt min_y = tiny3d::UInt(mmlMin(a.t[1], b.t[1], c.t[1]));
			const tiny3d::UInt max_y = tiny3d::UInt(mmlMax(a.t[1], b.t[1], c.t[1]));
			const tiny3d::UInt min_x = tiny3d::UInt(mmlMin(a.t[0], b.t[0], c.t[0]));
			const tiny3d::UInt max_x = tiny3d::UInt(mmlMax(a.t[0], b.t[0], c.t[0]));

			for (tiny3d::UInt y = min_y; y <= max_y; ++y) {
				for (tiny3d::UInt x = min_x; x <= max_x; ++x) {

					// test if x,y is inside UV tri
					const mmlVector<2> uv = mmlVector<2>(double(x), double(y));
					const mmlVector<3> bary = retro3d::CartesianToBarycentric(uv, a.t, b.t, c.t);

					if (IsIn(bary) == true) { // coordinate is inside triangle

						// decide to cast a ray... or not.

						const tiny3d::UPoint p = { tiny3d::UInt(mmlClamp(0.0f, uv[0], TU_SCALE - 1.0f)), out.GetHeight() - tiny3d::UInt(mmlClamp(0.0f, uv[1], TV_SCALE - 1.0f)) - 1 }; // y-axis is flipped on conversion, so need to do this now otherwise rendering will be wrong

						const mmlVector<3> v = retro3d::BarycentricInterpolation(bary, a.v, b.v, c.v);
						const mmlVector<3> n = mmlNormalize(retro3d::BarycentricInterpolation(bary, a.n, b.n, c.n));

						mmlVector<3> diff_light = V_ZERO;

						const mtlItem<retro3d::Light> *light = m_lights.GetFirst();
						while (light != nullptr) {

							float I = light->GetItem().CalculateIntensityAtVertex(v, n);

							if (light->GetItem().type != retro3d::Light::Ambient) {
								retro3d::Ray world_ray;
								world_ray.origin    = v + n * m_bias; // NOTE: a small offset to avoid floating point imprecision (might want to toy around with this value)
								world_ray.direction = -light->GetItem().GetLightDirection(v); // NOTE: maybe just 'v' without bias???
								const mtlItem<retro3d::PolyCollider> *collider = m_geometry.GetFirst();
								while (collider != nullptr) {
									retro3d::Ray::Contact contact;
									if (collider->GetItem().IsColliding(world_ray, &contact) == true) {
										if (light->GetItem().type == retro3d::Light::Direction || contact.entry.distance < mmlDist(v, light->GetItem().origin)) {
											I = 0.0f;
											break;
										}
									}
									collider = collider->GetNext();
								}
							}

							if (I > 0.0f) {
								diff_light += light->GetItem().color * I;
							}

							light = light->GetNext();
						}
						diff_light = mmlClamp(V_ZERO, diff_light, V_ONE);
						diff_light *= 255.0f;

						const tiny3d::Color c = { tiny3d::Byte(diff_light[0]), tiny3d::Byte(diff_light[1]), tiny3d::Byte(diff_light[2]), tiny3d::Color::Solid };
						if (m_use_dither == true) {
							out.SetColor(p, tiny3d::Dither2x2(c, p));
						} else {
							out.SetColor(p, c);
						}
					}
				}
			}
		}

		progress += PROG_DELTA;
		while (progress > last_prog_report + 0.1f) {
			last_prog_report += 0.1f;
			std::cout << "[" << std::round(last_prog_report*100.0f) << "%]" << std::flush;
		}
	}
	std::cout << "done" << std::endl;
}

void retro3d::LightScene::ClearScene( void )
{
	m_geometry.RemoveAll();
	m_lights.RemoveAll();
	m_has_target = false;
}

void retro3d::LightScene::SetShadowBias(float bias)
{
	m_bias = bias;
}

bool retro3d::LightScene::DitheringEnabled( void ) const
{
	return m_use_dither;
}

void retro3d::LightScene::ToggleDithering( void )
{
	m_use_dither = !m_use_dither;
}
