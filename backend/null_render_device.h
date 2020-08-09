#ifndef RETRO_NULL_RENDER_H
#define RETRO_NULL_RENDER_H

#include "../frontend/retro_render_device.h"

namespace platform
{

class NullRenderDevice : public retro3d::RenderDevice
{
public:
	bool Init( void ) override { return true; }

	void SetDepthClip(float, float, float) override {}
	bool CreateRenderSurface(uint32_t, uint32_t) override { return true; }
	void DestroyRenderSurface( void ) override {}
	void SetViewTransform(const mmlMatrix<4,4>&) override {}
	void SetViewTransform(const mmlMatrix<4,4>*) override {}
	void SetViewTransform(const retro3d::Camera&) override {}
	mmlMatrix<4,4> GetViewTransform( void ) const override { return mmlMatrix<4,4>::Identity(); }
	mmlMatrix<4,4> GetInverseViewTransform( void ) const override { return mmlMatrix<4,4>::Identity(); }
	void RenderModel(const retro3d::Model&, const mmlMatrix<4,4>&, LightMode, RenderMode) override {}
	void RenderModel(const retro3d::Model&, const mmlMatrix<4,4>*, LightMode, RenderMode) override {}
	void RenderDisplayModel(const retro3d::DisplayModel&, const mmlMatrix<4,4>&, LightMode, RenderMode) override {}
	void RenderSpritePlane(const retro3d::Model&, const mmlMatrix<4,4>&, LightMode, RenderMode) override {}
	void RenderSpritePlane(const retro3d::Model&, const mmlMatrix<4,4>*, LightMode, RenderMode) override {}
	void RenderLight(const retro3d::Light&) override {}
	void RenderAABB(const retro3d::AABB&, const mmlMatrix<4,4>&, const mmlVector<3>&, bool) override {}
	void RenderAABB(const retro3d::AABB&, const mmlMatrix<4,4>*, const mmlVector<3>&, bool) override {}
	void RenderViewFrustum(const retro3d::Frustum&, const mmlMatrix<4,4>&, const mmlVector<3>&) override {}
	void RenderViewFrustum(const retro3d::Frustum&, const mmlMatrix<4,4>*, const mmlVector<3>&) override {}
	void RenderOverlay(const tiny3d::Overlay&, const retro3d::Rect&, const retro3d::Rect&) override {}
	RenderDevice &RenderText(const std::string&, const mmlVector<3>&) override { return *this; }
	RenderDevice &RenderText(int64_t, const mmlVector<3>&) override { return *this; }
	RenderDevice &RenderText(uint64_t, const mmlVector<3>&) override { return *this; }
	RenderDevice &RenderText(double, const mmlVector<3>&) override { return *this; }
	RenderDevice &RenderText(const std::string&, tiny3d::Point, uint32_t, const mmlVector<3>&) override { return *this; }
	RenderDevice &RenderText(int64_t, tiny3d::Point, uint32_t, const mmlVector<3>&) override { return *this; }
	RenderDevice &RenderText(uint64_t, tiny3d::Point, uint32_t, const mmlVector<3>&) override { return *this; }
	RenderDevice &RenderText(double, tiny3d::Point, uint32_t, const mmlVector<3>&) override { return *this; }
	void FinishRender(bool = true) override {}
	void ToggleDepthView( void ) override {}
	bool DepthViewEnabled() const override { return false; }
	uint32_t GetRenderWidth( void ) const override { return 0; }
	uint32_t GetRenderHeight( void ) const override { return 0; }
	bool ToTexture(retro3d::Texture&) const override { return false; }
	bool SetSkyBox(const tiny3d::Image&, const tiny3d::Image&, const tiny3d::Image&, const tiny3d::Image&, const tiny3d::Image&, const tiny3d::Image&) override { return false; }
	void ToggleSkybox( void ) override {}
	bool SkyboxEnabled( void ) const override { return false; }
	float GetHorizontalFieldOfView( void ) const override { return 0.0f; }
	float GetVerticalFieldOfView( void ) const override { return 0.0f; }
	void  SetHorizontalFieldOfView(float) override {}
	retro3d::Frustum GetViewFrustum( void ) const override { return retro3d::Frustum(); }

	void Debug_RenderTriangle(const retro3d::Vertex&, const retro3d::Vertex&, const retro3d::Vertex&, const mmlMatrix<4,4>&, const mmlMatrix<4,4>&, const retro3d::Texture*, LightMode) override {};
};

}

#endif // RETRO_NULL_RENDER_H
