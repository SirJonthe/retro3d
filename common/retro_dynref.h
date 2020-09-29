#ifndef RETRO_DYNREF_H
#define RETRO_DYNREF_H

#include <cstdint>
#include "MiniLib/MTL/mtlPointer.h"
#include "retro_assert.h"

namespace retro3d
{

class IDynRef;

// DynRef
// A dynamic reference that automatically updates to null when the owner destroys the object being referred to.
template < typename ref_t >
class DynRef
{
	friend class IDynRef;

private:
	mtlShared< retro3d::IDynRef* >  m_ref;
	ref_t                          *m_data;

public:
	void Release( void );
	bool IsNull( void ) const;

	ref_t *operator->( void );
	const ref_t *operator->( void ) const;
};

// IDynRef
// The interface of an object that can be dynamically referred to.
class IDynRef
{
private:
	mtlShared< retro3d::IDynRef* > m_self_ref;

public:
	IDynRef( void );
	IDynRef(const IDynRef&) = delete;
	IDynRef &operator=(const IDynRef&) = delete;
	~IDynRef( void );

	template < typename ref_t >
	DynRef<ref_t> CreateReference( void );
};

}

template < typename ref_t >
void retro3d::DynRef<ref_t>::Release( void )
{
	m_data = nullptr;
	m_ref.Delete();
}

template < typename ref_t >
bool retro3d::DynRef<ref_t>::IsNull( void ) const
{
	return m_ref.IsNull() || *m_ref.GetShared() == nullptr;
}

template < typename ref_t >
ref_t *retro3d::DynRef<ref_t>::operator->( void )
{
	return !IsNull() ? m_data : nullptr;
}

template < typename ref_t >
const ref_t *retro3d::DynRef<ref_t>::operator->( void ) const
{
	return !IsNull() ? m_data : nullptr;
}

template < typename ref_t >
retro3d::DynRef<ref_t> retro3d::IDynRef::CreateReference( void )
{
	retro3d::DynRef<ref_t> ref;
	ref.m_ref = m_self_ref;
	ref.m_data = this;
	return ref;
}

#endif // RETRO_DYNREF_H
