#include "retro_render_system.h"
#include "../retro_entity.h"
#include "../../backend/null_render_device.h"
#include "../../backend/t3d_render_device.h"

void retro3d::RenderSystem::OnSpawn(retro3d::RenderComponent &c)
{
	m_view_hierarchy.Insert(&c, const_cast<retro3d::AABBCollider*>(&c.GetOcclusionCollider()));
}

void retro3d::RenderSystem::OnUpdate(retro3d::RenderComponent &c)
{
	// NOTE: Immediate rendering of all RenderComponents regardless if they are visible.

	if (c.GetModel().IsNull() == false) {
		GetEngine()->GetRenderer()->RenderModel(*c.GetModel().GetShared(), c.GetTransformMatrix(), c.GetLightMode());
	}
}

void retro3d::RenderSystem::OnDestroy(retro3d::RenderComponent &c)
{
	m_view_hierarchy.Remove(&c);
}

void retro3d::RenderSystem::OnUpdate( void )
{
	// NOTE: Deferred rendering of RenderComponents, omitting objects outside of view frustum.

//	m_view_hierarchy.Update();

	retro3d::RenderDevice *r = GetEngine()->GetRenderer();
	r->SetViewTransform(GetEngine()->GetCamera()->GetViewTransform());

	m_view_frustum = r->GetViewFrustum();

//	mtlList<retro3d::RenderComponent*> pvs;

//	m_view_hierarchy.Contains(m_view_frustum, ~uint64_t(0), &pvs);

//	mtlItem<retro3d::RenderComponent*> *i = pvs.GetFirst();
//	while (i != nullptr) {
//		retro3d::RenderComponent *rc = i->GetItem();
//		if (rc->GetModel().IsNull() == false) {
//			GetEngine()->GetRenderer()->RenderModel(*rc->GetModel().GetShared(), rc->GetTransformMatrix(), rc->GetLightMode());
//		}
//		i = i->GetNext();
//	}

	r->FinishRender();
}

retro3d::RenderSystem::RenderSystem( void ) : mtlInherit(this)
{}
