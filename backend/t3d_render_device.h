#ifndef RETRO_T3D_RENDER_H
#define RETRO_T3D_RENDER_H

#include <thread>
#include "../frontend/retro_render_device.h"
#include "../api/tiny3d/tiny_image.h"

namespace platform
{

class T3DRenderDevice : public retro3d::RenderDevice
{
private:
	struct Render3DJob
	{
		enum Type {
			Model, DisplayModel, Text, AABB, Frustum
		};

		const retro3d::Model        *model;
		const retro3d::DisplayModel *display_model;
		RenderMode                   render_mode;
		std::string                  text;
		retro3d::AABB                obj_aabb;
		retro3d::Frustum             obj_view_frustum;
		tiny3d::Color                color;
		Type                         type;
		mmlMatrix<4,4>               obj_to_world;
		mmlMatrix<4,4>               world_to_obj;
		const mmlMatrix<4,4>        *obj_to_world_ptr;
		LightMode                    light_mode;
		bool                         world_axis_aligned;
		bool                         face_camera;
	};

	struct Render2DJob // in separate array
	{
		enum Type {
			Text, AABB
		};

		std::string   text;
		tiny3d::Color color;
		tiny3d::Point xy;
		Type          type;
		bool          to_console;
	};

	enum BoxSides
	{
		Side_Right,
		Side_Left,
		Side_Up,
		Side_Down,
		Side_Back,
		Side_Front,
		Side_Count
	};

	struct Jobs
	{
		tiny3d::Array<Render2DJob>     m_print_queue;
		tiny3d::UInt                   m_num_print_items;
		tiny3d::Array<Render3DJob>     m_render_queue;
		tiny3d::UInt                   m_num_render_items;
		tiny3d::Array<retro3d::Light>  m_lights;
		tiny3d::UInt                   m_num_lights;
	};

private:
	tiny3d::Image                  m_dst;
	tiny3d::Array<float>           m_zbuf;
	mmlMatrix<4,4>                 m_world_to_view;
	mmlMatrix<4,4>                 m_view_to_world;
	const mmlMatrix<4,4>          *m_world_to_view_ptr;
	float                          m_near_z;
	float                          m_far_z;
	float                          m_hfov;
	float                          m_aspect_ratio;
	float                          m_hfov_scalar;
	retro3d::Plane                 m_near_plane;
	tiny3d::Array<Render2DJob>     m_print_queue;
	tiny3d::UInt                   m_num_print_items;
	tiny3d::Array<Render3DJob>     m_render_queue;
	tiny3d::UInt                   m_num_render_items;
	tiny3d::Array<retro3d::Light>  m_lights;
	tiny3d::UInt                   m_num_lights;
	retro3d::Array<std::thread>    m_render_threads;
	tiny3d::UInt                   m_frames_rendered;
	retro3d::Model                 m_skybox;
	retro3d::Frustum               m_object_space_view_frustum;
	float                          m_mip_ratio;
	bool                           m_depth_render;
	bool                           m_render_skybox;

private:
	Render3DJob  *Add3DJob(Render3DJob::Type type, const mmlMatrix<4,4> &obj_to_world, LightMode light_mode);
	Render3DJob  *Add3DJob(Render3DJob::Type type, const mmlMatrix<4,4> *obj_to_world, LightMode light_mode);
	Render2DJob  *Add2DJob(Render2DJob::Type type, tiny3d::Point xy, tiny3d::Color color);
	mmlVector<3>  ProjectWorldSpaceToScreenSpace(const mmlVector<3> &v) const;
	bool          IsFront(const mmlVector<3> &a, const mmlVector<3> &b, const mmlVector<3> &c) const;
	tiny3d::UInt  ClipNear(const retro3d::Vertex &a, const retro3d::Vertex &b, const retro3d::Vertex &c, retro3d::Vertex (&out)[4], bool &clipped) const;
	void          ClearBuffers(tiny3d::URect rect);
	tiny3d::UInt  DetermineMipLevel(const retro3d::Texture &mips, const retro3d::Vertex &ss_vert_a, const retro3d::Vertex &ss_vert_b, const retro3d::Vertex &ss_vert_c) const;
	void          Render(tiny3d::URect rect);
	void          RenderSky(tiny3d::URect rect);
	void          RenderModel(const tiny3d::URect &rect, const Render3DJob &job, tiny3d::Array< retro3d::Light > &lights);
	void          RenderDisplay(const tiny3d::URect &rect, const Render3DJob &job, tiny3d::Array< retro3d::Light > &lights);
	void          RenderAABB(const tiny3d::URect &rect, const Render3DJob &job, tiny3d::Array< retro3d::Light > &lights);
	void          RenderFrustum(const tiny3d::URect &rect, const Render3DJob &job, tiny3d::Array< retro3d::Light > &lights);
	void          DepthRender(tiny3d::URect rect);
	void          Print(tiny3d::URect rect);
	void          ExecuteJobs(tiny3d::SInt thread_num, tiny3d::SInt dst_height_per_thread, tiny3d::SInt video_height_per_thread, bool update_video_out);
	void          ClearJobBuffers( void );
	void          UpdateViewFrustum( void );

public:
	T3DRenderDevice( void );
	~T3DRenderDevice( void ) override;

	bool Init( void ) override;

	void SetDepthClip(float near_z, float far_z, float hori_fov) override;
	bool CreateRenderSurface(uint32_t width, uint32_t height) override;
	void DestroyRenderSurface( void ) override;
	void SetViewTransform(const mmlMatrix<4,4> &world_to_view) override;
	void SetViewTransform(const mmlMatrix<4,4> *world_to_view) override;
	void SetViewTransform(const retro3d::Camera &camera) override;
	mmlMatrix<4,4> GetViewTransform( void ) const override;
	mmlMatrix<4,4> GetInverseViewTransform( void ) const override;
	void RenderModel(const retro3d::Model &model, const mmlMatrix<4,4> &obj_to_world, LightMode light_mode, RenderMode render_mode = RenderMode_Polygons) override;
	void RenderModel(const retro3d::Model &model, const mmlMatrix<4,4> *obj_to_world, LightMode light_mode, RenderMode render_mode = RenderMode_Polygons) override;
	void RenderDisplayModel(const retro3d::DisplayModel &model, const mmlMatrix<4, 4> &obj_to_world, LightMode light_mode, RenderMode render_mode = RenderMode_Polygons) override;
	void RenderSpritePlane(const retro3d::Model &sprite_plane, const mmlMatrix<4,4> &obj_to_world, LightMode light_mode, RenderMode render_mode = RenderMode_Polygons) override;
	void RenderSpritePlane(const retro3d::Model &sprite_plane, const mmlMatrix<4,4> *obj_to_world, LightMode light_mode, RenderMode render_mode = RenderMode_Polygons) override;
	void RenderLight(const retro3d::Light &light) override;
	void RenderAABB(const retro3d::AABB &aabb, const mmlMatrix<4,4> &obj_to_world, const mmlVector<3> &color = mmlVector<3>(0.0, 1.0, 0.0), bool world_axis_aligned = true) override;
	void RenderAABB(const retro3d::AABB &aabb, const mmlMatrix<4,4> *obj_to_world, const mmlVector<3> &color = mmlVector<3>(0.0, 1.0, 0.0), bool world_axis_aligned = true) override;
	void RenderViewFrustum(const retro3d::Frustum &frustum, const mmlMatrix<4,4> &obj_to_world, const mmlVector<3> &color = mmlVector<3>(0.5, 0.5, 0.5)) override;
	void RenderViewFrustum(const retro3d::Frustum &frustum, const mmlMatrix<4,4> *obj_to_world, const mmlVector<3> &color = mmlVector<3>(0.5, 0.5, 0.5)) override;
	RenderDevice &RenderText(const std::string &str, const mmlVector<3> &color) override;
	RenderDevice &RenderText(int64_t n, const mmlVector<3> &color) override;
	RenderDevice &RenderText(uint64_t n, const mmlVector<3> &color) override;
	RenderDevice &RenderText(double n, const mmlVector<3> &color) override;
	void FinishRender(bool update_video_out = true) override;
	void ToggleDepthView( void ) override;
	bool DepthViewEnabled( void ) const override;
	uint32_t GetRenderWidth( void ) const override;
	uint32_t GetRenderHeight( void ) const override;
	bool ToTexture(retro3d::Texture &tex) const override;
	bool SetSkyBox(const tiny3d::Image &right, const tiny3d::Image &left, const tiny3d::Image &up, const tiny3d::Image &down, const tiny3d::Image &front, const tiny3d::Image &back) override;
	void ToggleSkybox( void ) override;
	bool SkyboxEnabled( void ) const override;
	float GetHorizontalFieldOfView( void ) const override;
	float GetVerticalFieldOfView( void ) const override;
	void SetHorizontalFieldOfView(float hori_fov) override;
	retro3d::Frustum GetViewFrustum( void ) const override;

	void Debug_RenderTriangle(const retro3d::Vertex &a, const retro3d::Vertex &b, const retro3d::Vertex &c, const mmlMatrix<4,4> &obj_to_world, const mmlMatrix<4,4> &world_to_view, const retro3d::Texture *texture, LightMode light_mode) override;
};

}

#endif // RETRO_T3D_RENDER_H
