#include "retro_camera.h"
#include "../common/retro_transform.h"

mmlVector<3> retro3d::Camera::GlobalUp( void )
{
	return retro3d::Transform::GetWorldUp();
}

mmlVector<3> retro3d::Camera::GlobalRight( void )
{
	return retro3d::Transform::GetWorldRight();
}

mmlVector<3> retro3d::Camera::GlobalForward( void )
{
	return retro3d::Transform::GetWorldForward();
}

retro3d::Camera::Camera( void ) : m_head(0.0f), m_pitch(0.0f), m_roll(0.0f), m_rx(mmlMatrix<3,3>::Identity()), m_ry(mmlMatrix<3,3>::Identity()), m_rz(mmlMatrix<3,3>::Identity()), m_pos(mmlVector<3>::Fill(0.0f)), m_view_to_world()
{
	m_view_to_world.New();
}

retro3d::Camera::Camera(const retro3d::Camera &c) : m_head(c.m_head), m_pitch(c.m_pitch), m_roll(c.m_roll), m_rx(c.m_rx), m_ry(c.m_ry), m_rz(c.m_rz), m_pos(c.m_pos)
{
	m_view_to_world.New();
	// there is no need to copy m_view_to_world since it depends on other values anyway
}

retro3d::Camera &retro3d::Camera::operator=(const retro3d::Camera &c)
{
	if (&c != this) {
		m_head  = c.m_head;
		m_pitch = c.m_pitch;
		m_roll  = c.m_roll;
		m_rx    = c.m_rx;
		m_ry    = c.m_ry;
		m_rz    = c.m_rz;
		m_pos   = c.m_pos;

		// don't need to do anything with m_view_to_world
	}
	return *this;
}

mmlVector<3> retro3d::Camera::GroundedForward( void ) const
{
	return GlobalForward() * m_ry;
}

mmlVector<3> retro3d::Camera::GroundedUp( void ) const
{
	return GlobalUp() * m_ry;
}

mmlVector<3> retro3d::Camera::GroundedRight( void ) const
{
	return GlobalRight() * m_ry;
}

mmlVector<3> retro3d::Camera::Forward( void ) const
{
	return GlobalForward() * (m_ry * m_rx);
}

mmlVector<3> retro3d::Camera::Up( void ) const
{
	return GlobalUp() * (m_ry * m_rx);
}

mmlVector<3> retro3d::Camera::Right( void ) const
{
	return GlobalRight() * (m_ry * m_rx);
}

const mmlVector<3> &retro3d::Camera::GetPosition( void ) const
{
	return m_pos;
}

void retro3d::Camera::SetPosition(const mmlVector<3> &pos)
{
	m_pos = pos;
	m_view_to_world->SetPosition(m_pos);
}

void retro3d::Camera::Move(const mmlVector<3> &move)
{
	SetPosition(m_pos + move);
}

void retro3d::Camera::LocalMove(const mmlVector<3> &move)
{
	Move(move[0] * GroundedRight() + move[1] * GroundedUp() + move[2] * GroundedForward());
}

void retro3d::Camera::LocalFly(const mmlVector<3> &move)
{
	Move(move[0] * GroundedRight() + move[1] * GroundedUp() + move[2] * Forward());
}

const retro3d::SharedTransform retro3d::Camera::GetTransform( void ) const
{
	return m_view_to_world;
}

retro3d::Transform retro3d::Camera::GetViewTransform( void ) const
{
	retro3d::Transform inv_view = m_view_to_world->GetInvTransform();
	return inv_view;
}

float retro3d::Camera::GetHeadAngle( void ) const
{
	return m_head;
}

float retro3d::Camera::GetPitchAngle( void ) const
{
	return m_pitch;
}

float retro3d::Camera::GetRollAngle( void ) const
{
	return m_roll;
}

void retro3d::Camera::SetRotation(float x_head, float y_pitch, float z_roll)
{
	constexpr float TAU = mmlPI*2.0f;
	constexpr float HPI = (mmlPI / 2) - 0.01f; // subtract a small amount
	constexpr float NHPI = -HPI;

	m_head  = mmlWrap(0.0f,  x_head,  TAU);
	m_pitch = mmlClamp(NHPI, y_pitch, HPI);
	m_roll  = mmlWrap(0.0f,  z_roll,  TAU);

	m_ry = mmlRotateY(m_head);
	m_rx = mmlRotateX(m_pitch);
	m_rz = mmlRotateZ(m_roll);

	*m_view_to_world.GetShared() = mmlTransform(m_ry * m_rx * m_rz, m_pos);
}

void retro3d::Camera::Rotate(float x_head, float y_pitch, float z_roll)
{
	SetRotation(m_head + x_head, m_pitch + y_pitch, m_roll + z_roll);
}

mmlMatrix<3,3> retro3d::Camera::GetRX( void ) const
{
	return m_rx;
}

mmlMatrix<3,3> retro3d::Camera::GetRY( void ) const
{
	return m_ry;
}

mmlMatrix<3,3> retro3d::Camera::GetRZ( void ) const
{
	return m_rz;
}
