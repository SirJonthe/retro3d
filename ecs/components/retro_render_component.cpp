#include "retro_render_component.h"
#include "retro_transform_component.h"
#include "../retro_entity.h"

void retro3d::RenderComponent::OnSpawn( void )
{
	m_transform = GetObject()->AddComponent<retro3d::TransformComponent>()->GetTransform();
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
	m_animations(), m_first_animation(nullptr), m_current_animation(nullptr),
	m_frame_index(0), m_current_frame_countdown(0.0f),
	m_light_mode(retro3d::RenderDevice::LightMode_Dynamic),
	m_animation_speed_scale(1.0f)
{
	CreateAnimation("idle");
}

mtlShared<retro3d::Model> retro3d::RenderComponent::GetModel( void )
{
	return m_current_animation != nullptr ? m_current_animation->frames[m_frame_index].model : mtlShared<retro3d::Model>();
}

const mtlShared<retro3d::Model> retro3d::RenderComponent::GetModel( void ) const
{
	return m_current_animation != nullptr ? m_current_animation->frames[m_frame_index].model : mtlShared<retro3d::Model>();
}

void retro3d::RenderComponent::SetModel(mtlShared< retro3d::Model > model)
{
	if (m_current_animation != nullptr) {
		m_current_animation->frames[m_frame_index].model = model;
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

void retro3d::RenderComponent::CreateAnimation(const mtlChars &animation_name)
{
	Animation *a = m_animations.CreateEntry(animation_name);
	a->name = std::string(animation_name.GetChars(), size_t(animation_name.GetSize()));
	if (a->frames.GetSize() == 0) {
		a->frames.Create(1);
		a->frames[0].model.New();
		*a->frames[0].model.GetShared() = *retro3d::Engine::DefaultModel().GetShared();
		a->frames[0].display_time = 0.0f;
	}
	if (m_current_animation == nullptr) {
		m_current_animation = a;
	}
	if (m_first_animation == nullptr) {
		m_first_animation = a;
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

const retro3d::RenderComponent::Animation *retro3d::RenderComponent::GetAnimation( void ) const
{
	return m_current_animation;
}

retro3d::RenderComponent::Animation *retro3d::RenderComponent::GetAnimation( void )
{
	return m_current_animation;
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
