#include <chrono>
#include "retro_time.h"
#include "retro_assert.h"
#include "MiniLib/MML/mmlMath.h"

#define MS_PER_SEC 1000


retro3d::Time::Time(uint64_t ms) : time(ms << SCALAR)
{
	RETRO3D_ASSERT(ms >= MAX_TIME_MS);
}

retro3d::Time retro3d::Time::Seconds(uint64_t s)       { return s * 1000; }
retro3d::Time retro3d::Time::Minutes(uint64_t m)       { return Seconds(m * 60); }
retro3d::Time retro3d::Time::Hours(uint64_t h)         { return Minutes(h * 60); }
retro3d::Time retro3d::Time::Days(uint64_t d)          { return Hours(d * 24); }
retro3d::Time retro3d::Time::Weeks(uint64_t w)         { return Days(w * 7); }

double retro3d::Time::FloatTime(uint64_t time) { return time / 1000.0; }

retro3d::Time &retro3d::Time::operator+=(const retro3d::Time &r)
{
	time += r.time;
	return *this;
}

retro3d::Time &retro3d::Time::operator-=(const retro3d::Time &r)
{
	time -= r.time;
	return *this;
}

retro3d::Time &retro3d::Time::operator*=(const retro3d::Time &r)
{
	time *= r.time;
	time >>= SCALAR;
	return *this;
}

retro3d::Time &retro3d::Time::operator/=(const retro3d::Time &r)
{
	time /= r.time;
	time <<= SCALAR;
	return *this;
}

void retro3d::RealTimeTimer::UpdateTimer( void ) const
{
	if (m_ticking) {
		uint64_t time = GetProgramTimeMS();
		m_acc_time += time - m_time_last;
		m_time_last = time;
	}
}

uint64_t retro3d::RealTimeTimer::GetProgramTimeMS( void )
{
	return uint64_t(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
}

retro3d::RealTimeTimer::RealTimeTimer( void ) : m_bps(0.0), m_beat_interval(0), m_acc_time(0), m_time_last(0), m_ticking(false)
{
	SetTickRate(1.0, TicksPerSecond);
}

retro3d::RealTimeTimer::RealTimeTimer(double rate, retro3d::RealTimeTimer::Units units): m_bps(0.0), m_beat_interval(0), m_acc_time(0), m_time_last(GetProgramTimeMS()), m_ticking(false)
{
	SetTickRate(rate, units);
}

void retro3d::RealTimeTimer::SetTickRate(double rate, retro3d::RealTimeTimer::Units units)
{
	rate = mmlMax(rate, 0.0);
	RETRO3D_ASSERT(rate > 0.0);
	m_bps = (units == TicksPerSecond) ? rate : 1.0 / rate;
	m_beat_interval = uint64_t(MS_PER_SEC / m_bps);
}

double retro3d::RealTimeTimer::GetTickRate(retro3d::RealTimeTimer::Units units) const
{
	return (units == TicksPerSecond) ? m_bps : 1.0 / m_bps;
}

void retro3d::RealTimeTimer::Start( void )
{
	if (!IsTicking()) {
		m_time_last = GetProgramTimeMS();
		m_ticking = true;
	}
}

void retro3d::RealTimeTimer::Stop( void )
{
	if (IsTicking()) {
		UpdateTimer();
		m_ticking = false;
	}
}

void retro3d::RealTimeTimer::Toggle( void )
{
	if (IsTicking()) {
		Stop();
	} else {
		Start();
	}
}

void retro3d::RealTimeTimer::Reset( void )
{
	m_acc_time = 0;
}

void retro3d::RealTimeTimer::ResetPartTick( void )
{
	m_acc_time = m_acc_time - m_acc_time % m_beat_interval;
}

void retro3d::RealTimeTimer::ResetTicks( void )
{
	m_acc_time = m_acc_time % m_beat_interval;
}

void retro3d::RealTimeTimer::DecrementTicks( void )
{
	if (m_acc_time >= m_beat_interval) {
		m_acc_time -= m_beat_interval;
	}
}

bool retro3d::RealTimeTimer::IsTicking( void ) const
{
	return m_ticking;
}

bool retro3d::RealTimeTimer::IsStopped( void ) const
{
	return !m_ticking;
}

bool retro3d::RealTimeTimer::IsDue( void ) const
{
	return GetTime() >= 1.0;
}

uint64_t retro3d::RealTimeTimer::GetFixedTime( void ) const
{
	UpdateTimer();
	return (m_acc_time * MS_PER_SEC) / m_beat_interval;
}

double retro3d::RealTimeTimer::GetTime( void ) const
{
	UpdateTimer();
	return double(m_acc_time) / double(m_beat_interval);
}

uint64_t retro3d::RealTimeTimer::GetTicks( void ) const
{
	UpdateTimer();
	return uint64_t(m_acc_time / m_beat_interval);
}

double retro3d::RealTimeTimer::GetProgress( void ) const
{
	UpdateTimer();
	return double(m_acc_time % m_beat_interval) / double(m_beat_interval);
}

retro3d::Time operator+(retro3d::Time l, const retro3d::Time &r) { return l += r; }
retro3d::Time operator-(retro3d::Time l, const retro3d::Time &r) { return l -= r; }
retro3d::Time operator*(retro3d::Time l, const retro3d::Time &r) { return l *= r; }
retro3d::Time operator/(retro3d::Time l, const retro3d::Time &r) { return l /= r; }

retro3d::Time operator "" _ms(unsigned long long int ms) { return retro3d::Time(ms); }
retro3d::Time operator ""  _s(unsigned long long int s)  { return retro3d::Time::Seconds(s); }
retro3d::Time operator ""  _m(unsigned long long int m)  { return retro3d::Time::Minutes(m); }
retro3d::Time operator ""  _h(unsigned long long int h)  { return retro3d::Time::Hours(h); }
retro3d::Time operator ""  _d(unsigned long long int d)  { return retro3d::Time::Days(d); }
retro3d::Time operator ""  _w(unsigned long long int w)  { return retro3d::Time::Weeks(w); }
