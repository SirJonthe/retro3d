#include "retro_render_system.h"
#include "../retro_entity.h"
#include "../../backend/null_render_device.h"
#include "../../backend/t3d_render_device.h"

void retro3d::RenderSystem::OnSpawn(retro3d::RenderComponent &c)
{
	m_view_hierarchy.Insert(&c, const_cast<retro3d::AABBCollider*>(&c.GetOcclusionCollider()));
	++m_render_items;
}

void retro3d::RenderSystem::OnUpdate(retro3d::RenderComponent &c)
{
	// NOTE: Immediate rendering of potentially visible RenderComponents.

	if (c.GetModel().IsNull() == false) {
		const mmlMatrix<4,4>   world_transform = c.GetTransformMatrix();
		const retro3d::AABB    world_aabb      = c.GetModel()->aabb.ApplyTransform(world_transform);
		const retro3d::Contain world_occlusion = m_view_frustum.Contains(world_aabb, false, true);
		if (world_occlusion >= retro3d::Contain_Partial) {
			GetEngine()->GetRenderer()->RenderModel(*c.GetModel().GetShared(), c.GetTransformMatrix(), c.GetLightMode());
			++m_potentially_visible_items_counter;
		}
	}
}

void retro3d::RenderSystem::OnDestroy(retro3d::RenderComponent &c)
{
	m_view_hierarchy.Remove(&c);
	--m_render_items;
}

void retro3d::RenderSystem::OnUpdate( void )
{
	// NOTE: Deferred rendering of RenderComponents, omitting objects outside of view frustum via BVH.

	m_potentially_visible_items = m_potentially_visible_items_counter;
	m_potentially_visible_items_counter = 0;

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

retro3d::RenderSystem::RenderSystem( void ) : mtlInherit(this), m_view_hierarchy(), m_light_hierarchy(), m_view_frustum(), m_render_items(0), m_potentially_visible_items(0), m_potentially_visible_items_counter(0)
{}

uint32_t retro3d::RenderSystem::GetRenderItemCount( void ) const
{
	return m_render_items;
}

uint32_t retro3d::RenderSystem::GetPotentiallyVisibleCount( void ) const
{
	return m_potentially_visible_items;
}
