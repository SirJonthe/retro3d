#ifndef RETRO_TRANSFORM_H
#define RETRO_TRANSFORM_H

#include <string>
#include "MiniLib/MML/mmlVector.h"
#include "MiniLib/MML/mmlMatrix.h"
#include "MiniLib/MTL/mtlPointer.h"

namespace retro3d
{

class Transform : public mmlMatrix<4,4>
{
public:
	enum Space
	{
		Space_Object,
		Space_World,
		Space_View,
		Space_Screen,
		Space_Tangent
	};

private:
	mtlShared<const retro3d::Transform> m_parent;
	mutable mmlMatrix<4,4>              m_inv;
	mutable mmlMatrix<3,3>              m_rot;
	uint32_t                            m_space_from;
	uint32_t                            m_space_to; // Spaces can be used to determine if matrix operations are formally correct
	mutable bool                        m_inv_updated;
	mutable bool                        m_rot_updated;

private:
	bool SelfRef(const retro3d::Transform *t) const;

public:
	static mmlVector<3> GetWorldRight( void );
	static mmlVector<3> GetWorldUp( void );
	static mmlVector<3> GetWorldForward( void );
	static mmlVector<3> GetWorldAxis(int axis_index);
	static std::string  GetWorldAxisName(int axis_index);

public:
	Transform(uint32_t space_from = Space_Object, uint32_t space_to = Space_World);
	Transform(const mmlMatrix<3,3> &basis, const mmlVector<3> &position);
	Transform(const mmlMatrix<4,4> &transform);
	Transform &operator=(const retro3d::Transform &transform) = default;
	Transform &operator=(const mmlMatrix<4,4> &transform);

	void SetParent(mtlShared<const retro3d::Transform> parent);
	void OrphanTransform( void );

	void SetTransform(const mmlMatrix<3,3> &basis, const mmlVector<3> &position);
	void SetTransform(const mmlMatrix<4,4> &transform);

	void SetRotation(const mmlMatrix<3,3> &rotation);
	void SetBasis(const mmlMatrix<3,3> &basis);
	void SetPosition(const mmlVector<3> &position);

	void NormalizeScale( void );
	void SetScale(const mmlVector<3> &xyz);
	void SetScale(float scale);

	mmlMatrix<4,4>     GetMatrix( void ) const;
	mmlMatrix<4,4>     GetInvMatrix( void ) const;
	mmlMatrix<3,3>     GetRotation( void ) const;
	mmlMatrix<3,3>     GetBasis( void ) const;
	mmlVector<3>       GetPosition( void ) const;
	mmlVector<3>       GetScale( void ) const;
	mmlVector<3>       GetRight( void ) const;
	mmlVector<3>       GetUp( void ) const;
	mmlVector<3>       GetForward( void ) const;
	retro3d::Transform GetInvTransform( void ) const;

	mmlMatrix<4,4> GetFinalMatrix( void ) const;

	mmlVector<3> TransformPoint(const mmlVector<3> &point) const;
	mmlVector<3> TransformVector(const mmlVector<3> &vec) const;
	mmlVector<3> TransformNormal(const mmlVector<3> &normal) const;

	mmlVector<3> InverseTransformPoint(const mmlVector<3> &point) const;
	mmlVector<3> InverseTransformVector(const mmlVector<3> &vec) const;
	mmlVector<3> InverseTransformNormal(const mmlVector<3> &normal) const;

	// LookAt
};

typedef mtlShared< retro3d::Transform > SharedTransform;

}

#endif // RETRO_TRANSFORM_H
