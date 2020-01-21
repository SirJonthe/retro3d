#include "retro_render_component.h"
#include "retro_transform_component.h"
#include "../retro_entity.h"
#include "../../common/MiniLib/MML/mmlMath.h"

void retro3d::RenderComponent::OnSpawn( void )
{
	m_transform = GetObject()->AddComponent<retro3d::TransformComponent>()->GetTransform();
	m_occlusion_collider.AttachTransform(m_transform);
	if (m_current_animation == nullptr) {
		SetAnimation("idle");
	}
}

void retro3d::RenderComponent::OnUpdate( void )
{
	if (m_current_animation != nullptr) {
		float delta_time = float(GetObject()->DeltaTime()) * m_animation_speed_scale;
		m_current_frame_countdown -= delta_time;
		while (m_current_frame_countdown <= 0.0f) {
			m_frame_index = (m_frame_index + 1) % uint32_t(m_current_animation->frames.GetSize());
			float new_duration = m_current_animation->frames[m_frame_index].display_time;
			m_current_frame_countdown += new_duration > 0.0f ? new_duration : delta_time;
		}
	}
}

retro3d::RenderComponent::RenderComponent( void ) :
	mtlInherit(this),
	m_transform(),
	m_animations(), m_first_animation(nullptr), m_current_animation(nullptr),
	m_frame_index(0), m_current_frame_countdown(0.0f),
	m_biggest_aabb(),
	m_occlusion_collider(),
	m_light_mode(retro3d::RenderDevice::LightMode_Dynamic),
	m_animation_speed_scale(1.0f)
{
	CreateAnimation("idle", 1);
}

mtlShared<retro3d::Model> retro3d::RenderComponent::GetModel( void )
{
	return m_current_animation != nullptr ? m_current_animation->frames[m_frame_index].model : mtlShared<retro3d::Model>();
}

const mtlShared<retro3d::Model> retro3d::RenderComponent::GetModel( void ) const
{
	return m_current_animation != nullptr ? m_current_animation->frames[m_frame_index].model : mtlShared<retro3d::Model>();
}

void retro3d::RenderComponent::SetFrame(const mtlShared< retro3d::Model > &model, float display_time)
{
	if (m_current_animation != nullptr) {
		m_current_animation->frames[m_frame_index].model = model;
		m_current_animation->frames[m_frame_index].display_time = display_time;
		m_current_animation->biggest_aabb = m_current_animation->biggest_aabb | model->aabb;
		m_biggest_aabb = m_biggest_aabb | m_current_animation->biggest_aabb;
		m_occlusion_collider.SetLocalAABB(m_biggest_aabb);
	}
}

retro3d::AABB retro3d::RenderComponent::GetBiggestAnimationAABB( void ) const
{
	return m_current_animation != nullptr ? m_current_animation->biggest_aabb : retro3d::AABB();
}

retro3d::AABB retro3d::RenderComponent::GetBiggestAABB( void ) const
{
	return m_biggest_aabb;
}

const retro3d::AABBCollider &retro3d::RenderComponent::GetOcclusionCollider( void ) const
{
	return m_occlusion_collider;
}

void retro3d::RenderComponent::CreateAnimation(const mtlChars &animation_name, uint32_t frame_count)
{
	Animation *a = m_animations.CreateEntry(animation_name);

	mtlShared<retro3d::Model> default_copy;
	default_copy.New();
	*default_copy.GetShared() = *retro3d::Engine::DefaultModel().GetShared();

	a->frames.Create(mmlMax(1, int(frame_count)));
	for (int i = 0; i < a->frames.GetSize(); ++i) {
		a->frames[i].model = default_copy;
	}

	if (m_current_animation == nullptr) {
		m_current_animation = a;
	}
	if (m_first_animation == nullptr) {
		m_first_animation = a;
	}
}

void retro3d::RenderComponent::CreateAnimation(uint32_t frame_count)
{
	if (m_current_animation != nullptr) {
		mtlShared<retro3d::Model> default_copy;
		default_copy.New();
		*default_copy.GetShared() = *retro3d::Engine::DefaultModel().GetShared();

		m_current_animation->frames.Create(mmlMax(1, int(frame_count)));
		for (int i = 0; i < m_current_animation->frames.GetSize(); ++i) {
			m_current_animation->frames[i].model = default_copy;
		}
	}
}

void retro3d::RenderComponent::SetAnimation(const mtlChars &animation_name)
{
	if (m_first_animation == nullptr) {
		std::cout << "[Warning] No animations available." << std::endl;
		return;
	}

	Animation *a = m_animations.GetEntry(animation_name);
	if (m_current_animation == a) { return; }

	m_current_animation = a;

	if (m_current_animation == nullptr) {
		std::cout << "[Warning] Missing animation \"" << std::string(animation_name.GetChars(), size_t(animation_name.GetSize())) << "\". Defaulting to \"idle\"." << std::endl;
		m_current_animation = m_animations.GetEntry("idle");
		if (m_current_animation == nullptr) {
			std::cout << "[Warning] Missing animation \"idle\". Defaulting to first animation." << std::endl;
			m_current_animation = m_first_animation;
		}
	}
	m_frame_index = 0;
	m_current_frame_countdown = m_current_animation->frames[m_frame_index].display_time;
}

uint32_t retro3d::RenderComponent::GetFrameIndex( void ) const
{
	return m_frame_index;
}

void retro3d::RenderComponent::SetFrameIndex(uint32_t i)
{
	if (m_current_animation != nullptr) {
		m_frame_index = (i % uint32_t(m_current_animation->frames.GetSize()));
	}
}

uint32_t retro3d::RenderComponent::GetAnimationFrameCount( void ) const
{
	return m_current_animation != nullptr ? uint32_t(m_current_animation->frames.GetSize()) : 0;
}

mmlMatrix<4,4> retro3d::RenderComponent::GetTransformMatrix( void ) const
{
	const retro3d::TransformComponent *t = GetObject()->GetComponent<retro3d::TransformComponent>();
	return t != nullptr ? t->GetTransform()->GetFinalMatrix() : mmlMatrix<4,4>::Identity();
}

retro3d::RenderDevice::LightMode retro3d::RenderComponent::GetLightMode( void ) const
{
	return m_light_mode;
}

void retro3d::RenderComponent::SetLightMode(retro3d::RenderDevice::LightMode light_mode)
{
	m_light_mode = light_mode;
}

float retro3d::RenderComponent::GetAnimationSpeedScale( void ) const
{
	return m_animation_speed_scale;
}

void retro3d::RenderComponent::SetAnimationSpeedScale(float scale)
{
	m_animation_speed_scale = scale;
}
