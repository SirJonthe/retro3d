#ifndef RETRO_RENDER_COMPONENT_H
#define RETRO_RENDER_COMPONENT_H

#include "../retro_component.h"
#include "../../graphics/retro_model.h"
#include "retro_transform_component.h"
#include "../../frontend/retro_render_device.h"

namespace retro3d
{

// TODO: Take into account level of detail

retro_component(RenderComponent)
{
public:
	struct Frame
	{
		mtlShared<retro3d::Model>         model;
		retro3d::Array<retro3d::Material> override_materials; // TODO: use these materials instead of the ones supplied with the model
		float                             display_time = 0.0f;
	};

	struct Animation
	{
		mtlArray<Frame> frames;
		std::string     name;
		retro3d::AABB   biggest_aabb;
	};

	typedef mtlStringMap< Animation > Animations;

private:
	retro3d::SharedTransform          m_transform;
	Animations                        m_animations;
	Animation                        *m_first_animation;
	Animation                        *m_current_animation;
	uint32_t                          m_frame_index;
	float                             m_current_frame_countdown;
	retro3d::AABB                     m_biggest_aabb;
	retro3d::RenderDevice::LightMode  m_light_mode;
	float                             m_animation_speed_scale;

protected:
	void OnSpawn( void ) override;
	void OnUpdate( void ) override;

public:
	RenderComponent( void );

	mtlShared<retro3d::Model>       GetModel( void );
	const mtlShared<retro3d::Model> GetModel( void ) const;
	void SetModel(mtlShared<retro3d::Model> model);

	retro3d::AABB GetBiggestAnimationAABB( void ) const;
	retro3d::AABB GetBiggestAABB( void ) const;

	void             CreateAnimation(const mtlChars &animation_name);
	void             SetAnimation(const mtlChars &animation_name);
	const Animation *GetAnimation( void ) const;
	Animation       *GetAnimation( void );

	uint32_t GetFrameIndex( void ) const;
	void     SetFrameIndex(uint32_t i);

	mmlMatrix<4,4> GetTransformMatrix( void ) const;

	retro3d::RenderDevice::LightMode GetLightMode( void ) const;
	void                             SetLightMode(retro3d::RenderDevice::LightMode light_mode);

	float GetAnimationSpeedScale( void ) const;
	void  SetAnimationSpeedScale(float scale);
};

}

#endif // RETRO_RENDER_COMPONENT_H
