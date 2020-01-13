#include "retro_transform.h"

bool retro3d::Transform::SelfRef(const retro3d::Transform *t) const
{
	return t == this || (m_parent.IsNull() == false ? m_parent->SelfRef(t) : false);
}

mmlVector<3> retro3d::Transform::GetWorldRight( void )
{
	mmlVector<3> x;
	x[0] = 1.0f;
	x[1] = 0.0f;
	x[2] = 0.0f;
	return x;
}

mmlVector<3> retro3d::Transform::GetWorldUp( void )
{
	mmlVector<3> y;
	y[0] = 0.0f;
	y[1] = 1.0f;
	y[2] = 0.0f;
	return y;
}

mmlVector<3> retro3d::Transform::GetWorldForward( void )
{
	mmlVector<3> z;
	z[0] = 0.0f;
	z[1] = 0.0f;
	z[2] = 1.0f;
	return z;
}

mmlVector<3> retro3d::Transform::GetWorldAxis(int axis_index)
{
	switch (axis_index) {
	case 0: return GetWorldRight();
	case 1: return GetWorldUp();
	case 2: return GetWorldForward();
	}
	return mmlVector<3>::Fill(0.0f);
}

std::string retro3d::Transform::GetWorldAxisName(int axis_index)
{
	switch (axis_index) {
	case 0: return "Right";
	case 1: return "Up";
	case 2: return "Forward";
	}
	return "Error";
}

retro3d::Transform::Transform(uint32_t space_from, uint32_t space_to) : mmlMatrix<4,4>(mmlMatrix<4,4>::Identity()), m_parent(), m_inv(mmlMatrix<4,4>::Identity()), m_rot(mmlMatrix<3,3>::Identity()), m_space_from(space_from), m_space_to(space_to), m_inv_updated(true), m_rot_updated(true)
{}

retro3d::Transform::Transform(const mmlMatrix<3,3> &basis, const mmlVector<3> &position) : Transform()
{
	SetTransform(basis, position);
}

retro3d::Transform::Transform(const mmlMatrix<4,4> &transform) : mmlMatrix<4,4>(transform), m_parent(), m_inv(mmlMatrix<4,4>::Identity()), m_rot(mmlMatrix<3,3>::Identity()), m_space_from(Space_Object), m_space_to(Space_World), m_inv_updated(false), m_rot_updated(false)
{}

retro3d::Transform &retro3d::Transform::operator=(const mmlMatrix<4,4> &transform)
{
	SetTransform(transform);
	return *this;
}

void retro3d::Transform::SetParent(mtlShared<const retro3d::Transform> parent)
{
	if (parent.IsNull() == false && SelfRef(parent.GetShared()) == false) {
		m_parent = parent;
	} else {
		OrphanTransform();
	}
}

void retro3d::Transform::OrphanTransform( void )
{
	m_parent.Delete();
}

void retro3d::Transform::SetTransform(const mmlMatrix<3,3> &basis, const mmlVector<3> &position)
{
	SetTransform(mmlTransform(basis, position));
}

void retro3d::Transform::SetTransform(const mmlMatrix<4,4> &transform)
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			(*this)[i][j] = transform[i][j];
		}
	}
	m_rot_updated = false;
	m_inv_updated = false;
}

void retro3d::Transform::SetRotation(const mmlMatrix<3,3> &rotation)
{
	const mmlVector<3> S = GetScale();
	for (int i = 0; i < 3; ++i) {
		const mmlVector<3> row = mmlNormalize(rotation[i]) * S[i];
		for (int j = 0; j < 3; ++j) {
			(*this)[i][j] = row[j];
		}
	}
	m_inv_updated = false;
	m_rot_updated = false;
}

void retro3d::Transform::SetBasis(const mmlMatrix<3,3> &basis)
{
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			(*this)[i][j] = basis[i][j];
		}
	}
	m_inv_updated = false;
	m_rot_updated = false;
}

void retro3d::Transform::SetPosition(const mmlVector<3> &position)
{
	for (int i = 0; i < 3; ++i) {
		(*this)[i][3] = position[i];
	}
	m_inv_updated = false;
	// does not affect rotation
}

void retro3d::Transform::NormalizeScale( void )
{
	mmlMatrix<3,3> rot = GetRotation();
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			(*this)[i][j] = rot[i][j];
		}
	}
	m_inv_updated = false;
	// does not affect rotation
}

void retro3d::Transform::SetScale(const mmlVector<3> &xyz)
{
	NormalizeScale();
	for (int i = 0; i < Rows - 1; ++i) {
		for (int j = 0; j < Columns - 1; ++j) {
			(*this)[i][j] *= xyz[i];
		}
	}
	m_inv_updated = false;
	// does not affect rotation
}

void retro3d::Transform::SetScale(float s)
{
	SetScale(mmlVector<3>::Fill(s));
}

mmlMatrix<4,4> retro3d::Transform::GetMatrix( void ) const
{
	return *this;
}

mmlMatrix<4,4> retro3d::Transform::GetInvMatrix( void ) const
{
	if (m_inv_updated == false) {
		m_inv = mmlInv(*this);
		m_inv_updated = true;
	}
	return m_inv;
}

mmlMatrix<3,3> retro3d::Transform::GetRotation( void ) const
{
	if (m_rot_updated == false) {
		const mmlVector<3> S = GetScale();
		for (int i = 0; i < 3; ++i) {
			m_rot[i] = mmlVector<3>((*this)[i]) / S[i];
		}
		m_rot_updated = true;
	}
	return m_rot;
}

mmlMatrix<3,3> retro3d::Transform::GetBasis( void ) const
{
	return mmlBasis(*this);
}

mmlVector<3> retro3d::Transform::GetPosition( void ) const
{
	return mmlTranslation(*this);
}

mmlVector<3> retro3d::Transform::GetScale( void ) const
{
	mmlVector<3> s;
	for (int i = 0; i < 3; ++i) {
		s[i] = mmlVector<3>((*this)[i]).Len();
	}
	return s;
}

mmlVector<3> retro3d::Transform::GetRight( void ) const
{
	return GetWorldRight() * GetRotation();
}

mmlVector<3> retro3d::Transform::GetUp( void ) const
{
	return GetWorldUp() * GetRotation();
}

mmlVector<3> retro3d::Transform::GetForward( void ) const
{
	return GetWorldForward() * GetRotation();
}

retro3d::Transform retro3d::Transform::GetInvTransform( void ) const
{
	Transform t;
	t = GetInvMatrix();
	t.m_inv = *this;
	t.m_inv_updated = true;
	t.m_rot_updated = false;
	return t;
}

mmlMatrix<4,4> retro3d::Transform::GetFinalMatrix( void ) const
{
	if (m_parent.IsNull() == true) {
		return *this;
	}
	return m_parent->GetFinalMatrix() * (*this);
}

mmlVector<3> retro3d::Transform::TransformPoint(const mmlVector<3> &point) const
{
	return point * GetFinalMatrix();
}

mmlVector<3> retro3d::Transform::TransformVector(const mmlVector<3> &vec) const
{
	return vec * mmlBasis(GetFinalMatrix());
}

mmlVector<3> retro3d::Transform::TransformNormal(const mmlVector<3> &normal) const
{
	mmlVector<3> n = normal * mmlRotation(GetFinalMatrix());
	n.NormalizeIf();
	return n;
}

mmlVector<3> retro3d::Transform::InverseTransformPoint(const mmlVector<3> &point) const
{
	return point * mmlInv(GetFinalMatrix());
}

mmlVector<3> retro3d::Transform::InverseTransformVector(const mmlVector<3> &vec) const
{
	return vec * mmlInv(mmlBasis(GetFinalMatrix()));
}

mmlVector<3> retro3d::Transform::InverseTransformNormal(const mmlVector<3> &normal) const
{
	return normal * mmlInv(mmlRotation(GetFinalMatrix()));
}
