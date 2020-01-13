#ifndef RETRO_AUDIO_SYSTEM_H
#define RETRO_AUDIO_SYSTEM_H

#include "../retro_system.h"
#include "../components/retro_audio_component.h"
#include "../../physics/retro_collider_tree.h"
#include "../../common/retro_transform.h"

namespace retro3d
{

retro_system(AudioSystem, retro3d::AudioComponent)
{
private:
	retro3d::ColliderTree<retro3d::AudioComponent> m_sound_stage;
	mtlList< retro3d::AudioComponent* >            m_components; // TODO: use a faster data structure for insert, remove

protected:
	void OnSpawn(retro3d::AudioComponent &c);
	void OnDestroy(retro3d::AudioComponent &c);
	void OnUpdate( void );

public:
	AudioSystem( void );
};

}

#endif // RETRO_AUDIO_SYSTEM_H
