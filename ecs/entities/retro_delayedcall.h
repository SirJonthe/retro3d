#ifndef RETRO_DELAYEDCALL_H
#define RETRO_DELAYEDCALL_H

#include "../retro_entity.h"
#include "../../common/retro_defs.h"
#include "../../common/retro_time.h"

namespace retro3d
{

class DelayedCall : public mtlInherit< retro3d::Entity, DelayedCall >
{
private:
	retro3d::Time                  m_time;
	retro3d::TimerType             m_timer_type;
	mtlShared<retro3d::IProcedure> m_procedure;

protected:
	virtual void OnUpdate( void );

public:
	DelayedCall( void ) = delete;
	DelayedCall(const mtlShared<retro3d::IProcedure> &procedure, retro3d::Time time, retro3d::TimerType timer_type = retro3d::TIMER_REAL);
};

}

#endif // RETRO_DELAYEDCALL_H
