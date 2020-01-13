#include "retro_audio_component.h"
#include "../../retro3d.h"
#include "../components/retro_transform_component.h"

void retro3d::AudioComponent::OnUpdate( void )
{
	if (IsPlaying() == true) {
		m_time_stamp += GetObject()->DeltaTime(); // NOTE BUG: This is completely inaccurate for time scaled objects, i.e. sound needs to play slower or faster depending on scale
		if (m_time_stamp > double(m_voice.GetSound()->GetDuration())) {
			const int32_t remaining_loops = m_loops - 1;
			Stop();
			if (remaining_loops < 0) {
				Play(-1);
			} else {
				Play(remaining_loops);
			}
		}
	}
}

void retro3d::AudioComponent::OnDestroy( void )
{
	Pause();
}

void retro3d::AudioComponent::SetAudioIndex(int32_t audio_index)
{
	m_audio_index = audio_index;
}

int32_t retro3d::AudioComponent::GetAudioIndex( void ) const
{
	return m_audio_index;
}

void retro3d::AudioComponent::Unlock( void )
{
	m_audio_index = -1;
}

bool retro3d::AudioComponent::IsLocked( void ) const
{
	return m_audio_index > -1;
}

retro3d::AudioComponent::AudioComponent( void ) : mtlInherit(this), m_time_stamp(0.0), m_synthed(), m_audio_index(-1), m_loops(0), m_playing(false), m_moving_emitter(false)
{}

void retro3d::AudioComponent::Play(int32_t times)
{
	if (times == 0) {
		if (IsPlaying() == true) {
			Stop();
		}
		return;
	}

	retro3d::TransformComponent *t = GetObject()->GetComponent<retro3d::TransformComponent>();
	if (t == nullptr) {
		SetAudioIndex(GetEngine()->GetSoundDevice()->Play(m_voice, mmlMatrix<4,4>::Identity(), m_time_stamp));
	} else if (m_moving_emitter == true) {
		SetAudioIndex(GetEngine()->GetSoundDevice()->Play(m_voice, t->GetTransform(), m_time_stamp));
	} else {
		SetAudioIndex(GetEngine()->GetSoundDevice()->Play(m_voice, t->GetTransform()->GetFinalMatrix(), m_time_stamp));
	}
	m_playing = IsLocked();
	m_loops = (IsPlaying() == true) ? times : 0;
}

void retro3d::AudioComponent::Reset(double to_time)
{
	m_time_stamp = to_time;
}

void retro3d::AudioComponent::Pause( void )
{
	m_playing = false;
	GetEngine()->GetSoundDevice()->Kill(m_audio_index);
	Unlock();
}

void retro3d::AudioComponent::Stop( void )
{
	Pause();
	m_time_stamp = 0.0;
	m_loops = 0;
}

bool retro3d::AudioComponent::IsPlaying( void ) const
{
	return m_playing;
}

retro3d::Voice &retro3d::AudioComponent::GetVoice( void )
{
	return m_voice;
}

const retro3d::Voice &retro3d::AudioComponent::GetVoice( void ) const
{
	return m_voice;
}

bool retro3d::AudioComponent::IsMovingEmitter( void ) const
{
	return m_moving_emitter;
}

void retro3d::AudioComponent::ToggleMovingEmitter( void )
{
	m_moving_emitter = !m_moving_emitter;
}
