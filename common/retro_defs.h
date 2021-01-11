#ifndef RETRO_DEFS_H
#define RETRO_DEFS_H

#include <stdint.h>
#include "MiniLib/MML/mmlMath.h"
#include "MiniLib/MTL/mtlPointer.h"

namespace retro3d
{

template < typename num_t, typename real_t >
num_t Cyclerp(const num_t &a, const num_t &b, const real_t &x)
{
	return mmlLerp(
		a, b,
		real_t(1.0) - (mmlCos(x * mmlPI) * real_t(0.5) + real_t(0.5))
	);
}

template < typename num_t, typename real_t >
num_t Accelerp(const num_t &a, const num_t &b, const real_t &x)
{
	return Cyclerp(
		a, b,
		mmlClamp(real_t(0.0), x, real_t(1.0))
	);
}

template < typename num_t, typename real_t >
num_t Sigmoid(const num_t &a, const num_t &b, const real_t &x)
{
	return mmlLerp(a, b, x / (real_t(1.0) + mmlAbs(x)));
}

template < typename real_t >
real_t Sigmoid(const real_t &x)
{
	return Sigmoid(real_t(0.0), real_t(1.0), x);
}

template < typename num_t, typename real_t >
num_t Jerklerp(const num_t &a, const num_t &b, const real_t &x)
{
	return Accelerp(b, a, x);
}

template < typename num_t, typename real_t >
num_t Berp(const num_t &a, const num_t &b, const num_t &c, const real_t &x)
{
	return mmlLerp(mmlLerp(a, c), mmlLerp(c, b), x);
}

template < typename type_t >
using Schematic = type_t (*)(void);

class IProcedure
{
public:
	virtual bool IsNull( void ) const = 0;
	virtual void operator()( void ) = 0;
};

class Procedure : public IProcedure
{
public:
	typedef void (*Function)(void);

private:
	Function m_fn;

public:
	Procedure( void ) : m_fn(nullptr) {}
	Procedure(Function fn) : m_fn(fn) {}
	Procedure(const Procedure&) = default;
	Procedure &operator=(const Procedure&) = default;

	bool IsNull( void ) const { return m_fn == nullptr; }
	void operator()( void ) { if (m_fn != nullptr) { m_fn(); } }
};

template < typename class_t >
class ClassProcedure : public IProcedure
{
public:
	typedef void (class_t::*MemberFunction)(void);

private:
	class_t        *m_this;
	MemberFunction  m_fn;

public:
	ClassProcedure( void ) : m_this(nullptr), m_fn(nullptr) {}
	ClassProcedure(class_t object, MemberFunction fn) : m_this(object), m_fn(fn) {}
	ClassProcedure(const ClassProcedure&) = default;
	ClassProcedure &operator=(const ClassProcedure&) = default;

	bool IsNull( void ) const { return m_this == nullptr || m_fn == nullptr; }
	void operator()( void ) { if (m_this != nullptr && m_fn != nullptr) { (m_this->*m_fn)(); } }
};

inline mtlShared<retro3d::IProcedure> CreateProcedure(retro3d::Procedure::Function fn)
{
	auto proc = mtlShared<retro3d::Procedure>::Create();
	*proc.GetShared() = retro3d::Procedure(fn);
	return proc;
}

template < typename class_t >
inline mtlShared<retro3d::IProcedure> CreateProcedure(class_t *object, typename retro3d::ClassProcedure<class_t>::MemberFunction fn)
{
	auto proc = mtlShared< retro3d::ClassProcedure<class_t> >::Create();
	*proc.GetShared() = retro3d::ClassProcedure<class_t>(object, fn);
	return proc;
}

}

#endif // RETRO_DEFS_H
