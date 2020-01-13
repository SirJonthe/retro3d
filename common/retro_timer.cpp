#include <chrono>
#include "retro_timer.h"
#include "MiniLib/MML/mmlMath.h"

#define MS_PER_SEC 1000

void retro3d::Timer::UpdateTimer( void ) const
{
	if (m_ticking) {
		uint64_t time = GetProgramTimeMS();
		m_acc_time += time - m_time_last;
		m_time_last = time;
	}
}

uint64_t retro3d::Timer::GetProgramTimeMS( void )
{
	return uint64_t(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
}

retro3d::Timer::Timer( void ) : m_parent(nullptr), m_bps(0.0), m_beat_interval(0), m_acc_time(0), m_time_last(0), m_ticking(false)
{
	SetTempo(1.0, BeatsPerSecond);
}

retro3d::Timer::Timer(double tempo, retro3d::Timer::Units units): m_bps(0.0), m_beat_interval(0), m_acc_time(0), m_time_last(GetProgramTimeMS()), m_ticking(false)
{
	SetTempo(tempo, units);
}

void retro3d::Timer::SetTempo(double tempo, retro3d::Timer::Units units)
{
	tempo = mmlMax(tempo, 0.0);
	if (units == BeatsPerSecond) {
		m_bps = tempo;
	} else if (units == SecondsPerBeat) {
		m_bps = tempo > 0.0 ? 1.0 / tempo : 0.0;
	} else {
		m_bps = 0.0;
	}
	m_beat_interval = uint64_t(MS_PER_SEC / m_bps);
}

double retro3d::Timer::GetTempo(retro3d::Timer::Units units) const
{
	if (units == BeatsPerSecond) {
		return m_bps;
	} else if (units == SecondsPerBeat) {
		return m_bps > 0.0 ? 1.0 / m_bps : 0.0;
	}
	return 0.0;
}

void retro3d::Timer::Start( void )
{
	if (!IsTicking()) {
		m_time_last = GetProgramTimeMS();
		m_ticking = true;
	}
}

void retro3d::Timer::Stop( void )
{
	if (IsTicking()) {
		UpdateTimer();
		m_ticking = false;
	}
}

void retro3d::Timer::Toggle( void )
{
	if (IsTicking()) {
		Stop();
	} else {
		Start();
	}
}

void retro3d::Timer::Reset( void )
{
	m_acc_time = 0;
}

void retro3d::Timer::ResetPartBeat( void )
{
	m_acc_time = m_acc_time - m_acc_time % m_beat_interval;
}

void retro3d::Timer::ResetBeats( void )
{
	m_acc_time = m_acc_time % m_beat_interval;
}

void retro3d::Timer::DecrementBeat( void )
{
	if (m_acc_time >= m_beat_interval) {
		m_acc_time -= m_beat_interval;
	}
}

bool retro3d::Timer::IsTicking( void ) const
{
	return m_ticking;
}

bool retro3d::Timer::IsStopped( void ) const
{
	return !m_ticking;
}

bool retro3d::Timer::IsDue( void ) const
{
	return GetTime() >= 1.0;
}

uint64_t retro3d::Timer::GetFixedTime( void ) const
{
	UpdateTimer();
	return (m_acc_time * MS_PER_SEC) / m_beat_interval;
}

double retro3d::Timer::GetTime( void ) const
{
	UpdateTimer();
	return double(m_acc_time) / double(m_beat_interval);
}

int retro3d::Timer::GetBeats( void ) const
{
	UpdateTimer();
	return int(m_acc_time / m_beat_interval);
}

double retro3d::Timer::GetPartBeat( void ) const
{
	UpdateTimer();
	return double(m_acc_time % m_beat_interval) / double(m_beat_interval);
}
