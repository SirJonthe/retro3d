#include "../../retro3d.h"
#include "retro_audio_system.h"
#include "../components/retro_transform_component.h"


void retro3d::AudioSystem::OnSpawn(retro3d::AudioComponent &c)
{
	// Add to sound_stage
	m_components.AddLast(&c);
}

void retro3d::AudioSystem::OnDestroy(retro3d::AudioComponent &c)
{
	// Remove from sound_stage
	mtlItem<retro3d::AudioComponent*> *i = m_components.GetFirst();
	while (i != nullptr) { // TODO: Find a better solution to O(n) for this
		if (i->GetItem() == &c) {
			i = i->Remove();
			break;
		} else {
			i = i->GetNext();
		}
	}
}

void retro3d::AudioSystem::OnUpdate( void )
{
	// TODO: Support arbitrary sound receiver transform (not just camera transform)
	GetEngine()->GetSoundDevice()->SetReceiverTransform(GetEngine()->GetCamera()->GetTransform());
	GetEngine()->GetSoundDevice()->Update();
}

retro3d::AudioSystem::AudioSystem( void ) : mtlInherit(this), m_sound_stage()
{}
