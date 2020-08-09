#ifndef RETRO_TIMER_COMPONENT_H
#define RETRO_TIMER_COMPONENT_H

#include "../retro_entity.h"
#include "../../common/retro_defs.h"

namespace retro3d
{

retro_component(TimerComponent)
{
private:
	retro3d::RealTimeTimer         m_timer;
	int64_t                        m_tick_count;
	mtlShared<retro3d::IProcedure> m_on_tick;
	mtlShared<retro3d::IProcedure> m_on_destroy;
	bool                           m_multitick;

protected:
	void OnSpawn( void );
	void OnUpdate( void );
	void OnDestroy( void );

public:
	TimerComponent( void );

	void SetTickRate(retro3d::TimerType timer_type, uint32_t num_ticks, retro3d::Time over_time = 1_s);

	void Start( void );
	void Pause( void );
	void Reset( void );

	void SetOnTick(const mtlShared<retro3d::IProcedure> &procedure);
	void DisableOnTick( void );

	void SetOnDestroy(const mtlShared<retro3d::IProcedure> &procedure);
	void DisableOnDestroy( void );
};

}

#endif // RETRO_TIMER_COMPONENT_H
