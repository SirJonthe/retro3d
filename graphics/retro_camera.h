#ifndef RETRO_CAMERA_H
#define RETRO_CAMERA_H

#include "../common/MiniLib/MML/mmlMatrix.h"
#include "../common/MiniLib/MML/mmlVector.h"
#include "../api/tiny3d/tiny_math.h"
#include "../common/retro_geom.h"

namespace retro3d
{

class Camera
{	
private:
	float                    m_head;
	float                    m_pitch; // [-pi, x, pi]
	float                    m_roll;
	mmlMatrix<3,3>           m_rx;
	mmlMatrix<3,3>           m_ry;
	mmlMatrix<3,3>           m_rz;
	mmlVector<3>             m_pos;
	retro3d::SharedTransform m_view_to_world;

public:
	static mmlVector<3> GlobalUp( void );
	static mmlVector<3> GlobalRight( void );
	static mmlVector<3> GlobalForward( void );

public:
	Camera( void );
	Camera(const retro3d::Camera &c);
	Camera &operator=(const retro3d::Camera &c);

	mmlVector<3> GroundedForward( void ) const;
	mmlVector<3> GroundedUp( void ) const;
	mmlVector<3> GroundedRight( void ) const;
	mmlVector<3> Forward( void ) const;
	mmlVector<3> Up( void ) const;
	mmlVector<3> Right( void ) const;

	const mmlVector<3> &GetPosition( void ) const;
	void                SetPosition(const mmlVector<3> &pos);
	void                Move(const mmlVector<3> &move);
	void                LocalMove(const mmlVector<3> &move);
	void                LocalFly(const mmlVector<3> &move);

	const retro3d::SharedTransform GetTransform( void ) const;
	retro3d::Transform             GetViewTransform( void ) const;

	float GetHeadAngle( void ) const;
	float GetPitchAngle( void ) const;
	float GetRollAngle( void ) const;
	void  SetRotation(float x_head, float y_pitch, float z_roll);
	void  Rotate(float x_head, float y_pitch, float z_roll);

	mmlMatrix<3,3> GetRX( void ) const;
	mmlMatrix<3,3> GetRY( void ) const;
	mmlMatrix<3,3> GetRZ( void ) const;
};

}

#endif // RETRO_CAMERA_H
