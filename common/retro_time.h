#ifndef RETRO_TIME_H
#define RETRO_TIME_H

#include <cstdint>
#include <limits>
#include <climits>

namespace retro3d
{

class RealTimeTimer;

// A time class that measures time in fixed-point arithmetic. Mainly used to measure smaller (0 - MAX_TIME_MS) time spans.
class Time
{
	friend class RealTimeTimer;

private:
	static constexpr uint64_t NUM_BITS       = sizeof(uint64_t) * CHAR_BIT;
	static constexpr uint64_t SCALAR         = NUM_BITS >> 1;
	static constexpr uint64_t PRECISION_MASK = (uint64_t(1) << SCALAR) - 1;
	static constexpr uint64_t RANGE_MASK     = ~PRECISION_MASK;

private:
	// Time in milliseconds.
	uint64_t time;

public:
	// Constructing.
	Time(uint32_t ms = 0);
	Time(const Time&) = default;
	Time &operator=(const Time&) = default;

	// Useful constants.
	static constexpr uint64_t MAX_TIME_MS = std::numeric_limits<uint64_t>::max() >> SCALAR;
	static constexpr uint64_t MAX_TIME_S  = MAX_TIME_MS / 1000;
	static constexpr uint64_t MAX_TIME_M  = MAX_TIME_S  / 60;
	static constexpr uint64_t MAX_TIME_H  = MAX_TIME_M  / 60;
	static constexpr uint64_t MAX_TIME_D  = MAX_TIME_H  / 24;
	static constexpr uint64_t MAX_TIME_W  = MAX_TIME_D  / 7;

	// Converts seconds to Time.
	static Time Seconds(uint32_t s);

	// Converts minutes to Time.
	static Time Minutes(uint32_t m);

	// Converts hours to Time.
	static Time Hours(uint32_t h);

	// Converts days to Time.
	static Time Days(uint32_t d);

	// Converts weeks to Time.
	static Time Weeks(uint32_t w);

	// Returns the time of day in 0-23 hours, 0-59 minutes, 0-59 seconds (no milliseconds).
	static Time TimeOfDay( void );

	// Returns the current year as an integer.
	static uint32_t CurrentYear( void );

	// Returns the current week day as an integer 0-6.
	static uint32_t CurrentWeekDay( void );

	// Returns the current month day as an integer 0-30.
	static uint32_t CurrentMonthDay( void );

	// Returns the current year day as an integer 0-365.
	static uint32_t CurrentYearDay( void );

	// Returns the current month as an integer 0-11.
	static uint32_t CurrentMonth( void );

	// Returns time in seconds in real units.
	static double FloatSeconds(uint32_t ms);

	// Returns the total amount of milliseconds.
	uint32_t GetTotalMS( void ) const;

	// Returns the total amount of seconds.
	uint32_t GetTotalSeconds( void ) const;

	// Returns the total amount of minutes.
	uint32_t GetTotalMinutes( void ) const;

	// Returns the total amount of hours.
	uint32_t GetTotalHours( void ) const;

	// Returns the total amount of days.
	uint32_t GetTotalDays( void ) const;

	// Returns the total amount of weeks.
	uint32_t GetTotalWeeks( void ) const;

	// Returns the part amount of milliseconds (0-999).
	uint32_t GetPartMS( void ) const;

	// Returns the part amount of seconds (0-59).
	uint32_t GetPartSeconds( void ) const;

	// Returns the part amount of minutes (0-59).
	uint32_t GetPartMinutes( void ) const;

	// Returns the part amount of hours (0-23).
	uint32_t GetPartHours( void ) const;

	// Returns the part amount of days (0-6).
	uint32_t GetPartDays( void ) const;

	// Returns the part amount of weeks (0-x).
	uint32_t GetPartWeeks( void ) const;

	// Returns the total amount of seconds as real numbers.
	double GetFloatSeconds( void ) const;

	// Same as GetFloatSeconds(), but less confusingly named when used for in RealTimeTimer and scaled times.
	double ToFloat( void ) const;

	// Basic arithmetic.
	Time &operator+=(const Time &r);
	Time &operator-=(const Time &r);
	Time &operator*=(const Time &r);
	Time &operator/=(const Time &r);

	// Time comparisons.
	bool operator==(const Time &r) const;
	bool operator!=(const Time &r) const;
	bool operator<(const Time &r) const;
	bool operator>(const Time &r) const;
	bool operator<=(const Time &r) const;
	bool operator>=(const Time &r) const;
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

namespace retro3d
{


// A scaled timer that can be used to track time in other than seconds (called "ticks"). Updates as you look at the time.
class RealTimeTimer
{
public:
	enum Units
	{
		TicksPerSecond,
		SecondsPerTick
	};

private:
	const RealTimeTimer *m_parent;
	retro3d::Time        m_time_scale;
	retro3d::Time        m_origin_time;
	retro3d::Time        m_accum_ticks;
	bool                 m_ticking;

private:
	// Updates the internal timer.
	void          UpdateTimer( void );

	// Returns the current time scale including parent time scales.
	retro3d::Time GetTimeScale( void ) const;

	// Returns the scaled time since last UpdateTimer.
	retro3d::Time TimeDelta( void ) const;

public:
	// Returns unscaled time since epoch.
	static uint64_t GetProgramTimeMS( void );

public:
	// Constructs a timer with properties. Does not start it.
	RealTimeTimer(uint32_t num_ticks = 1, retro3d::Time over_time = 1_s);

	// Default
	RealTimeTimer(const RealTimeTimer&) = default;
	RealTimeTimer &operator=(const RealTimeTimer&) = default;

	// Changes the tick rate. Can be called while timer is ticking and only affect the time added to the clock *after* the call is made. 1 ms granularity.
	void SetTickRate(uint32_t num_ticks, retro3d::Time over_time = 1_s);

	// Returns the current tick rate.
	retro3d::Time GetSecondsPerTick( void ) const;

	// Starts the timer.
	void Start( void );

	// Stops the timer. Does not reset the time on the clock.
	void Pause( void );

	// Toggles the timer on and off without resetting the time on the clock.
	void Toggle( void );

	// Resets the time on the clock.
	void Reset( void );

	// Resets the number of ticks elapsed.
	void ResetTicks( void );

	// The status of the timer.
	bool IsTicking( void ) const;

	// The status of the timer.
	bool IsStopped( void ) const;

	// Returns true if a whole tick has elapsed since last Start/Reset*/Decrement
	bool IsDue( void ) const;

	// Returns time (as floating-point seconds) scaled in relation to ticks.
	retro3d::Time GetScaledTime( void ) const;

	// Returns the number of ticks that have elapsed. Corresponds to the whole part of GetTime().
	uint32_t GetTicks( void ) const;

	// Returns the progress 0-1 between last tick and next beat. Corresponds to the fractional part of GetTime().
	double GetTickProgress( void ) const;

	// Add a timer parent to be used as a parent time scale. Can be called while timer is ticking and only affect the time added to the clock *after* the call is made.
	void SetParent(const RealTimeTimer *parent);

	// Creates a new timer with the current timer as parent. The user must decouple parent from child if parent is destroyed.
	RealTimeTimer SpawnChild(uint32_t num_ticks = 1, retro3d::Time over_time = 1_s) const;
};

}

#endif // RETRO_TIME_H
