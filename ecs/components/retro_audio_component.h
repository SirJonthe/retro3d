#ifndef RETRO_AUDIO_COMPONENT_H
#define RETRO_AUDIO_COMPONENT_H

#include "../retro_component.h"
#include "../../audio/retro_voice.h"

namespace retro3d
{

retro_component(AudioComponent)
{
private:
	retro3d::Voice              m_voice;
	double                      m_time_stamp;
	mtlShared< retro3d::Sound > m_synthed; // TODO: the synthesized piece of sound to be played (we can do frequency manipulation)
	int32_t                     m_audio_index;
	int32_t                     m_loops;
	bool                        m_playing;
	bool                        m_moving_emitter;

protected:
	void OnUpdate( void ) override;
	void OnDestroy( void ) override;

private:
	void    SetAudioIndex(int32_t audio_index);
	int32_t GetAudioIndex( void ) const;
	void    Unlock( void );
	bool    IsLocked( void ) const;

public:
	AudioComponent( void );

	void Play(int32_t times = 1);
	void Reset(double to_time = 0.0);
	void Pause( void );
	void Stop( void );

	bool IsPlaying( void ) const;

	retro3d::Voice       &GetVoice( void );
	const retro3d::Voice &GetVoice( void ) const;

	bool IsMovingEmitter( void ) const;
	void ToggleMovingEmitter( void );
};

}

#endif // RETRO_AUDIO_COMPONENT_H
