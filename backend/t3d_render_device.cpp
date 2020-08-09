#include <sstream>
#include <iomanip>
#include "../common/MiniLib/MML/mmlMath.h"
#include "../api/tiny3d/tiny_draw.h"
#include "../retro3d.h"
#include "../common/retro_transform.h"
#include "t3d_render_device.h"

platform::T3DRenderDevice::Render3DJob *platform::T3DRenderDevice::Add3DJob(platform::T3DRenderDevice::Render3DJob::Type type, const mmlMatrix<4,4> &obj_to_world, retro3d::RenderDevice::LightMode light_mode)
{
	if (m_num_3d_items >= m_3d_queue.GetSize()) {
		RenderText(retro_text_err("render overflow\n"));
		return nullptr;
	} else {
		m_3d_queue[m_num_3d_items].type             = type;
		m_3d_queue[m_num_3d_items].light_mode       = light_mode;
		m_3d_queue[m_num_3d_items].obj_to_world     = obj_to_world;
		m_3d_queue[m_num_3d_items].world_to_obj     = mmlInv(obj_to_world);
		m_3d_queue[m_num_3d_items].obj_to_world_ptr = &m_3d_queue[m_num_3d_items].obj_to_world;
		++m_num_3d_items;
	}
	return &m_3d_queue[m_num_3d_items - 1];
}

platform::T3DRenderDevice::Render3DJob *platform::T3DRenderDevice::Add3DJob(platform::T3DRenderDevice::Render3DJob::Type type, const mmlMatrix<4,4> *obj_to_world, retro3d::RenderDevice::LightMode light_mode)
{
	if (m_num_3d_items >= m_3d_queue.GetSize()) {
		RenderText(retro_text_err("render overflow\n"));
		return nullptr;
	} else {
		m_3d_queue[m_num_3d_items].type             = type;
		m_3d_queue[m_num_3d_items].light_mode       = light_mode;
		m_3d_queue[m_num_3d_items].obj_to_world_ptr = obj_to_world;
		++m_num_3d_items;
	}
	return &m_3d_queue[m_num_3d_items - 1];
}

platform::T3DRenderDevice::Render2DJob *platform::T3DRenderDevice::Add2DJob(platform::T3DRenderDevice::Render2DJob::Type type, tiny3d::Point xy, tiny3d::Color color)
{
	if (m_num_2d_items < m_2d_queue.GetSize()) {
		m_2d_queue[m_num_2d_items].type  = type;
		m_2d_queue[m_num_2d_items].xy    = xy;
		m_2d_queue[m_num_2d_items].color = color;
		m_2d_queue[m_num_2d_items].scale = 1;
		++m_num_2d_items;
		return &m_2d_queue[m_num_2d_items - 1];
	}
	return nullptr;
}

mmlVector<3> platform::T3DRenderDevice::ProjectWorldSpaceToScreenSpace(const mmlVector<3> &v) const
{
	const float fwidth = float(m_dst.GetWidth());
	const float fheight = float(m_dst.GetHeight());
	const float screen_offset_x = fwidth / 2.0f;
	const float screen_offset_y = fheight / 2.0f;
	mmlVector<3> p;
	p[0] =  (v[0] / v[2]) * fheight * m_hfov_scalar + screen_offset_x;
	p[1] = -(v[1] / v[2]) * fheight * m_hfov_scalar + screen_offset_y;
	p[2] =  v[2];
	return p;
}

bool platform::T3DRenderDevice::IsFront(const mmlVector<3> &a, const mmlVector<3> &b, const mmlVector<3> &c) const
{
	return (b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]) > 0.0f;
}

void platform::T3DRenderDevice::ClearBuffers(tiny3d::URect rect)
{
	for (tiny3d::UInt y = rect.a.y; y < rect.b.y; ++y) {
		for (tiny3d::UInt x = rect.a.x; x < rect.b.x; ++x) {
			m_zbuf[y * m_dst.GetWidth() + x] = std::numeric_limits<float>::infinity();
		}
	}
}

/*float platform::T3DRenderDevice::Area(const mmlVector<2> &a, const mmlVector<2> &b, const mmlVector<2> &c) const
{
	return ((a[0]*b[1] - a[1]*b[0]) + (b[0]*c[1] - b[1]*c[0]) + (c[0]*a[1] - c[1]*a[0])) / 2.0f;
}*/

tiny3d::UInt platform::T3DRenderDevice::DetermineMipLevel(const retro3d::Texture &mips, const retro3d::Vertex &ss_a, const retro3d::Vertex &ss_b, const retro3d::Vertex &ss_c) const
{
	// ss_* = screen space triangle vertex
	mmlVector<2> ta, tb, tc;
	float inv_tri_area, tex_area, ratio;
	tiny3d::UInt mip = 0;
	if (mips[0] != nullptr) {
		inv_tri_area = 1.0f / retro3d::TriangleArea2D(mmlVector<2>::Cast(ss_a.v), mmlVector<2>::Cast(ss_b.v), mmlVector<2>::Cast(ss_c.v));
		for (mip = 0; mip < RETRO3D_MIP_COUNT - 1; ++mip) {
			ta = ss_a.t * float(mips[mip]->GetWidth());
			tb = ss_b.t * float(mips[mip]->GetWidth());
			tc = ss_c.t * float(mips[mip]->GetWidth());
			tex_area = retro3d::TriangleArea2D(ta, tb, tc);
			ratio = mmlAbs(tex_area * inv_tri_area);
			if (ratio < m_mip_ratio) { break; }
		}
	}
	return mip;
}

void platform::T3DRenderDevice::Render(tiny3d::URect rect)
{
	tiny3d::Array< retro3d::Light > temp_light_array(m_num_lights);

	for (tiny3d::UInt n = 0; n < m_num_3d_items; ++n) {
		switch (m_3d_queue[n].type) {
		case Render3DJob::Model:
			RenderModel(rect, m_3d_queue[n], temp_light_array);
			break;
		case Render3DJob::DisplayModel:
			RenderDisplay(rect, m_3d_queue[n], temp_light_array);
			break;
		case Render3DJob::Text:
			// Implement 3D text
			break;
		case Render3DJob::AABB:
			RenderAABB(rect, m_3d_queue[n], temp_light_array);
			break;
		case Render3DJob::Frustum:
			RenderFrustum(rect, m_3d_queue[n], temp_light_array);
			break;
		default: break;
		}
	}
}

void platform::T3DRenderDevice::RenderModel(const tiny3d::URect &rect, const platform::T3DRenderDevice::Render3DJob &job, tiny3d::Array< retro3d::Light > &lights)
{
	tiny3d::UInt num_lights = 0;

	const retro3d::Model *model = job.model;
	const mmlMatrix<4,4> obj_to_view = job.face_camera == false ? m_world_to_view * job.obj_to_world : m_world_to_view * mmlTransform(mmlRotation(job.obj_to_world) * mmlRotation(m_view_to_world), mmlTranslation(job.obj_to_world));
	const RenderMode mode = job.render_mode;
	LightMode        light_mode = job.light_mode;

	// Determine what lights affect the model
	if (job.light_mode == LightMode_Dynamic) {
		retro3d::AABB world_model_aabb = model->aabb.ApplyTransform(job.obj_to_world);
		for (tiny3d::UInt l = 0; l < m_num_lights; ++l) {
//			if (m_lights[l].aabb.Overlaps(world_model_aabb) == true && world_model_aabb.Contains(m_lights[l].aabb) == false) {
			if (m_lights[l].activation_aabb.Contains(world_model_aabb) > retro3d::Contain_False) {
				lights[num_lights] = m_lights[l].ApplyTransform(job.world_to_obj);
				++num_lights;
			}
		}
	} else if (light_mode == LightMode_Lightmap && model->lightmap.IsNull()) {
		light_mode = LightMode_Fullbright;
	}

	// Iterate through materials and render triangles therein
	for (int i = 0; i < model->m.GetSize(); ++i) {

		const retro3d::Material &material = model->m[i];
		const retro3d::Texture  *texture = material.td.GetShared();

		for (int j = 0; j < material.f.GetSize(); ++j) {

			const retro3d::FaceIndex &face = material.f[j];

			for (int t = 1; t < face.GetSize() - 1; ++t) {

				retro3d::IndexVTN i1 = face[0], i2 = face[t], i3 = face[t + 1];

				retro3d::Vertex a, b, c;

				// Transform model space vertices to view space
				a.v = model->v[i1.v] * obj_to_view;
				b.v = model->v[i2.v] * obj_to_view;
				c.v = model->v[i3.v] * obj_to_view;

				// Early exit if all triangles are behind near clipping plane
				if (a.v[2] < m_near_z && b.v[2] < m_near_z && c.v[2] < m_near_z) { continue; }

				// Apply lighting to vertices
				mmlVector<3> an, bn, cn;
				if (model->n.GetSize() > 0) {
					an = model->n[i1.n];
					bn = model->n[i2.n];
					cn = model->n[i3.n];
				} else {
					an = bn = cn = mmlSurfaceNormal(model->v[i1.v], model->v[i2.v], model->v[i3.v]);
				}
				switch (light_mode) {
				case LightMode_Fullbright:
				case LightMode_Lightmap:
					a.c = b.c = c.c = material.cd;
					break;
				case LightMode_Dynamic:
					a.c = b.c = c.c = mmlVector<3>::Fill(0.0f);
					break;
				case LightMode_NormalsAsColor:
					a.c = an;
					b.c = bn;
					c.c = cn;
					break;
				}
				for (tiny3d::UInt l = 0; l < num_lights; ++l) {
					float da = lights[l].CalculateIntensityAtVertex(model->v[i1.v], an);
					float db = lights[l].CalculateIntensityAtVertex(model->v[i2.v], bn);
					float dc = lights[l].CalculateIntensityAtVertex(model->v[i3.v], cn);
					a.c = mmlMin(a.c + material.cd * da * lights[l].color, mmlVector<3>::Fill(1.0f));
					b.c = mmlMin(b.c + material.cd * db * lights[l].color, mmlVector<3>::Fill(1.0f));
					c.c = mmlMin(c.c + material.cd * dc * lights[l].color, mmlVector<3>::Fill(1.0f));
				}

				if (model->t.GetSize() > 0) {
					a.t = model->t[i1.t];
					b.t = model->t[i2.t];
					c.t = model->t[i3.t];
				} else {
					a.t = b.t = c.t = mmlVector<2>::Fill(0.0f);
				}

				// Clip view space triangles against near plane
				retro3d::Vertex out[4];
				tiny3d::Vertex final_vtx[4];
				const tiny3d::UInt num_out = m_near_plane.Clip(a, b, c, out);

				// Early exit if clipping does not result in a valid geometric shape
				if (num_out <= 2) { continue; }

				// Project view space vertices to screen space
				for (tiny3d::UInt i = 0; i < num_out; ++i) {
					out[i].v = ProjectWorldSpaceToScreenSpace(out[i].v);
				}

				// Early exit if resulting shape does not face camera
				if (IsFront(out[0].v, out[1].v, out[2].v) == false) { continue; }

				for (tiny3d::UInt i = 0; i < num_out; ++i) {
					final_vtx[i].v = tiny3d::Vector3(tiny3d::Real(out[i].v[0]), tiny3d::Real(out[i].v[1]), tiny3d::Real(out[i].v[2]));
					final_vtx[i].t = tiny3d::Vector2(tiny3d::Real(out[i].t[0]), tiny3d::Real(out[i].t[1]));
					final_vtx[i].c = tiny3d::Color{ tiny3d::Byte(out[i].c[0] * 255.0f), tiny3d::Byte(out[i].c[1] * 255.0f), tiny3d::Byte(out[i].c[2] * 255.0f), tiny3d::Color::Solid };
				}

				const tiny3d::Texture *tex_mip = texture != nullptr ? (*texture)[DetermineMipLevel(*texture, out[0], out[1], out[2])] : nullptr;

				switch (num_out) {
				case 4:
					switch (mode) {
					case RenderMode_Points:
						tiny3d::DrawPoint(m_dst, &m_zbuf, &m_zbuf, final_vtx[0], tex_mip, &rect);
						tiny3d::DrawPoint(m_dst, &m_zbuf, &m_zbuf, final_vtx[1], tex_mip, &rect);
						tiny3d::DrawPoint(m_dst, &m_zbuf, &m_zbuf, final_vtx[2], tex_mip, &rect);
						tiny3d::DrawPoint(m_dst, &m_zbuf, &m_zbuf, final_vtx[3], tex_mip, &rect);
						break;
					case RenderMode_Lines:
						tiny3d::DrawLine(m_dst, &m_zbuf, &m_zbuf, final_vtx[0], final_vtx[1], tex_mip, &rect);
						tiny3d::DrawLine(m_dst, &m_zbuf, &m_zbuf, final_vtx[1], final_vtx[2], tex_mip, &rect);
						tiny3d::DrawLine(m_dst, &m_zbuf, &m_zbuf, final_vtx[2], final_vtx[3], tex_mip, &rect);
						tiny3d::DrawLine(m_dst, &m_zbuf, &m_zbuf, final_vtx[3], final_vtx[0], tex_mip, &rect);
						break;
					case RenderMode_Polygons:
						switch (light_mode) {
						default:
							tiny3d::DrawTriangle(m_dst, &m_zbuf, &m_zbuf, final_vtx[0], final_vtx[1], final_vtx[2], tex_mip, &rect);
							tiny3d::DrawTriangle(m_dst, &m_zbuf, &m_zbuf, final_vtx[0], final_vtx[2], final_vtx[3], tex_mip, &rect);
							break;
						case LightMode_Lightmap:
							{
								tiny3d::LVertex fvl[4] = {
									{ final_vtx[0].v, final_vtx[0].t, final_vtx[0].t},
									{ final_vtx[1].v, final_vtx[1].t, final_vtx[1].t},
									{ final_vtx[2].v, final_vtx[2].t, final_vtx[2].t},
									{ final_vtx[3].v, final_vtx[3].t, final_vtx[3].t}
								};
								const retro3d::Texture *rlmap = model->lightmap.GetShared();
								const tiny3d::Texture  *tlmap = (*rlmap)[DetermineMipLevel(*rlmap, out[0], out[1], out[2])];
								tiny3d::DrawTriangle(m_dst, &m_zbuf, &m_zbuf, fvl[0], fvl[1], fvl[2], tex_mip, *tlmap, &rect);
								tiny3d::DrawTriangle(m_dst, &m_zbuf, &m_zbuf, fvl[0], fvl[2], fvl[3], tex_mip, *tlmap, &rect);
								break;
							}
						}
						break;
					}
					break;
				case 3:
					switch (mode) {
					case RenderMode_Points:
						tiny3d::DrawPoint(m_dst, &m_zbuf, &m_zbuf, final_vtx[0], tex_mip, &rect);
						tiny3d::DrawPoint(m_dst, &m_zbuf, &m_zbuf, final_vtx[1], tex_mip, &rect);
						tiny3d::DrawPoint(m_dst, &m_zbuf, &m_zbuf, final_vtx[2], tex_mip, &rect);
						break;
					case RenderMode_Lines:
						tiny3d::DrawLine(m_dst, &m_zbuf, &m_zbuf, final_vtx[0], final_vtx[1], tex_mip, &rect);
						tiny3d::DrawLine(m_dst, &m_zbuf, &m_zbuf, final_vtx[1], final_vtx[2], tex_mip, &rect);
						tiny3d::DrawLine(m_dst, &m_zbuf, &m_zbuf, final_vtx[2], final_vtx[0], tex_mip, &rect);
						break;
					case RenderMode_Polygons:
						switch (light_mode) {
						default:
							tiny3d::DrawTriangle(m_dst, &m_zbuf, &m_zbuf, final_vtx[0], final_vtx[1], final_vtx[2], tex_mip, &rect);
							break;
						case LightMode_Lightmap:
							{
								tiny3d::LVertex fvl[3] = {
									{ final_vtx[0].v, final_vtx[0].t, final_vtx[0].t},
									{ final_vtx[1].v, final_vtx[1].t, final_vtx[1].t},
									{ final_vtx[2].v, final_vtx[2].t, final_vtx[2].t}
								};
								const retro3d::Texture *rlmap = model->lightmap.GetShared();
								const tiny3d::Texture  *tlmap = (*rlmap)[DetermineMipLevel(*rlmap, out[0], out[1], out[2])];
								tiny3d::DrawTriangle(m_dst, &m_zbuf, &m_zbuf, fvl[0], fvl[1], fvl[2], tex_mip, *tlmap, &rect);
								break;
							}
						}
						break;
					}
					break;
				default: break;
				}
			}
		}
	}
}

void platform::T3DRenderDevice::RenderSky(tiny3d::URect rect)
{
	// HACK:
	for (tiny3d::UInt y = rect.a.y; y < rect.b.y; ++y) {
		for (tiny3d::UInt x = rect.a.x; x < rect.b.x; ++x) {
			if (std::isinf(m_zbuf[y * m_dst.GetWidth() + x]) == false) {
				m_dst.SetStencil(tiny3d::UPoint{ x, y }, tiny3d::Color::Transparent);
			}
		}
	}

	mmlMatrix<4,4> obj_to_world = mmlTransform(
		mmlMatrix<3,3>::Identity() * (m_near_z * 5.0f), // scale the box so that the near plane will not visibly clip it
		mmlTranslation(m_view_to_world)
	);

	const retro3d::Model *model = &m_skybox;
	const mmlMatrix<4,4>  obj_to_view = m_world_to_view * obj_to_world;

	// Iterate through materials and render triangles therein
	for (int i = 0; i < model->m.GetSize(); ++i) {

		const retro3d::Material &material = model->m[i];
		const retro3d::Texture  *texture = material.td.GetShared();

		for (int j = 0; j < material.f.GetSize(); ++j) {

			const retro3d::FaceIndex &face = material.f[j];

			for (int t = 1; t < face.GetSize() - 1; ++t) {

				retro3d::IndexVTN i1 = face[0], i2 = face[t], i3 = face[t + 1];

				retro3d::Vertex a, b, c;

				// Transform model space vertices to view space
				a.v = model->v[i1.v] * obj_to_view;
				b.v = model->v[i2.v] * obj_to_view;
				c.v = model->v[i3.v] * obj_to_view;

				if (model->t.GetSize() > 0) {
					a.t = model->t[i1.t];
					b.t = model->t[i2.t];
					c.t = model->t[i3.t];
				} else {
					a.t = b.t = c.t = mmlVector<2>::Fill(0.0f);
				}

				// Clip view space triangles against near plane
				retro3d::Vertex out[4];
				tiny3d::Vertex final_vtx[4];
				const tiny3d::UInt num_out = m_near_plane.Clip(a, b, c, out);

				// Project view space vertices to screen space
				for (tiny3d::UInt i = 0; i < num_out; ++i) {
					out[i].v = ProjectWorldSpaceToScreenSpace(out[i].v);
				}

				for (tiny3d::UInt i = 0; i < num_out; ++i) {
					final_vtx[i].v = tiny3d::Vector3(tiny3d::Real(out[i].v[0]), tiny3d::Real(out[i].v[1]), tiny3d::Real(out[i].v[2]));
					final_vtx[i].t = tiny3d::Vector2(tiny3d::Real(out[i].t[0]), tiny3d::Real(out[i].t[1]));
					final_vtx[i].c = tiny3d::Color{ 255, 255, 255, tiny3d::Color::Solid };
				}

				const tiny3d::Texture *tex_mip = texture->GetHighestQuality();

				switch (num_out) {
				case 4:
					tiny3d::DrawTriangle(m_dst, nullptr, nullptr, final_vtx[0], final_vtx[1], final_vtx[2], tex_mip, &rect);
					tiny3d::DrawTriangle(m_dst, nullptr, nullptr, final_vtx[0], final_vtx[2], final_vtx[3], tex_mip, &rect);
					break;
				case 3:
					tiny3d::DrawTriangle(m_dst, nullptr, nullptr, final_vtx[0], final_vtx[1], final_vtx[2], tex_mip, &rect);
					break;
				default: break;
				}
			}
		}
	}

	// HACK: Revert render mask (FIXME: use stencil to render normal objects, then this won't be necessary)
	m_dst.ClearStencil(rect, tiny3d::Color::Solid);
}

void platform::T3DRenderDevice::RenderDisplay(const tiny3d::URect &rect, const platform::T3DRenderDevice::Render3DJob &job, tiny3d::Array< retro3d::Light > &lights)
{
	const retro3d::Frustum world_view = GetViewFrustum();

	tiny3d::UInt num_lights = 0;

	const retro3d::DisplayModel *model = job.display_model;
	const mmlMatrix<4,4> obj_to_view = m_world_to_view * job.obj_to_world;
	const retro3d::Frustum view_frustum = world_view.ApplyTransform(job.world_to_obj);
	const RenderMode mode = job.render_mode;

	// Determine what lights affect the model
	if (job.light_mode == LightMode_Dynamic) {
		retro3d::AABB world_model_aabb = model->GetAABB().ApplyTransform(job.obj_to_world);
		for (tiny3d::UInt l = 0; l < m_num_lights; ++l) {
//			if (m_lights[l].aabb.Overlaps(world_model_aabb) == true && world_model_aabb.Contains(m_lights[l].aabb) == false) {
			if (m_lights[l].activation_aabb.Contains(world_model_aabb) > retro3d::Contain_False) {
				lights[num_lights] = m_lights[l].ApplyTransform(job.world_to_obj);
				++num_lights;
			}
		}
	}

	retro3d::DisplayModel::GeometryQueue q = model->BuildViewOrder(view_frustum);
	while (q.IsEmpty() == false) {
		const retro3d::DisplayModel::GeometryNode *node = q.Pop();

		const retro3d::DisplayModel::Surface *surface = node->GetSurfaces();
		const retro3d::DisplayModel::Polygon *poly    = node->GetPolygons();
		for (uint32_t si = 0; si < node->surface_count; ++si) {

			const retro3d::DisplayModel::Material *material = &model->GetMaterial(surface->material_index);
			const retro3d::Texture                *texture = material->texture.GetShared();

			for (uint32_t pi = 0; pi < surface->poly_count; ++pi) {

				const retro3d::DisplayModel::Vertex *vert = poly->GetVerts();

				for (uint32_t t = 1; t < poly->vert_count - 1; ++t) {

					retro3d::Vertex a, b, c;

					// Transform model space vertices to view space
					a.v = vert[0].v * obj_to_view;
					b.v = vert[t].v * obj_to_view;
					c.v = vert[t+1].v * obj_to_view;

					// Early exit if all triangles are behind near clipping plane
					if (a.v[2] < m_near_z && b.v[2] < m_near_z && c.v[2] < m_near_z) { continue; }

					// Set texture coordinates
					a.t = vert[0].t;
					b.t = vert[t].t;
					c.t = vert[t+1].t;

					// Apply lighting to vertices
					mmlVector<3> an, bn, cn;
					an = vert[0].n;
					bn = vert[t].n;
					cn = vert[t+1].n;

					switch (job.light_mode) {
					case LightMode_Fullbright:
					case LightMode_Lightmap:
						a.c = b.c = c.c = material->color;
						break;
					case LightMode_Dynamic:
						a.c = b.c = c.c = mmlVector<3>::Fill(0.0f);
						break;
					case LightMode_NormalsAsColor:
						a.c = an;
						b.c = bn;
						c.c = cn;
						break;
					}

					for (tiny3d::UInt l = 0; l < num_lights; ++l) {
						float da = lights[l].CalculateIntensityAtVertex(vert[0].v, an);
						float db = lights[l].CalculateIntensityAtVertex(vert[t].v, bn);
						float dc = lights[l].CalculateIntensityAtVertex(vert[t+1].v, cn);
						a.c = mmlMin(a.c + material->color * da * lights[l].color, mmlVector<3>::Fill(1.0f));
						b.c = mmlMin(b.c + material->color * db * lights[l].color, mmlVector<3>::Fill(1.0f));
						c.c = mmlMin(c.c + material->color * dc * lights[l].color, mmlVector<3>::Fill(1.0f));
					}

					// Clip view space triangles against near plane
					retro3d::Vertex out[4];
					tiny3d::Vertex final_vtx[4];
					const tiny3d::UInt num_out = m_near_plane.Clip(a, b, c, out);

					// Early exit if clipping does not result in a valid geometric shape
					if (num_out <= 2) { continue; }

					// Project view space vertices to screen space
					for (tiny3d::UInt i = 0; i < num_out; ++i) {
						out[i].v = ProjectWorldSpaceToScreenSpace(out[i].v);
					}

					// Early exit if resulting shape does not face camera
					if (IsFront(out[0].v, out[1].v, out[2].v) == false) { continue; }

					for (tiny3d::UInt i = 0; i < num_out; ++i) {
						final_vtx[i].v = tiny3d::Vector3(tiny3d::Real(out[i].v[0]), tiny3d::Real(out[i].v[1]), tiny3d::Real(out[i].v[2]));
						final_vtx[i].t = tiny3d::Vector2(tiny3d::Real(out[i].t[0]), tiny3d::Real(out[i].t[1]));
						final_vtx[i].c = tiny3d::Color{ tiny3d::Byte(out[i].c[0] * 255.0f), tiny3d::Byte(out[i].c[1] * 255.0f), tiny3d::Byte(out[i].c[2] * 255.0f), tiny3d::Color::Solid };
					}

					const tiny3d::Texture *tex_mip = texture != nullptr ? (*texture)[DetermineMipLevel(*texture, out[0], out[1], out[2])] : nullptr;

					switch (num_out) {
					case 4:
						switch (mode) {
						case RenderMode_Points:
							tiny3d::DrawPoint(m_dst, &m_zbuf, &m_zbuf, final_vtx[0], tex_mip, &rect);
							tiny3d::DrawPoint(m_dst, &m_zbuf, &m_zbuf, final_vtx[1], tex_mip, &rect);
							tiny3d::DrawPoint(m_dst, &m_zbuf, &m_zbuf, final_vtx[2], tex_mip, &rect);
							tiny3d::DrawPoint(m_dst, &m_zbuf, &m_zbuf, final_vtx[3], tex_mip, &rect);
							break;
						case RenderMode_Lines:
							tiny3d::DrawLine(m_dst, &m_zbuf, &m_zbuf, final_vtx[0], final_vtx[1], tex_mip, &rect);
							tiny3d::DrawLine(m_dst, &m_zbuf, &m_zbuf, final_vtx[1], final_vtx[2], tex_mip, &rect);
							tiny3d::DrawLine(m_dst, &m_zbuf, &m_zbuf, final_vtx[2], final_vtx[3], tex_mip, &rect);
							tiny3d::DrawLine(m_dst, &m_zbuf, &m_zbuf, final_vtx[3], final_vtx[0], tex_mip, &rect);
							break;
						case RenderMode_Polygons:
							tiny3d::DrawTriangle(m_dst, &m_zbuf, &m_zbuf, final_vtx[0], final_vtx[1], final_vtx[2], tex_mip, &rect);
							tiny3d::DrawTriangle(m_dst, &m_zbuf, &m_zbuf, final_vtx[0], final_vtx[2], final_vtx[3], tex_mip, &rect);
							break;
						}
						break;
					case 3:
						switch (mode) {
						case RenderMode_Points:
							tiny3d::DrawPoint(m_dst, &m_zbuf, &m_zbuf, final_vtx[0], tex_mip, &rect);
							tiny3d::DrawPoint(m_dst, &m_zbuf, &m_zbuf, final_vtx[1], tex_mip, &rect);
							tiny3d::DrawPoint(m_dst, &m_zbuf, &m_zbuf, final_vtx[2], tex_mip, &rect);
							break;
						case RenderMode_Lines:
							tiny3d::DrawLine(m_dst, &m_zbuf, &m_zbuf, final_vtx[0], final_vtx[1], tex_mip, &rect);
							tiny3d::DrawLine(m_dst, &m_zbuf, &m_zbuf, final_vtx[1], final_vtx[2], tex_mip, &rect);
							tiny3d::DrawLine(m_dst, &m_zbuf, &m_zbuf, final_vtx[2], final_vtx[0], tex_mip, &rect);
							break;
						case RenderMode_Polygons:
							tiny3d::DrawTriangle(m_dst, &m_zbuf, &m_zbuf, final_vtx[0], final_vtx[1], final_vtx[2], tex_mip, &rect);
							break;
						}
						break;
					default: break;
					}
				}
				poly = poly->GetNext();
			}
			surface = surface->GetNext();
		}
	}
}

void platform::T3DRenderDevice::RenderAABB(const tiny3d::URect &rect, const platform::T3DRenderDevice::Render3DJob &job, tiny3d::Array< retro3d::Light > &lights)
{
	const tiny3d::UInt ldef[] = {
		0, 1, 1, 2, 2, 3, 3, 0,
		4, 5, 5, 6, 6, 7, 7, 4,
		0, 4, 1, 5, 2, 6, 3, 7
	};
	mmlVector<3> v[8];

	if (job.world_axis_aligned == true) {
		retro3d::AABB world_model_aabb = job.obj_aabb.ApplyTransform(job.obj_to_world);
		world_model_aabb.GetCorners(v);
		for (int i = 0; i < 8; ++i) {
			v[i] *= m_world_to_view;
		}
	} else {
		const mmlMatrix<4,4> obj_to_view = m_world_to_view * job.obj_to_world;
		job.obj_aabb.GetCorners(v);
		for (int i = 0; i < 8; ++i) {
			v[i] *= obj_to_view;
		}
	}

	for (tiny3d::UInt i = 0; i < sizeof(ldef) / sizeof(tiny3d::UInt); i += 2) {
		mmlVector<3> a = v[ldef[i]];
		mmlVector<3> b = v[ldef[i+1]];

		if (a[2] < m_near_z) {
			if (b[2] < m_near_z) { continue; }
			// clip A
			const float a_dist = m_near_plane.GetDistance(a);
			const float b_dist = m_near_plane.GetDistance(b);
			a = mmlLerp(b, a, b_dist / (b_dist - a_dist));
		} else if (b[2] < m_near_z) {
			// clip B
			const float a_dist = m_near_plane.GetDistance(a);
			const float b_dist = m_near_plane.GetDistance(b);
			b = mmlLerp(a, b, a_dist / (a_dist - b_dist));
		}

		a = ProjectWorldSpaceToScreenSpace(a);
		b = ProjectWorldSpaceToScreenSpace(b);

		tiny3d::Vertex A = { tiny3d::Vector3{ tiny3d::Real(a[0]), tiny3d::Real(a[1]), tiny3d::Real(a[2]) }, tiny3d::Vector2{ tiny3d::Real(0.0f), tiny3d::Real(0.0f) }, job.color };
		tiny3d::Vertex B = { tiny3d::Vector3{ tiny3d::Real(b[0]), tiny3d::Real(b[1]), tiny3d::Real(b[2]) }, tiny3d::Vector2{ tiny3d::Real(0.0f), tiny3d::Real(0.0f) }, job.color };

		tiny3d::DrawLine(m_dst, &m_zbuf, &m_zbuf, A, B, nullptr, &rect);
	}
}

void platform::T3DRenderDevice::RenderFrustum(const tiny3d::URect &rect, const platform::T3DRenderDevice::Render3DJob &job, tiny3d::Array< retro3d::Light > &lights)
{
	const tiny3d::UInt ldef[] = {
		0, 1, 1, 2, 2, 3, 3, 0,
		4, 5, 5, 6, 6, 7, 7, 4,
		0, 4, 1, 5, 2, 6, 3, 7
	};
	mmlVector<3> v[8];

	const mmlMatrix<4,4> obj_to_view = m_world_to_view * job.obj_to_world;
	job.obj_view_frustum.GetCorners(v);
	for (int i = 0; i < 8; ++i) {
		v[i] *= obj_to_view;
	}

	for (tiny3d::UInt i = 0; i < sizeof(ldef) / sizeof(tiny3d::UInt); i += 2) {
		mmlVector<3> a = v[ldef[i]];
		mmlVector<3> b = v[ldef[i+1]];

		if (a[2] < m_near_z) {
			if (b[2] < m_near_z) { continue; }
			// clip A
			const float a_dist = m_near_plane.GetDistance(a);
			const float b_dist = m_near_plane.GetDistance(b);
			a = mmlLerp(b, a, b_dist / (b_dist - a_dist));
		} else if (b[2] < m_near_z) {
			// clip B
			const float a_dist = m_near_plane.GetDistance(a);
			const float b_dist = m_near_plane.GetDistance(b);
			b = mmlLerp(a, b, a_dist / (a_dist - b_dist));
		}

		a = ProjectWorldSpaceToScreenSpace(a);
		b = ProjectWorldSpaceToScreenSpace(b);

		const tiny3d::Vertex A = { tiny3d::Vector3{ tiny3d::Real(a[0]), tiny3d::Real(a[1]), tiny3d::Real(a[2]) }, tiny3d::Vector2{ tiny3d::Real(0.0f), tiny3d::Real(0.0f) }, job.color };
		const tiny3d::Vertex B = { tiny3d::Vector3{ tiny3d::Real(b[0]), tiny3d::Real(b[1]), tiny3d::Real(b[2]) }, tiny3d::Vector2{ tiny3d::Real(0.0f), tiny3d::Real(0.0f) }, job.color };

		tiny3d::DrawLine(m_dst, &m_zbuf, &m_zbuf, A, B, nullptr, &rect);
	}
}

void platform::T3DRenderDevice::DepthRender(tiny3d::URect rect)
{
	for (tiny3d::UInt y = rect.a.y; y < rect.b.y; ++y) {
		for (tiny3d::UInt x = rect.a.x; x < rect.b.x; ++x) {
			float z = m_zbuf[y * m_dst.GetWidth() + x];
			if (std::isinf(z)) {
				m_dst.SetColor(tiny3d::UPoint{ x, y }, tiny3d::Color{ 255, 0, 0, tiny3d::Color::Solid });
			} else if (std::isnan(z)) {
				m_dst.SetColor(tiny3d::UPoint{ x, y }, tiny3d::Color{ 255, 0, 255, tiny3d::Color::Solid });
			} else {
				tiny3d::Byte i = tiny3d::Byte((z / m_far_z) * 255.0f);
				m_dst.SetColor(tiny3d::UPoint{ x, y }, tiny3d::Color{ i, i, i, tiny3d::Color::Solid});
			}
		}
	}
}

void platform::T3DRenderDevice::Print(tiny3d::URect rect)
{
	const tiny3d::SInt caret_offset = 2;
	tiny3d::Point caret = { caret_offset, caret_offset };
	for (tiny3d::UInt n = 0; n < m_num_2d_items; ++n) {

		const Render2DJob &p = m_2d_queue[n];

		switch (p.type) {
		case Render2DJob::Text:
			tiny3d::DrawChars(m_dst, tiny3d::Point{ caret.x + 1, caret.y + 1 }, caret_offset, p.text.c_str(), tiny3d::UInt(p.text.size()), tiny3d::Color{0,0,0,tiny3d::Color::Solid}, 1, &rect);
			caret = tiny3d::DrawChars(m_dst, caret, caret_offset, p.text.c_str(), tiny3d::UInt(p.text.size()), p.color, 1, &rect);
			break;
		case Render2DJob::TextFree:
			//tiny3d::DrawChars(m_dst, tiny3d::Point{ p.xy.x + 1, p.xy.y + 1 }, p.xy.x, p.text.c_str(), tiny3d::UInt(p.text.size()), tiny3d::Color{0,0,0,tiny3d::Color::Solid}, 1, &rect);
			tiny3d::DrawChars(m_dst, p.xy, p.xy.x, p.text.c_str(), tiny3d::UInt(p.text.size()), p.color, p.scale, &rect);
			break;
		case Render2DJob::Overlay:
			tiny3d::DrawRegion(m_dst, p.dst_rect, *p.overlay, p.src_rect, &rect);
			break;
		case Render2DJob::Line:
			break;
		case Render2DJob::BoxLines:
			break;
		case Render2DJob::BoxFilled:
			break;
		}
	}
}

void platform::T3DRenderDevice::ClearJobBuffers( void )
{
	m_num_3d_items = 0;
	m_num_2d_items = 0;
	m_num_lights   = 0;
}

void platform::T3DRenderDevice::UpdateViewFrustum( void )
{
	const float inv_fov_scalar = 1.0f / m_hfov_scalar;
	mmlVector<3> c[4];
	// NOTE: Top-left
	c[0][0] = -1.0f * m_near_z * inv_fov_scalar;
	c[0][1] = m_aspect_ratio * m_near_z * inv_fov_scalar;
	c[0][2] = m_near_z;
	// NOTE: Top-right
	c[1][0] = 1.0f * m_near_z * inv_fov_scalar;
	c[1][1] = m_aspect_ratio * m_near_z * inv_fov_scalar;
	c[1][2] = m_near_z;
	// NOTE: Bottom-right
	c[2][0] = 1.0f * m_near_z * inv_fov_scalar;
	c[2][1] = -m_aspect_ratio * m_near_z * inv_fov_scalar;
	c[2][2] = m_near_z;
	// NOTE: Bottom-left
	c[3][0] = -1.0f * m_near_z * inv_fov_scalar;
	c[3][1] = -m_aspect_ratio * m_near_z * inv_fov_scalar;
	c[3][2] = m_near_z;

	m_object_space_view_frustum.SetFrustum(mmlVector<3>::Fill(0.0f), c, 4, m_far_z);
}

platform::T3DRenderDevice::T3DRenderDevice( void ) :
	m_dst(), m_zbuf(),
	m_world_to_view(mmlMatrix<4,4>::Identity()), m_view_to_world(mmlMatrix<4,4>::Identity()), m_world_to_view_ptr(&m_world_to_view),
	m_near_z(1.0f), m_far_z(1.0f), m_hfov(1.0f), m_aspect_ratio(1.0f), m_hfov_scalar(1.0f), m_near_plane(mmlVector<3>(0.0, 0.0, double(m_near_z)), retro3d::Transform::GetWorldForward()),
	m_2d_queue(512), m_num_2d_items(0),
	m_3d_queue(512), m_num_3d_items(0),
	m_lights(16), m_num_lights(0),
	m_frames_rendered(0),
	m_skybox(*retro3d::Model::Library.Fetch("Default.Cube.Model").GetShared()),
	m_mip_ratio(4.0f),
	m_depth_render(false),
	m_render_skybox(false)
{
	// NOTE: We want to see the inside of the box.
	m_skybox.ReverseModel();

	// NOTE: Adjust texture coordinates by a half the size of a texel so that they do not accidentally wrap around.
	const float small_val = 0.5f / float(tiny3d::Texture::MaxDimension());
	for (int t = 0; t < m_skybox.t.GetSize(); ++t) {
		m_skybox.t[t][0] = mmlClamp(small_val, m_skybox.t[t][0], 1.0f - small_val);
		m_skybox.t[t][1] = mmlClamp(small_val, m_skybox.t[t][1], 1.0f - small_val);
	}

	// NOTE: It is necessary to use this function as I do not want to repeat all calculations in the constructor.
	SetDepthClip(1.0f, 100.0f, mmlPI/2.0f);
}

platform::T3DRenderDevice::~T3DRenderDevice( void )
{}

//#if defined(__linux) || defined(__linux__) || defined (__LINUX__) || defined (__gnu_linux__)
//	#undef XLIB_THREADS
//	#define XLIB_THREADS 1
//#endif

//#ifdef XLIB_THREADS
//	#include "X11/Xlib.h"
//#endif

bool platform::T3DRenderDevice::Init( void )
{
//#ifdef XLIB_THREADS
//	if (XInitThreads() != True) {
//		std::cout << "[T3DRenderDevice::Init] XInitThreads failed." << std::endl;
//		return false;
//	}
//#endif
	return true;
}

void platform::T3DRenderDevice::SetDepthClip(float near_z, float far_z, float hori_fov)
{
	m_near_z       = mmlMax(0.1f, mmlAbs(near_z));
	m_far_z        = mmlAbs(far_z);
	m_near_plane   = retro3d::Plane(mmlVector<3>(0.0, 0.0, double(m_near_z)), retro3d::Transform::GetWorldForward()); // NOTE: We could just use the view frustum plane instead of this.
	m_hfov         = mmlClamp(0.1f, hori_fov, mmlPI - 0.1f);
	m_aspect_ratio = m_dst.GetWidth() > 0 ? m_dst.GetHeight() / float(m_dst.GetWidth()) : 1.0f;
	m_hfov_scalar  = 1.0f / tanf(m_hfov * 0.5f);
	UpdateViewFrustum();
}

bool platform::T3DRenderDevice::CreateRenderSurface(uint32_t width, uint32_t height)
{
	if (m_dst.Create(tiny3d::UInt(width), tiny3d::UInt(height))) {
		tiny3d::UInt zsize = tiny3d::UInt(width * height);
		m_zbuf.Create(zsize);
		m_dst.Fill({ { 0, 0 }, { m_dst.GetWidth(), m_dst.GetHeight() } }, tiny3d::Color{ 255, 0, 255, tiny3d::Color::Solid });
		for (tiny3d::UInt i = 0; i < zsize; ++i) {
			m_zbuf[i] = std::numeric_limits<float>::infinity();
		}
		SetDepthClip(m_near_z, m_far_z, m_hfov);
		return true;
	}
	std::cout << "[T3DRenderDevice::CreateRenderSurface] Failed to allocate render surface memory." << std::endl;
	return false;
}

void platform::T3DRenderDevice::DestroyRenderSurface( void )
{
	m_dst.Destroy();
	m_zbuf.Destroy();
}

void platform::T3DRenderDevice::SetViewTransform(const mmlMatrix<4,4> &world_to_view)
{
	m_world_to_view     = world_to_view;
	m_world_to_view_ptr = &m_world_to_view;
	m_view_to_world     = mmlInv(world_to_view);
}

void platform::T3DRenderDevice::SetViewTransform(const mmlMatrix<4,4> *world_to_view)
{
	m_world_to_view_ptr = world_to_view;
}

void platform::T3DRenderDevice::SetViewTransform(const retro3d::Camera &camera)
{
	m_world_to_view     = camera.GetViewTransform();
	m_world_to_view_ptr = &m_world_to_view;
	m_view_to_world     = camera.GetTransform()->GetMatrix();
}

mmlMatrix<4,4> platform::T3DRenderDevice::GetViewTransform( void ) const
{
	return *m_world_to_view_ptr;
}

mmlMatrix<4,4> platform::T3DRenderDevice::GetInverseViewTransform( void ) const
{
	return (m_world_to_view_ptr != &m_world_to_view) ? mmlInv(*m_world_to_view_ptr) : m_view_to_world;
}

void platform::T3DRenderDevice::RenderModel(const retro3d::Model &model, const mmlMatrix<4,4> &obj_to_world, retro3d::RenderDevice::LightMode light_mode, retro3d::RenderDevice::RenderMode render_mode)
{
	Render3DJob *job = Add3DJob(Render3DJob::Model, obj_to_world, light_mode);
	if (job != nullptr) {
		job->model       = &model;
		job->render_mode = render_mode;
		job->face_camera = false;
	}
}

void platform::T3DRenderDevice::RenderModel(const retro3d::Model &model, const mmlMatrix<4,4> *obj_to_world, retro3d::RenderDevice::LightMode light_mode, retro3d::RenderDevice::RenderMode render_mode)
{
	Render3DJob *job = Add3DJob(Render3DJob::Model, obj_to_world, light_mode);
	if (job != nullptr) {
		job->model       = &model;
		job->render_mode = render_mode;
		job->face_camera = false;
	}
}

void platform::T3DRenderDevice::RenderDisplayModel(const retro3d::DisplayModel &model, const mmlMatrix<4,4> &obj_to_world, retro3d::RenderDevice::LightMode light_mode, retro3d::RenderDevice::RenderMode render_mode)
{
	Render3DJob *job = Add3DJob(Render3DJob::DisplayModel, obj_to_world, light_mode);
	if (job != nullptr) {
		job->display_model = &model;
		job->render_mode   = render_mode;
	}
}

void platform::T3DRenderDevice::RenderSpritePlane(const retro3d::Model &sprite_plane, const mmlMatrix<4,4> &obj_to_world, retro3d::RenderDevice::LightMode light_mode, retro3d::RenderDevice::RenderMode render_mode)
{	
	Render3DJob *job = Add3DJob(Render3DJob::Model, obj_to_world, light_mode);
	if (job != nullptr) {
		job->model       = &sprite_plane;
		job->render_mode = render_mode;
		job->face_camera = true;
	}
}

void platform::T3DRenderDevice::RenderSpritePlane(const retro3d::Model &sprite_plane, const mmlMatrix<4,4> *obj_to_world, retro3d::RenderDevice::LightMode light_mode, retro3d::RenderDevice::RenderMode render_mode)
{
	Render3DJob *job = Add3DJob(Render3DJob::Model, obj_to_world, light_mode);
	if (job != nullptr) {
		job->model       = &sprite_plane;
		job->render_mode = render_mode;
		job->face_camera = true;
	}
}

void platform::T3DRenderDevice::RenderLight(const retro3d::Light &light)
{
	if (m_num_lights >= m_lights.GetSize()) {
		RenderText(retro_text_err("light overflow\n"));
	} else {
		m_lights[m_num_lights++] = light;
	}
}

void platform::T3DRenderDevice::RenderAABB(const retro3d::AABB &aabb, const mmlMatrix<4,4> &obj_to_world, const mmlVector<3> &color, bool world_axis_aligned)
{
	Render3DJob *job = Add3DJob(Render3DJob::AABB, obj_to_world, LightMode_Fullbright);
	if (job != nullptr) {
		job->obj_aabb           = aabb;
		job->color              = tiny3d::Color{ tiny3d::Byte(color[0] * 255.0f), tiny3d::Byte(color[1] * 255.0f), tiny3d::Byte(color[2] * 255.0f), tiny3d::Color::Solid };
		job->world_axis_aligned = world_axis_aligned;
	}
}

void platform::T3DRenderDevice::RenderAABB(const retro3d::AABB &aabb, const mmlMatrix<4,4> *obj_to_world, const mmlVector<3> &color, bool world_axis_aligned)
{
	Render3DJob *job = Add3DJob(Render3DJob::AABB, obj_to_world, LightMode_Fullbright);
	if (job != nullptr) {
		job->obj_aabb           = aabb;
		job->color              = tiny3d::Color{ tiny3d::Byte(color[0] * 255.0f), tiny3d::Byte(color[1] * 255.0f), tiny3d::Byte(color[2] * 255.0f), tiny3d::Color::Solid };
		job->world_axis_aligned = world_axis_aligned;
	}
}

void platform::T3DRenderDevice::RenderViewFrustum(const retro3d::Frustum &frustum, const mmlMatrix<4,4> &obj_to_world, const mmlVector<3> &color)
{
	Render3DJob *job = Add3DJob(Render3DJob::Frustum, obj_to_world, LightMode_Fullbright);
	if (job != nullptr) {
		job->obj_view_frustum = frustum;
		job->color            = tiny3d::Color{ tiny3d::Byte(color[0] * 255.0f), tiny3d::Byte(color[1] * 255.0f), tiny3d::Byte(color[2] * 255.0f), tiny3d::Color::Solid };
	}
}

void platform::T3DRenderDevice::RenderViewFrustum(const retro3d::Frustum &frustum, const mmlMatrix<4,4> *obj_to_world, const mmlVector<3> &color)
{
	Render3DJob *job = Add3DJob(Render3DJob::Frustum, obj_to_world, LightMode_Fullbright);
	if (job != nullptr) {
		job->obj_view_frustum = frustum;
		job->color            = tiny3d::Color{ tiny3d::Byte(color[0] * 255.0f), tiny3d::Byte(color[1] * 255.0f), tiny3d::Byte(color[2] * 255.0f), tiny3d::Color::Solid };
	}
}

void platform::T3DRenderDevice::RenderOverlay(const tiny3d::Overlay &overlay, const retro3d::Rect &src_region, const retro3d::Rect &dst_region)
{
	Render2DJob *job = Add2DJob(Render2DJob::Overlay, tiny3d::Point{ 0,0 }, tiny3d::Color{ 0,0,0,tiny3d::Color::Transparent });
	if (job != nullptr) {
		job->overlay = &overlay;
		job->dst_rect = dst_region;
		job->src_rect = src_region;
	}
}

retro3d::RenderDevice &platform::T3DRenderDevice::RenderText(const std::string &str, const mmlVector<3> &color)
{
	Render2DJob *job = Add2DJob(Render2DJob::Text, tiny3d::Point{ 0,0 }, tiny3d::Color{ tiny3d::Byte(color[0] * 255.0f), tiny3d::Byte(color[1] * 255.0f), tiny3d::Byte(color[2] * 255.0f), tiny3d::Color::Solid });
	if (job != nullptr) {
		job->text = str;
	}
	return *this;
}

retro3d::RenderDevice &platform::T3DRenderDevice::RenderText(int64_t n, const mmlVector<3> &color)
{
	std::ostringstream sout;
	sout << n;
	return RenderText(sout.str(), color);
}

retro3d::RenderDevice &platform::T3DRenderDevice::RenderText(uint64_t n, const mmlVector<3> &color)
{
	std::ostringstream sout;
	sout << n;
	return RenderText(sout.str(), color);
}

retro3d::RenderDevice &platform::T3DRenderDevice::RenderText(double n, const mmlVector<3> &color)
{
	std::ostringstream sout;
	sout << std::fixed << std::setprecision(3) << n;
	return RenderText(sout.str(), color);
}

retro3d::RenderDevice &platform::T3DRenderDevice::RenderText(const std::string &str, tiny3d::Point xy, uint32_t scale, const mmlVector<3> &color)
{
	Render2DJob *job = Add2DJob(Render2DJob::TextFree, xy, tiny3d::Color{ tiny3d::Byte(color[0] * 255.0f), tiny3d::Byte(color[1] * 255.0f), tiny3d::Byte(color[2] * 255.0f), tiny3d::Color::Solid });
	if (job != nullptr) {
		job->text = str;
		job->scale = scale;
	}
	return *this;
}

retro3d::RenderDevice &platform::T3DRenderDevice::RenderText(int64_t n, tiny3d::Point xy, uint32_t scale, const mmlVector<3> &color)
{
	std::ostringstream sout;
	sout << n;
	return RenderText(sout.str(), xy, scale, color);
}

retro3d::RenderDevice &platform::T3DRenderDevice::RenderText(uint64_t n, tiny3d::Point xy, uint32_t scale, const mmlVector<3> &color)
{
	std::ostringstream sout;
	sout << n;
	return RenderText(sout.str(), xy, scale, color);
}

retro3d::RenderDevice &platform::T3DRenderDevice::RenderText(double n, tiny3d::Point xy, uint32_t scale, const mmlVector<3> &color)
{
	std::ostringstream sout;
	sout << std::fixed << std::setprecision(3) << n;
	return RenderText(sout.str(), xy, scale, color);
}

void platform::T3DRenderDevice::ExecuteJobs(tiny3d::SInt thread_num, tiny3d::SInt dst_height_per_thread, tiny3d::SInt video_height_per_thread, bool update_video_out)
{
	tiny3d::URect thread_mask = { { tiny3d::UInt(0), tiny3d::UInt(thread_num * dst_height_per_thread)   }, { tiny3d::UInt(m_dst.GetWidth()),         tiny3d::UInt((thread_num + 1) * dst_height_per_thread)   } };
	tiny3d::URect out_mask    = { { tiny3d::UInt(0), tiny3d::UInt(thread_num * video_height_per_thread) }, { tiny3d::UInt(GetEngine()->GetVideo()->GetWindowWidth()), tiny3d::UInt((thread_num + 1) * video_height_per_thread) } };
	// TODO: Clearing buffers HERE
	Render(thread_mask);
	if (m_depth_render == true) {
		DepthRender(thread_mask);
	} else if (m_render_skybox == true) { // render sky only if enabled AND depth rendering not enabled
		RenderSky(thread_mask);
	}
	Print(thread_mask);
	if (update_video_out == true) {
		GetEngine()->GetVideo()->FromImage(m_dst, out_mask);
	}
	ClearBuffers(thread_mask); // TODO, NOTE, BUG, HACK: Do NOT clear buffers here (I am only doing this to enable Debug_RenderTriangle).
}

void platform::T3DRenderDevice::FinishRender(bool update_video_out)
{
	// update the inverse world_to_view if world_to_view may have been changed outside the renderer
	m_view_to_world = GetInverseViewTransform();

	const tiny3d::SInt num_threads             = tiny3d::SInt(std::thread::hardware_concurrency());
	const tiny3d::SInt dst_height_per_thread   = tiny3d::SInt(m_dst.GetHeight()) / num_threads;
	const tiny3d::SInt video_height_per_thread = tiny3d::SInt(GetEngine()->GetVideo()->GetWindowHeight()) / num_threads;

	m_render_threads.Create(num_threads);

	for (tiny3d::SInt thread = 0; thread < num_threads; ++thread) {
		m_render_threads[thread] = std::thread(&platform::T3DRenderDevice::ExecuteJobs, this, thread, dst_height_per_thread, video_height_per_thread, update_video_out);
	}
	for (tiny3d::SInt thread = 0; thread < m_render_threads.GetSize(); ++thread) {
		m_render_threads[thread].join();
	}

	ClearJobBuffers();
	++m_frames_rendered;
}

void platform::T3DRenderDevice::ToggleDepthView( void )
{
	m_depth_render = !m_depth_render;
}

bool platform::T3DRenderDevice::DepthViewEnabled( void ) const
{
	return m_depth_render;
}

uint32_t platform::T3DRenderDevice::GetRenderWidth( void ) const
{
	return uint32_t(m_dst.GetWidth());
}

uint32_t platform::T3DRenderDevice::GetRenderHeight( void ) const
{
	return uint32_t(m_dst.GetHeight());
}

bool platform::T3DRenderDevice::ToTexture(retro3d::Texture &tex) const
{
	return tex.FromImage(m_dst);
}

bool LoadSkybox(retro3d::Texture &t, const tiny3d::Image &i)
{
	bool ret_val = t.FromImage(i);
	t.SetBlendMode1(tiny3d::Color::Emissive);
	t.SetBlendMode2(tiny3d::Color::Emissive);
	return ret_val;
}

bool platform::T3DRenderDevice::SetSkyBox(const tiny3d::Image &right, const tiny3d::Image &left, const tiny3d::Image &up, const tiny3d::Image &down, const tiny3d::Image &front, const tiny3d::Image &back)
{
	for (int i = 0; i < m_skybox.m.GetSize(); ++i) {
		m_skybox.m[i].td.New();
	}
	return
		LoadSkybox(*m_skybox.m[Side_Right].td.GetShared(), right) &&
		LoadSkybox(*m_skybox.m[Side_Left].td.GetShared(),  left)  &&
		LoadSkybox(*m_skybox.m[Side_Up].td.GetShared(),    up)    &&
		LoadSkybox(*m_skybox.m[Side_Down].td.GetShared(),  down)  &&
		LoadSkybox(*m_skybox.m[Side_Back].td.GetShared(),  back)  &&
		LoadSkybox(*m_skybox.m[Side_Front].td.GetShared(), front);
}

void platform::T3DRenderDevice::ToggleSkybox( void )
{
	m_render_skybox = !m_render_skybox;
}

bool platform::T3DRenderDevice::SkyboxEnabled( void ) const
{
	return m_render_skybox;
}

float platform::T3DRenderDevice::GetHorizontalFieldOfView( void ) const
{
	// NOTE: Returns stored horizontal FOV value.
	//       Do not calculate the angle between the left and right view frustum planes because there is a slight difference between the stored value and the computed value.
	//       If the computed value is used recursively in SetHorizontalFieldOfView/SetDepthClip you may get instable behavior.

	// NOTE: Retrieving vertices from view frustum is dependent on the order in which they are used to construct the frustum (see UpdateViewFrustum).
//	const mmlVector<3> left  = mmlSurfaceNormal(mmlVector<3>::Fill(0.0f), m_object_space_view_frustum.GetPortalVertex(0), m_object_space_view_frustum.GetPortalVertex(3));
//	const mmlVector<3> right = mmlSurfaceNormal(mmlVector<3>::Fill(0.0f), m_object_space_view_frustum.GetPortalVertex(1), m_object_space_view_frustum.GetPortalVertex(2));
//	return mmlAng(left, right);

	return m_hfov;
}

float platform::T3DRenderDevice::GetVerticalFieldOfView( void ) const
{
	// NOTE: Measures angle between top and bottom planes of view frustum.

	// NOTE: Retrieving vertices from view frustum is dependent on the order in which they are used to construct the frustum (see UpdateViewFrustum).
	const mmlVector<3> up   = mmlSurfaceNormal(mmlVector<3>::Fill(0.0f), m_object_space_view_frustum.GetPortalVertex(0), m_object_space_view_frustum.GetPortalVertex(1));
	const mmlVector<3> down = mmlSurfaceNormal(mmlVector<3>::Fill(0.0f), m_object_space_view_frustum.GetPortalVertex(3), m_object_space_view_frustum.GetPortalVertex(2));
	return mmlAng(up, down);
}

void platform::T3DRenderDevice::SetHorizontalFieldOfView(float hori_fov)
{
	SetDepthClip(m_near_z, m_far_z, hori_fov);
}

retro3d::Frustum platform::T3DRenderDevice::GetViewFrustum( void ) const
{
	return m_object_space_view_frustum.ApplyTransform(m_view_to_world);
}

void platform::T3DRenderDevice::Debug_RenderTriangle(const retro3d::Vertex &A, const retro3d::Vertex &B, const retro3d::Vertex &C, const mmlMatrix<4,4> &obj_to_world, const mmlMatrix<4,4> &world_to_view, const retro3d::Texture *texture, LightMode light_mode)
{
	const mmlMatrix<4,4> obj_to_view = world_to_view * obj_to_world;

	retro3d::Vertex a, b, c;

	// Transform model space vertices to view space
	a.v = A.v * obj_to_view;
	b.v = B.v * obj_to_view;
	c.v = C.v * obj_to_view;

	// Early exit if all triangles are behind near clipping plane
	if (a.v[2] < m_near_z && b.v[2] < m_near_z && c.v[2] < m_near_z) { return; }

	// Apply lighting to vertices
	mmlVector<3> an, bn, cn;
	an = A.c;
	bn = B.c;
	cn = C.c;

	switch (light_mode) {
	case LightMode_Fullbright:
	case LightMode_Lightmap:
		a.c = b.c = c.c = mmlVector<3>::Fill(1.0f);
		break;
	case LightMode_Dynamic:
		a.c = b.c = c.c = mmlVector<3>::Fill(0.0f);
		for (tiny3d::UInt l = 0; l < m_num_lights; ++l) {
			if (m_lights[l].activation_aabb.Contains(A.v) || m_lights[l].activation_aabb.Contains(B.v) || m_lights[l].activation_aabb.Contains(C.v)) {
				float da = m_lights[l].CalculateIntensityAtVertex(A.v, an);
				float db = m_lights[l].CalculateIntensityAtVertex(B.v, bn);
				float dc = m_lights[l].CalculateIntensityAtVertex(C.v, cn);
				a.c = mmlMin(a.c + da * m_lights[l].color, mmlVector<3>::Fill(1.0f));
				b.c = mmlMin(b.c + db * m_lights[l].color, mmlVector<3>::Fill(1.0f));
				c.c = mmlMin(c.c + dc * m_lights[l].color, mmlVector<3>::Fill(1.0f));
			}
		}
		break;
	case LightMode_NormalsAsColor:
		a.c = an;
		b.c = bn;
		c.c = cn;
		break;
	}

	a.t = A.t;
	b.t = B.t;
	c.t = C.t;

	// Clip view space triangles against near plane
	retro3d::Vertex out[4];
	tiny3d::Vertex final_vtx[4];
	const tiny3d::UInt num_out = m_near_plane.Clip(a, b, c, out);

	// Early exit if clipping does not result in a valid geometric shape
	if (num_out <= 2) { return; }

	// Project view space vertices to screen space
	for (tiny3d::UInt i = 0; i < num_out; ++i) {
		out[i].v = ProjectWorldSpaceToScreenSpace(out[i].v);
	}

	// Early exit if resulting shape does not face camera
	if (IsFront(out[0].v, out[1].v, out[2].v) == false) { return; }

	for (tiny3d::UInt i = 0; i < num_out; ++i) {
		final_vtx[i].v = tiny3d::Vector3(tiny3d::Real(out[i].v[0]), tiny3d::Real(out[i].v[1]), tiny3d::Real(out[i].v[2]));
		final_vtx[i].t = tiny3d::Vector2(tiny3d::Real(out[i].t[0]), tiny3d::Real(out[i].t[1]));
		final_vtx[i].c = tiny3d::Color{ tiny3d::Byte(out[i].c[0] * 255.0f), tiny3d::Byte(out[i].c[1] * 255.0f), tiny3d::Byte(out[i].c[2] * 255.0f), tiny3d::Color::Solid };
	}

	const tiny3d::Texture *tex_mip = texture != nullptr ? (*texture)[DetermineMipLevel(*texture, out[0], out[1], out[2])] : nullptr;

	switch (num_out) {
	case 4:
		tiny3d::DrawTriangle(m_dst, &m_zbuf, &m_zbuf, final_vtx[0], final_vtx[1], final_vtx[2], tex_mip);
		tiny3d::DrawTriangle(m_dst, &m_zbuf, &m_zbuf, final_vtx[0], final_vtx[2], final_vtx[3], tex_mip);
		break;
	case 3:
		tiny3d::DrawTriangle(m_dst, &m_zbuf, &m_zbuf, final_vtx[0], final_vtx[1], final_vtx[2], tex_mip);
		break;
	default: break;
	}
}
