#ifndef RETRO_TIMER_H
#define RETRO_TIMER_H

#include <cstdint>

namespace retro3d
{

/*struct Time
{
	uint64_t sec;
	double   part;
};*/

class Timer
{
public:
	enum Units
	{
		BeatsPerSecond,
		SecondsPerBeat
	};

private:
	Timer            *m_parent;
	double            m_bps;
	uint64_t          m_beat_interval;
	mutable uint64_t  m_acc_time;
	mutable uint64_t  m_time_last;
	bool              m_ticking;

private:
	void UpdateTimer( void ) const;
	static uint64_t GetProgramTimeMS( void );

public:
	Timer( void );
	Timer(double tempo, Units units);

	void   SetTempo(double tempo, Units units);
	double GetTempo(Units units) const;

	void Start( void );
	void Stop( void );
	void Toggle( void );
	void Reset( void );
	void ResetPartBeat( void );
	void ResetBeats( void );
	void DecrementBeat( void );

	bool IsTicking( void ) const;
	bool IsStopped( void ) const;
	bool IsDue( void ) const;

	uint64_t GetFixedTime( void ) const;
	double   GetTime( void ) const;
	int32_t  GetBeats( void ) const;
	double   GetPartBeat( void ) const;
};

}

#endif // RETRO_TIMER_H
