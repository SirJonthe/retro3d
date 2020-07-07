#ifndef RETRO_TIMER_COMPONENT_H
#define RETRO_TIMER_COMPONENT_H

#include "../retro_entity.h"
#include "../../common/retro_defs.h"

namespace retro3d
{

retro_component(TimerComponent)
{
private:
	double             m_bps;
	double             m_ticks;
	double             m_p_ticks;
	uint64_t           m_i_ticks;
	retro3d::Procedure m_on_tick;
	bool               m_is_ticking;

protected:
	void OnUpdate( void );

public:
	TimerComponent( void );

	void SetTickRate(double rate, retro3d::RealTimeTimer::Units units);
	double GetTickRate(retro3d::RealTimeTimer::Units units) const;

	void Start( void );
	void Stop( void );
	void Toggle( void );
	void Reset( void );

	bool IsTicking( void ) const;
	bool IsStopped( void ) const;
	bool IsDue( void ) const;

	double GetTime( void ) const;
	uint64_t GetTicks( void ) const;

	void SetProcedure(retro3d::Procedure procedure);
	void RemoveProcedure( void );
};

}

#endif // RETRO_TIMER_COMPONENT_H
