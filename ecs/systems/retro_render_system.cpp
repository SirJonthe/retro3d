#include "retro_render_system.h"
#include "../retro_entity.h"
#include "../../backend/null_render_device.h"
#include "../../backend/t3d_render_device.h"

void retro3d::RenderSystem::OnUpdate(retro3d::RenderComponent &c)
{
	// Add geometry (or reference) to SCENE data structure
/*	m_frustum = GetEngine()->GetCamera()->GetViewFrustum();
	mtlShared<retro3d::Model> model = c.GetModel();
	if (model.IsNull() == false && m_frustum.Contains(model->aabb.ApplyTransform(c.GetObject()->GetTransform().GetMatrix())) == true) {
		m_renderer->RenderModel(c.GetModel().GetShared(), c.GetObject()->GetTransform().GetMatrix());
	}*/

	if (c.GetModel().IsNull() == false) {
		GetEngine()->GetRenderer()->RenderModel(*c.GetModel().GetShared(), c.GetTransformMatrix(), c.GetLightMode());
	}
}

void retro3d::RenderSystem::OnUpdate( void )
{
	retro3d::RenderDevice *r = GetEngine()->GetRenderer();
	r->SetViewTransform(GetEngine()->GetCamera()->GetViewTransform());
	r->FinishRender();
}

retro3d::RenderSystem::RenderSystem( void ) : mtlInherit(this)
{}
