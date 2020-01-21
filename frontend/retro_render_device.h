#ifndef RETRO_RENDER_DEVICE_H
#define RETRO_RENDER_DEVICE_H

#include "../api/tiny3d/tiny3d.h"

#include "retro_device.h"
#include "../common/MiniLib/MML/mmlVector.h"
#include "../common/MiniLib/MML/mmlMatrix.h"
#include "../graphics/retro_model.h"
#include "../common/retro_geom.h"
#include "../graphics/retro_light.h"
#include "../graphics/retro_camera.h"

#define retro_text_std(msg) (msg), mmlVector<3>::Fill(1.0f)
#define retro_text_warn(msg) (msg), mmlVector<3>(1.0, 1.0, 0.0)
#define retro_text_val(msg) (msg), mmlVector<3>(0.0, 1.0, 1.0)
#define retro_text_err(msg) (msg), mmlVector<3>(1.0, 0.0, 0.0)

namespace retro3d
{

class RenderDevice : public retro3d::Device
{
public:
	enum RenderMode
	{
		RenderMode_Points,
		RenderMode_Lines,
		RenderMode_Polygons
	};

	enum LightMode
	{
		LightMode_Fullbright,     // All lights = (1,1,1)
		LightMode_Dynamic,        // All vertices are processed by all intersecting lights
		LightMode_NormalsAsColor, // Normal channel is interpreted as color channel
		LightMode_Lightmap        // Fullbright + lightmap
	};

public:
	virtual ~RenderDevice( void );

	virtual bool Init( void ) = 0;

	virtual void SetDepthClip(float near_z, float far_z, float hori_fov) = 0;
	virtual bool CreateRenderSurface(uint32_t width, uint32_t height) = 0;
	virtual void DestroyRenderSurface( void ) = 0;
	virtual void SetViewTransform(const mmlMatrix<4,4> &world_to_view) = 0;
	virtual void SetViewTransform(const mmlMatrix<4,4> *world_to_view) = 0;
	virtual void SetViewTransform(const retro3d::Camera &camera) = 0;
	virtual mmlMatrix<4,4> GetViewTransform( void ) const = 0;
	virtual mmlMatrix<4,4> GetInverseViewTransform( void ) const = 0;
	virtual void RenderModel(const retro3d::Model &model, const mmlMatrix<4,4> &obj_to_world, LightMode light_mode, RenderMode render_mode = RenderMode_Polygons) = 0;
	virtual void RenderModel(const retro3d::Model &model, const mmlMatrix<4,4> *obj_to_world, LightMode light_mode, RenderMode render_mode = RenderMode_Polygons) = 0;
	virtual void RenderDisplayModel(const retro3d::DisplayModel &model, const mmlMatrix<4,4> &obj_to_world, LightMode light_mode, RenderMode render_mode = RenderMode_Polygons) = 0;
	virtual void RenderSpritePlane(const retro3d::Model &sprite_plane, const mmlMatrix<4,4> &obj_to_world, LightMode light_mode, RenderMode render_mode = RenderMode_Polygons) = 0;
	virtual void RenderSpritePlane(const retro3d::Model &sprite_plane, const mmlMatrix<4,4> *obj_to_world, LightMode light_mode, RenderMode render_mode = RenderMode_Polygons) = 0;
	virtual void RenderLight(const retro3d::Light &light) = 0;
	virtual void RenderAABB(const retro3d::AABB &aabb, const mmlMatrix<4,4> &obj_to_world, const mmlVector<3> &color = mmlVector<3>(0.0, 1.0, 0.0), bool world_axis_aligned = true) = 0;
	virtual void RenderAABB(const retro3d::AABB &aabb, const mmlMatrix<4,4> *obj_to_world, const mmlVector<3> &color = mmlVector<3>(0.0, 1.0, 0.0), bool world_axis_aligned = true) = 0;
	virtual void RenderViewFrustum(const retro3d::Frustum &frustum, const mmlMatrix<4,4> &obj_to_world, const mmlVector<3> &color = mmlVector<3>::Fill(0.5f)) = 0;
	virtual void RenderViewFrustum(const retro3d::Frustum &frustum, const mmlMatrix<4,4> *obj_to_world, const mmlVector<3> &color = mmlVector<3>::Fill(0.5f)) = 0;
	virtual RenderDevice &RenderText(const std::string &str, const mmlVector<3> &color = mmlVector<3>::Fill(1.0f)) = 0;
	virtual RenderDevice &RenderText(int64_t n, const mmlVector<3> &color = mmlVector<3>::Fill(1.0f)) = 0;
	virtual RenderDevice &RenderText(uint64_t n, const mmlVector<3> &color = mmlVector<3>::Fill(1.0f)) = 0;
	virtual RenderDevice &RenderText(double n, const mmlVector<3> &color = mmlVector<3>::Fill(1.0f)) = 0;
	virtual void FinishRender(bool update_video_out = true) = 0;
	virtual void ToggleDepthView( void ) = 0;
	virtual bool DepthViewEnabled( void ) const = 0;
	virtual uint32_t GetRenderWidth( void ) const = 0;
	virtual uint32_t GetRenderHeight( void ) const = 0;
	virtual bool ToTexture(retro3d::Texture &tex) const = 0;
	virtual bool SetSkyBox(const tiny3d::Image &left, const tiny3d::Image &right, const tiny3d::Image &up, const tiny3d::Image &down, const tiny3d::Image &front, const tiny3d::Image &back) = 0;
	virtual void ToggleSkybox( void ) = 0;
	virtual bool SkyboxEnabled( void ) const = 0;
	virtual float GetHorizontalFieldOfView( void ) const = 0;
	virtual float GetVerticalFieldOfView( void ) const = 0;
	virtual retro3d::Frustum GetViewFrustum( void ) const = 0;
//	virtual void ToDataInterchangeFormat(retro3d::RenderDevice &data) const;

	virtual void Debug_RenderTriangle(const retro3d::Vertex &a, const retro3d::Vertex &b, const retro3d::Vertex &c, const mmlMatrix<4,4> &obj_to_world, const mmlMatrix<4,4> &world_to_view, const retro3d::Texture *texture, LightMode light_mode) = 0;
};

}

#endif // RETRO_RENDERDEVICE_H
