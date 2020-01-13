#include "retro_light_system.h"
#include "../../retro3d.h"

void retro3d::LightSystem::OnSpawn(retro3d::LightComponent &c)
{
	m_light_tree.Insert(c.GetObject(), &c.GetActivationBounds(), c.IsStatic(), c.GetFilterFlags());
	if (c.ShouldReinsert() == true) {
		c.ToggleReinsert();
	}
}

void retro3d::LightSystem::OnUpdate(retro3d::LightComponent &c)
{
	if (c.ShouldReinsert() == true) {
		m_light_tree.Remove(c.GetObject());
		m_light_tree.Insert(c.GetObject(), &c.GetActivationBounds(), c.IsStatic(), c.GetFilterFlags());
		c.ToggleReinsert();
	}
}

void retro3d::LightSystem::OnDestroy(retro3d::LightComponent &c)
{
	m_light_tree.Remove(c.GetObject());
}

retro3d::LightSystem::LightSystem( void ) : mtlInherit(this)
{
	if (m_light_tree.IsCheckingContacts() == true) {
		m_light_tree.ToggleContactCheck();
	}
}

void retro3d::LightSystem::Debug_PrintTree( void ) const
{
	m_light_tree.Debug_PrintTree();
}

void retro3d::LightSystem::Debug_RenderTree( void )
{
	std::queue<retro3d::ColliderTree<retro3d::Entity>::Debug_AABB> q = m_light_tree.Debug_GetTree();
	while (q.empty() == false) {
		mmlVector<3> c = mmlVector<3>::Fill(0.0f);
		switch (q.front().type) {
		case retro3d::ColliderTree<retro3d::Entity>::Debug_AABB::Node_Dynamic:
			c[2] = 1.0f;
			break;
		case retro3d::ColliderTree<retro3d::Entity>::Debug_AABB::Node_Static:
			c[0] = 1.0f;
			c[1] = 1.0f;
			break;
		case retro3d::ColliderTree<retro3d::Entity>::Debug_AABB::Leaf_Dynamic:
			c[1] = 1.0f;
			break;
		case retro3d::ColliderTree<retro3d::Entity>::Debug_AABB::Leaf_Static:
			c[0] = 1.0f;
			break;
		}
		GetEngine()->GetRenderer()->RenderAABB(q.front(), mmlMatrix<4,4>::Identity(), c);
		q.pop();
	}
}

void retro3d::LightSystem::UpdateLightmap( void )
{
	// render static lights to the light map
	// need input vertex, normals, light uvs
}
