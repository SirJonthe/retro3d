#include <chrono>
#include "retro_time.h"
#include "retro_assert.h"
#include "MiniLib/MML/mmlMath.h"

#define MS_PER_SEC 1000

retro3d::Time::Time(uint32_t ms) : time(uint64_t(ms) << SCALAR)
{
	RETRO3D_ASSERT(ms <= MAX_TIME_MS);
}

retro3d::Time retro3d::Time::FloatSeconds(double s) { return retro3d::Time(mmlRound(s * 1000.0)); }
retro3d::Time retro3d::Time::Seconds(uint32_t s)    { RETRO3D_ASSERT(s <= MAX_TIME_S); return retro3d::Time(s * 1000); }
retro3d::Time retro3d::Time::Minutes(uint32_t m)    { RETRO3D_ASSERT(m <= MAX_TIME_M); return Seconds(m * 60); }
retro3d::Time retro3d::Time::Hours(uint32_t h)      { RETRO3D_ASSERT(h <= MAX_TIME_H); return Minutes(h * 60); }
retro3d::Time retro3d::Time::Days(uint32_t d)       { RETRO3D_ASSERT(d <= MAX_TIME_D); return Hours(d * 24); }
retro3d::Time retro3d::Time::Weeks(uint32_t w)      { RETRO3D_ASSERT(w <= MAX_TIME_W); return Days(w * 7); }

uint64_t retro3d::Time::Now( void )
{
	return uint64_t(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
}

retro3d::Time retro3d::Time::TimeOfDay( void )
{
	const std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	auto *parts = std::localtime(&now);
	return retro3d::Time(parts->tm_hour * 1000 * 60 * 60 + parts->tm_min * 1000 * 60 + parts->tm_sec * 1000);
}

uint32_t retro3d::Time::CurrentYear( void )
{
	const std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	auto *parts = std::localtime(&now);
	return uint32_t(parts->tm_year) + 1900;
}

uint32_t retro3d::Time::CurrentWeekDay( void )
{
	const std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	auto *parts = std::localtime(&now);
	return uint32_t(parts->tm_wday);
}

uint32_t retro3d::Time::CurrentMonthDay( void )
{
	const std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	auto *parts = std::localtime(&now);
	return uint32_t(parts->tm_mday - 1);
}

uint32_t retro3d::Time::CurrentYearDay( void )
{
	const std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	auto *parts = std::localtime(&now);
	return uint32_t(parts->tm_wday);
}

uint32_t retro3d::Time::CurrentMonth( void )
{
	const std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	auto *parts = std::localtime(&now);
	return uint32_t(parts->tm_mon);
}

uint32_t retro3d::Time::GetTotalMS( void )      const { return uint32_t(time >> SCALAR); }
uint32_t retro3d::Time::GetTotalSeconds( void ) const { return GetTotalMS() / 1000; }
uint32_t retro3d::Time::GetTotalMinutes( void ) const { return GetTotalSeconds() / 60; }
uint32_t retro3d::Time::GetTotalHours( void )   const { return GetTotalMinutes() / 60; }
uint32_t retro3d::Time::GetTotalDays( void )    const { return GetTotalHours() / 24; }
uint32_t retro3d::Time::GetTotalWeeks( void )   const { return GetTotalDays() / 7; }

uint32_t retro3d::Time::GetPartMS( void )      const { return (time % 1000); }
uint32_t retro3d::Time::GetPartSeconds( void ) const { return (time % (1000 * 60)) / 1000; }
uint32_t retro3d::Time::GetPartMinutes( void ) const { return (time % (1000 * 60 * 60)) / (1000 * 60); }
uint32_t retro3d::Time::GetPartHours( void )   const { return (time % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60); }
uint32_t retro3d::Time::GetPartDays( void )    const { return (time % (1000 * 60 * 60 * 24 * 7)) / (1000 * 60 * 60 * 24); }
uint32_t retro3d::Time::GetPartWeeks( void )   const { return GetTotalWeeks(); }

double retro3d::Time::GetFloatSeconds( void ) const { return (time >> SCALAR) / 1000.0; }
double retro3d::Time::ToFloat( void ) const { return GetFloatSeconds(); }

bool retro3d::Time::IsZero( void ) const { return time == 0; }

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
	static constexpr uint64_t ONE = uint64_t(1) << SCALAR;
	if (r.time == ONE) { return *this; } // r is 1
	if (time   == ONE) { time = r.time; return *this; }
	mml::uint128 o = mml::mul128(mml::uint64(time), mml::uint64(r.time));
	time = (o.hi << SCALAR) | (o.lo >> SCALAR);
	return *this;
}

retro3d::Time &retro3d::Time::operator/=(const retro3d::Time &r)
{
	static constexpr uint64_t ONE = uint64_t(1) << SCALAR;
	if (r.time == ONE) { return *this; }
	mml::uint128 l = { time >> SCALAR, time << SCALAR };
	time = mml::div128(l, r.time);
	return *this;
}

bool retro3d::Time::operator==(const Time &r) const { return time == r.time; }
bool retro3d::Time::operator!=(const Time &r) const { return time != r.time; }
bool retro3d::Time::operator <(const Time &r) const { return time  < r.time; }
bool retro3d::Time::operator >(const Time &r) const { return time  > r.time; }
bool retro3d::Time::operator<=(const Time &r) const { return time <= r.time; }
bool retro3d::Time::operator>=(const Time &r) const { return time >= r.time; }

retro3d::Time operator+(retro3d::Time l, const retro3d::Time &r) { return l += r; }
retro3d::Time operator-(retro3d::Time l, const retro3d::Time &r) { return l -= r; }
retro3d::Time operator*(retro3d::Time l, const retro3d::Time &r) { return l *= r; }
retro3d::Time operator/(retro3d::Time l, const retro3d::Time &r) { return l /= r; }

retro3d::Time operator "" _ms(unsigned long long ms) { return retro3d::Time(ms); }
retro3d::Time operator ""  _s(unsigned long long s)  { return retro3d::Time::Seconds(s); }
retro3d::Time operator ""  _m(unsigned long long m)  { return retro3d::Time::Minutes(m); }
retro3d::Time operator ""  _h(unsigned long long h)  { return retro3d::Time::Hours(h); }
retro3d::Time operator ""  _d(unsigned long long d)  { return retro3d::Time::Days(d); }
retro3d::Time operator ""  _w(unsigned long long w)  { return retro3d::Time::Weeks(w); }

retro3d::Time retro3d::RealTimeTimer::GetTimeScale( void ) const
{
	return m_parent == nullptr ? m_time_scale : m_time_scale * m_parent->GetTimeScale();
}

retro3d::Time retro3d::RealTimeTimer::TimeDelta( void ) const
{
	return (retro3d::Time(retro3d::Time::Now()) - m_origin_time) * GetTimeScale();
}

retro3d::RealTimeTimer::RealTimeTimer(uint32_t num_ticks, retro3d::Time over_time) : m_parent(nullptr), m_time_scale(0), m_origin_time(retro3d::Time::Now()), m_accum_ticks(0), m_ticking(false)
{
	SetTickRate(num_ticks, over_time);
}

retro3d::Time retro3d::RealTimeTimer::UpdateTimer( void )
{
	if (m_ticking) {
		retro3d::Time now = retro3d::Time(retro3d::Time::Now());
		retro3d::Time delta = now - m_origin_time;
		m_accum_ticks += delta * GetTimeScale();
		m_origin_time = now;
	}
	return m_accum_ticks;
}

void retro3d::RealTimeTimer::SetTickRate(uint32_t num_ticks, retro3d::Time over_time, bool accumulate_time)
{
	RETRO3D_ASSERT(over_time.IsZero() != true && num_ticks > 0);
	if (accumulate_time) { UpdateTimer(); }
	m_time_scale = retro3d::Time::Seconds(num_ticks) / over_time;
}

void retro3d::RealTimeTimer::SetTickRate(const retro3d::RealTimeTimer &timer, bool accumulate_time)
{
	if (accumulate_time) { UpdateTimer(); }
	m_time_scale = timer.m_time_scale;
}

void retro3d::RealTimeTimer::SetTimeScale(retro3d::Time time_scale, bool accumulate_time)
{
	RETRO3D_ASSERT(time_scale.IsZero() != true);
	if (accumulate_time) { UpdateTimer(); }
	m_time_scale = time_scale;
}

retro3d::Time retro3d::RealTimeTimer::GetTimePerTick( void ) const
{
	return 1_s / m_time_scale;
}

void retro3d::RealTimeTimer::Start( void )
{
	if (!IsTicking()) {
		m_ticking = true;
		UpdateTimer();
	}
}

void retro3d::RealTimeTimer::Pause( void )
{
	if (IsTicking()) {
		UpdateTimer();
		m_ticking = false;
	}
}

void retro3d::RealTimeTimer::Toggle( void )
{
	if (IsTicking()) {
		Pause();
	} else {
		Start();
	}
}

void retro3d::RealTimeTimer::Reset( void )
{
	UpdateTimer();
	m_accum_ticks = 0_ms;
}

void retro3d::RealTimeTimer::ResetTicks( void )
{
	UpdateTimer();
	m_accum_ticks -= retro3d::Time::Seconds(m_accum_ticks.GetTotalSeconds());
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
	return GetTicks() >= 1;
}

retro3d::Time retro3d::RealTimeTimer::GetScaledTime( void ) const
{
	return m_ticking ? m_accum_ticks + TimeDelta() : m_accum_ticks;
}

retro3d::Time retro3d::RealTimeTimer::GetScaledUpdateTime( void ) const
{
	return m_accum_ticks;
}

uint32_t retro3d::RealTimeTimer::GetTicks( void ) const
{
	return GetScaledTime().GetTotalSeconds();
}

double retro3d::RealTimeTimer::GetTickProgress( void ) const
{
	const double sec = GetScaledTime().GetFloatSeconds();
	return sec - int64_t(sec);
}

void retro3d::RealTimeTimer::SetParent(const retro3d::RealTimeTimer *parent)
{
	UpdateTimer();
	m_parent = parent;
}

retro3d::RealTimeTimer retro3d::RealTimeTimer::SpawnChild(uint32_t num_ticks, retro3d::Time over_time) const
{
	RealTimeTimer timer(num_ticks, over_time);
	timer.SetParent(this);
	return timer;
}
