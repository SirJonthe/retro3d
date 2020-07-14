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
	retro3d::RealTimeTimer m_timer;
	retro3d::Procedure     m_procedure;

protected:
	virtual void OnSpawn( void );
	virtual void OnUpdate( void );
	virtual void OnDestroy( void );

public:
	DelayedCall( void );

	void SetProcedure(retro3d::Procedure procedure, retro3d::Time time, bool scale_time);
};

}

#endif // RETRO_DELAYEDCALL_H
