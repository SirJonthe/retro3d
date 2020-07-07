#ifndef RETRO_TIME_H
#define RETRO_TIME_H

#include <cstdint>
#include <limits>

namespace retro3d
{

// A time class that measures time in fixed-point arithmetic. Mainly used to measure smaller (0 - MAX_TIME_MS) time spans.
class Time
{
private:
	static constexpr uint64_t SCALAR = 32;

private:
	uint64_t time; // 32.32 fixed-point (ms).

public:
	Time(uint64_t ms = 0);
	Time(const Time&) = default;
	Time &operator=(const Time&) = default;

	static constexpr uint64_t MAX_TIME_MS = std::numeric_limits<uint64_t>::max() >> SCALAR;

	// Converts seconds to Time.
	static Time Seconds(uint64_t s);

	// Converts minutes to Time.
	static Time Minutes(uint64_t m);

	// Converts hours to Time.
	static Time Hours(uint64_t h);

	// Converts days to Time.
	static Time Days(uint64_t d);

	// Converts weeks to Time.
	static Time Weeks(uint64_t w);

	// Returns time in seconds in real units.
	static double FloatTime(uint64_t time);

	Time &operator+=(const Time &r);
	Time &operator-=(const Time &r);
	Time &operator*=(const Time &r);
	Time &operator/=(const Time &r);
};

// A scaled timer that can be used to track time in other than seconds (called "ticks"). Updates as you look at the time. NOT THREAD SAFE!

// FIXME: GetTime *does not* support altering tick rate while running (*all* elapsed time will be compressed/expanded). Fix this!

class RealTimeTimer
{
public:
	enum Units
	{
		TicksPerSecond,
		SecondsPerTick
	};

private:
	double            m_bps;
	uint64_t          m_beat_interval;
	mutable uint64_t  m_acc_time;
	mutable uint64_t  m_time_last;
	bool              m_ticking;

private:
	void UpdateTimer( void ) const;

public:
	static uint64_t GetProgramTimeMS( void );

public:
	// Constructs a timer. Does not start it.
	RealTimeTimer( void );

	// Constructs a timer with properties. Does not start it.
	RealTimeTimer(double rate, Units units);

	// Changes the tick rate. Can be called while timer is ticking, but rate must be > 0.0.
	void   SetTickRate(double rate, Units units);

	// Returns the current tick rate.
	double GetTickRate(Units units) const;

	// Starts the timer.
	void Start( void );

	// Stops the timer. Does not reset the time on the clock.
	void Stop( void );

	// Toggles the timer on and off without resetting the time on the clock.
	void Toggle( void );

	// Resets the time on the clock.
	void Reset( void );

	// Resets the fractional part of the clock.
	void ResetPartTick( void );

	// Resets the whole part of the clock.
	void ResetTicks( void );

	// Decrements the number of ticks elapsed by one if possible.
	void DecrementTicks( void );

	// The status of the timer.
	bool IsTicking( void ) const;

	// The status of the timer.
	bool IsStopped( void ) const;

	// Returns true if a whole tick has elapsed since last Start/Reset*/Decrement
	bool IsDue( void ) const;

	// Returns scaled time in relation to ticks.
	uint64_t GetFixedTime( void ) const;

	// Returns time (as floating-point seconds) scaled in relation to ticks.
	double   GetTime( void ) const;

	// Returns the number of ticks that have elapsed. Corresponds to the whole part of GetTime().
	uint64_t GetTicks( void ) const;

	// Returns the progress 0-1 between last tick and next beat. Corresponds to the fractional part of GetTime().
	double   GetProgress( void ) const;
};

}

retro3d::Time operator+(retro3d::Time l, const retro3d::Time &r);
retro3d::Time operator-(retro3d::Time l, const retro3d::Time &r);
retro3d::Time operator*(retro3d::Time l, const retro3d::Time &r);
retro3d::Time operator/(retro3d::Time l, const retro3d::Time &r);

retro3d::Time operator "" _ms(unsigned long long int ms);
retro3d::Time operator ""  _s(unsigned long long int s);
retro3d::Time operator ""  _m(unsigned long long int m);
retro3d::Time operator ""  _h(unsigned long long int h);
retro3d::Time operator ""  _d(unsigned long long int d);
retro3d::Time operator ""  _w(unsigned long long int w);

#endif // RETRO_TIME_H
