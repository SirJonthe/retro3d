#ifndef RETRO_GEOM_H
#define RETRO_GEOM_H

#include "MiniLib/MML/mmlVector.h"
#include "MiniLib/MML/mmlMatrix.h"
#include "../api/tiny3d/tiny_math.h"
#include "../api/tiny3d/tiny_structs.h"
#include "retro_transform.h"
#include "retro_containers.h"
#include "retro_assert.h"

namespace retro3d
{

using Point = tiny3d::Point;
using Rect = tiny3d::Rect;
using UPoint = tiny3d::UPoint;
using URect = tiny3d::URect;

class Plane;

enum Contain
{
	Contain_False   = 0,
	Contain_Partial = 1,
	Contain_Full    = 2
};

class AABB
{
private:
	mmlVector<3> m_center;
	mmlVector<3> m_half_extents;

public:
	AABB( void );
	AABB(const mmlVector<3> &p_center, const mmlVector<3> &p_half_extents);
	AABB(const retro3d::Array< mmlVector<3> > &v);
	AABB(const AABB &aabb) = default;
	AABB &operator=(const AABB &aabb) = default;

	mmlVector<3> GetCenter( void ) const;
	void         SetCenter(const mmlVector<3> &center);
	mmlVector<3> GetHalfExtents( void ) const;
	void         SetHalfExtents(const mmlVector<3> &half_extents);
	mmlVector<3> GetExtents( void ) const;

	mmlVector<3> GetMin( void ) const;
	void         SetMin(mmlVector<3> min);
	mmlVector<3> GetMax( void ) const;
	void         SetMax(mmlVector<3> max);
	void         SetExtremes(mmlVector<3> min, mmlVector<3> max);

	AABB ApplyTransform(const retro3d::Transform &transform) const;

	bool             Overlaps(const retro3d::AABB &aabb) const;
	retro3d::Contain Contains(const retro3d::AABB &aabb) const;
	retro3d::Contain Contains(const mmlVector<3> &point) const;

	float GetArea( void ) const;
	float GetVolume( void ) const;

	void  FromGeometry(const retro3d::Array< mmlVector<3> > &v);
	void  GetCorners(mmlVector<3> *out) const;
	void  GetCorners(retro3d::Array< mmlVector<3> > &out) const;
	float GetDistance(const retro3d::Plane &plane) const;

	float GetInnerRadius( void ) const;
	float GetOuterRadius( void ) const;
};

AABB operator &(const AABB &l, const AABB &r); // find overlapping AABB
AABB operator |(const AABB &l, const AABB &r); // add AABB to AABB

struct Vertex
{
	mmlVector<3> v;
	mmlVector<2> t;
	mmlVector<3> c;

	Vertex( void );
	Vertex(const mmlVector<3> &position, const mmlVector<2> &tex_coord, const mmlVector<3> &color);
	Vertex(const Vertex &v) = default;
	Vertex &operator=(const Vertex &v) = default;
};

mmlVector<3> NormalFromAToB(const mmlVector<3> &a, const mmlVector<3> &b);
mmlVector<3> VectorFromAToB(const mmlVector<3> &a, const mmlVector<3> &b);
float TriangleArea2D(const mmlVector<2> &a, const mmlVector<2> &b, const mmlVector<2> &c);
float TriangleArea3D(const mmlVector<3> &a, const mmlVector<3> &b, const mmlVector<3> &c);
bool  IsVerySmallPolygon(const retro3d::Array<retro3d::Vertex> &poly);
bool  IsVerySmallPolygon(const retro3d::Array< mmlVector<3> > &poly);
bool  IsVerySmallTriangle(const mmlVector<3> &a, const mmlVector<3> &b, const mmlVector<3> &c);
float GetTriangleDegenerativity(const mmlVector<3> &a, const mmlVector<3> &b, const mmlVector<3> &c);
bool  IsDegenerateTriangle(const mmlVector<3> &a, const mmlVector<3> &b, const mmlVector<3> &c);
mmlVector<3> CartesianToBarycentric(const mmlVector<2> &coord, const mmlVector<2> &a, const mmlVector<2> &b, const mmlVector<2> &c);
mmlVector<2> BarycentricToCartesian(const mmlVector<3> &coord, const mmlVector<2> &a, const mmlVector<2> &b, const mmlVector<2> &c);
bool IsInTriangle(const mmlVector<2> &point, const mmlVector<2> &a, const mmlVector<2> &b, const mmlVector<2> &c);
template < typename type_t > type_t BarycentricInterpolation(const mmlVector<3> &barycentric_coord, const type_t &a, const type_t &b, const type_t &c) { return barycentric_coord[0]*a + barycentric_coord[1]*b + barycentric_coord[2]*c; }

class PolyHull
{
public:
	struct Index
	{
	};
	typedef retro3d::Array< PolyHull::Index > Poly;

private:
	retro3d::Array< PolyHull::Poly > m_polys; // TODO: This is sloppy and can be made into ONE linear array.

public:
	PolyHull( void ) = default;
	PolyHull(const retro3d::PolyHull &hull);
	PolyHull(const retro3d::Array< retro3d::PolyHull::Poly > *hull, uint32_t poly_count);
	PolyHull &operator=(const retro3d::PolyHull &hull);

	void Append(const retro3d::PolyHull &hull);
	void Append(const retro3d::Array< retro3d::PolyHull::Poly > *hull, uint32_t poly_count);

	void Destroy( void );

	void ReverseDirection( void );
};

class VertexArray
{
private:
	retro3d::Array< mmlVector<3> > m_points;
	mmlVector<3>                   m_center_of_mass;
	retro3d::AABB                  m_aabb;

private:
	void Refresh( void );

public:
	VertexArray( void );
	VertexArray(const retro3d::VertexArray &points) = default;
	VertexArray(const mmlVector<3> *points, uint32_t point_count);
	VertexArray &operator=(const retro3d::VertexArray &points) = default;

	void Append(const retro3d::VertexArray &points);
	void Append(const mmlVector<3> *points, uint32_t point_count);

	void Create(uint32_t count);
	void Destroy( void );

	void AlignToOrigin( void );
	void AxisAlign(int32_t x, int32_t y, int32_t z); // values -1, 0, 1
	void Scale(float scalar);
	void FlipXAxis( void );
	void FlipYAxis( void );
	void FlipZAxis( void );
	void FlipAxis( void );
	void ApplyTransform(const retro3d::Transform &transform);
	void CalculateConvexHull(retro3d::VertexArray *convex_points, retro3d::PolyHull *convex_hull);
	void Split(const retro3d::Plane &p, retro3d::VertexArray *front, retro3d::VertexArray *back);
	// convex_points != null -> return hull with index to convex_points
	// convex_points == null -> return hull with index to the source points
	retro3d::AABB GetAABB( void ) const;
	mmlVector<3> GetCenterOfVolume( void ) const;
	mmlVector<3> GetCenterOfMass( void ) const;
	float        GetScale( void ) const;

	uint32_t GetSize( void ) const;
	operator mmlVector<3>*( void );
	operator const mmlVector<3>*( void ) const;
};

class UVArray
{
private:
	retro3d::Array< mmlVector<2> > m_uvs;

public:
	UVArray( void ) = default;
	UVArray(const retro3d::UVArray &uvs) = default;
	UVArray(const mmlVector<2> *uvs, uint32_t uv_count);
	UVArray &operator=(const retro3d::UVArray &uvs) = default;

	void Append(const retro3d::UVArray &uvs);
	void Append(const mmlVector<2> *uvs, uint32_t uv_count);

	void Create(uint32_t count);
	void Destroy( void );

	void FlipU( void );
	void FlipV( void );
	void FlipUV( void );

	uint32_t GetSize( void ) const;
	operator mmlVector<2>*( void );
	operator const mmlVector<2>*( void ) const;
};

class NormalArray
{
private:
	retro3d::Array< mmlVector<3> > m_normals;

public:
	NormalArray( void ) = default;
	NormalArray(const retro3d::NormalArray &normals) = default;
	NormalArray(const mmlVector<3> *normals, uint32_t normal_count);
	NormalArray &operator=(const retro3d::NormalArray &normals) = default;

	void Append(const retro3d::NormalArray &normals);
	void Append(const mmlVector<3> *normals, uint32_t normal_count);

	void Create(uint32_t count);
	void Destroy( void );

	void ApplyTransform(const retro3d::Transform &transform); // applies transform safely
	void CreateVertexNormals(const retro3d::VertexArray &points, const retro3d::PolyHull &hull); // creates one averaged normal per vertex
	void CreateFaceNormals(const retro3d::VertexArray &points, const retro3d::PolyHull &hull); // creates one normal per face
	void FlipXAxis( void );
	void FlipYAxis( void );
	void FlipZAxis( void );
	void FlipAxis( void );

	uint32_t GetSize( void ) const;
	operator mmlVector<3>*( void );
	operator const mmlVector<3>*( void ) const;
};

class Plane
{
private:
	mmlVector<3> m_position;
	mmlVector<3> m_normal;

public:
	Plane( void );
	Plane(const mmlVector<3> &p_position, const mmlVector<3> &p_normal);
	Plane(const mmlVector<3> &a, const mmlVector<3> &b, const mmlVector<3> &c);
	Plane(const Plane &p) = default;
	Plane &operator=(const Plane &p) = default;

	float    GetDistance(const mmlVector<3> &p) const;
	int32_t  DetermineSide(float dist, float FP_EPSILON = std::numeric_limits<float>::epsilon()) const;
	int32_t  DetermineSide(const retro3d::Vertex &v, float FP_EPSILON = std::numeric_limits<float>::epsilon()) const;
	int32_t  DetermineSide(const mmlVector<3> &v, float FP_EPSILON = std::numeric_limits<float>::epsilon()) const;
	uint32_t Clip(const retro3d::Vertex &a, const retro3d::Vertex &b, const retro3d::Vertex &c, retro3d::Vertex (&out)[4], float FP_EPSILON = std::numeric_limits<float>::epsilon()) const;

	template < typename type_t >
	uint32_t Clip(const mmlVector<3> &a, const mmlVector<3> &b, const mmlVector<3> &c, const type_t &a_attr, const type_t &b_attr, const type_t &c_attr, type_t (&out_attr)[4], float FP_EPSILON = std::numeric_limits<float>::epsilon()) const;
	template < typename type_t >
	void     Clip(const retro3d::Array< mmlVector<3> > &poly, const retro3d::Array<type_t> &attr, retro3d::Array<type_t> *front_attr, retro3d::Array<type_t> *back_attr, float FP_EPSILON = std::numeric_limits<float>::epsilon()) const;

	mmlVector<3> GetClosestPoint(const mmlVector<3> &pt) const;

	const mmlVector<3> &GetPosition( void ) const;
	const mmlVector<3> &GetNormal( void ) const;
	float GetDistance( void ) const;

	mmlVector<3> ProjectPoint(const mmlVector<3> &pt) const;
	mmlVector<2> ProjectPointTo2D(const mmlVector<3> &pt, const mmlVector<3> &x_normal = retro3d::Transform::GetWorldRight()) const;

	Plane ApplyTransform(const retro3d::Transform &transform) const;
};

class Frustum
{
private:
	enum Planes
	{
		Plane_Near,
		Plane_Far,
		Plane_FixedCount
	};

private:
	retro3d::Array< retro3d::Plane > m_planes;
	retro3d::Array< mmlVector<3> >   m_verts;
	mmlVector<3>                     m_origin;
	float                            m_zfar;

public:
	Frustum( void );
	Frustum(const mmlVector<3> &view_point, const retro3d::Array< mmlVector<3> > &port_verts, float zfar);
	Frustum(const mmlVector<3> &view_point, const mmlVector<3> *port_verts, int32_t port_vert_count, float zfar);
	Frustum(const Frustum&) = default;
	Frustum &operator=(const Frustum&) = default;

	void SetFrustum(const mmlVector<3> &view_point, const retro3d::Array< mmlVector<3> > &port_verts, float zfar);
	void SetFrustum(const mmlVector<3> &view_point, const mmlVector<3> *port_verts, int32_t port_vert_count, float zfar);
	void SetOrigin(const mmlVector<3> &view_point);

	retro3d::Contain Contains(const mmlVector<3> &pt, bool disregard_near = false, bool disregard_far = false) const;
	retro3d::Contain Contains(const retro3d::AABB &aabb, bool disregard_near = false, bool disregard_far = false) const;

	mmlVector<3> GetViewDirection( void ) const;
	mmlVector<3> GetPortalDirection( void ) const;
	mmlVector<3> GetOrigin( void ) const;

	uint32_t GetPlaneCount( void ) const;
	uint32_t GetPortalVertexCount( void ) const;
	uint32_t GetFrustumVertexCount( void ) const;

	mmlVector<3> GetPortalVertex(uint32_t i) const;

	Frustum ApplyTransform(const retro3d::Transform &transform) const;
	void    GetCorners(mmlVector<3> *out) const;
	void    GetCorners(retro3d::Array< mmlVector<3> > &out) const;

	retro3d::Plane GetPortalPlane( void ) const;
};

}

template < typename type_t >
uint32_t retro3d::Plane::Clip(const mmlVector<3> &a, const mmlVector<3> &b, const mmlVector<3> &c, const type_t &a_attr, const type_t &b_attr, const type_t &c_attr, type_t (&out_attr)[4], float FP_EPSILON) const
{
	constexpr uint32_t TVERT = 3;
	const mmlVector<3> (&t)[TVERT] = { a, b, c };
	const type_t (&attr)[TVERT] = { a_attr, b_attr, c_attr };
	float dist[TVERT];
	int32_t side[TVERT];
	uint32_t num_in = 0;
	uint32_t num_out = 0;

	for (uint32_t i = 0; i < TVERT; ++i) {
		dist[i] = mmlDot(t[i] - m_position, m_normal);
		side[i] = DetermineSide(dist[i], FP_EPSILON);
		if (side[i] < 0) { ++num_out; }
	}

	if (num_out == TVERT) { return 0; }

	for (uint32_t i = 0, j = TVERT-1; i < TVERT; j=i, ++i) {

		if ((side[i] < 0) == (side[j] >= 0)) {
			const float x = dist[i] / (dist[i] - dist[j]);
			out_attr[num_in++] = mmlLerp(attr[i], attr[j], x);
		}
		if (side[i] >= 0) {
			out_attr[num_in++] = attr[i];
		}
	}

	return num_in;
}

template < typename type_t >
void retro3d::Plane::Clip(const retro3d::Array< mmlVector<3> > &poly, const retro3d::Array<type_t> &attr, retro3d::Array<type_t> *front_attr, retro3d::Array<type_t> *back_attr, float FP_EPSILON) const
{
	RETRO3D_ASSERT(poly.GetSize() == attr.GetSize());

	mtlArray<float>                dist(poly.GetSize());
	mtlArray<int32_t>              side(poly.GetSize());
	retro3d::Array< mmlVector<3> > front_vert, back_vert;

	front_vert.SetCapacity(poly.GetSize() + 1);
	back_vert.SetCapacity(front_vert.GetSize());

	if (front_attr != nullptr) { front_attr->SetCapacity(front_vert.GetSize()); front_attr->Resize(0); }
	if (back_attr  != nullptr) { back_attr->SetCapacity(front_vert.GetSize());  back_attr->Resize(0); }

	uint32_t in = 0, out = 0, on = 0;
	for (int i = 0; i < poly.GetSize(); ++i) {
		dist[i] = mmlDot(poly[i] - m_position, m_normal);
		side[i] = DetermineSide(dist[i], FP_EPSILON);
		switch (side[i]) {
		case -1: ++out; break;
		case  0: ++on;  break;
		case  1: ++in;  break;
		}
	}

	// NOTE: Early exit if polygon is wholly on one side of the plane.
	if (on == uint32_t(poly.GetSize()) && on > 2) { // NOTE: If all points are roughly on the same plane, use the polygon normal as determining factor where to put the attributes.
		const mmlVector<3> n = mmlSurfaceNormal(poly[0], poly[1], poly[2]);
		if (mmlDot(n, m_normal) >= 0.0f) { *front_attr = attr; }
		else                             { *back_attr = attr; }
		return;
	}
	if (out == 0) { // NOTE: If there are no points outside the plane, then they are either in front or on the plane. Store in front.
		*front_attr = attr;
		return;
	}
	if (in == 0) { // NOTE: If there are no points inside the plane, then they are either behind or on the plane. Store in back.
		*back_attr = attr;
		return;
	}

	for (int i = 0, j = poly.GetSize() - 1; i < poly.GetSize(); j=i, ++i) {

		if ((side[i] < 0) == (side[j] >= 0)) {
			const float x = dist[i] / (dist[i] - dist[j]);

			type_t a = mmlLerp(attr[i], attr[j], x);
			if (front_attr != nullptr) { front_attr->Add(a); }
			if (back_attr  != nullptr) { back_attr->Add(a); }

			mmlVector<3> v = mmlLerp(poly[i], poly[j], x);
			front_vert.Add(v);
			back_vert.Add(v);
		}
		if (side[i] >= 0) {
			if (front_attr != nullptr) { front_attr->Add(attr[i]); }
			front_vert.Add(poly[i]);
		}
		if (side[i] < 0) {
			if (back_attr != nullptr) { back_attr->Add(attr[i]); }
			back_vert.Add(poly[i]);
		}
	}

	// NOTE: If one of the sides only contain a very small polygon, then summarily say there is no clipping.
	if (front_vert.GetSize() <= 2 || retro3d::IsVerySmallPolygon(front_vert) == true) {
		if (front_attr != nullptr) { front_attr->Free(); }
		if (back_attr  != nullptr) { *back_attr = attr; }
	} else if (back_vert.GetSize() <= 2 || retro3d::IsVerySmallPolygon(back_vert) == true) {
		if (front_attr != nullptr) { *front_attr = attr; }
		if (back_attr  != nullptr) { back_attr->Free(); }
	}
}

#endif // RETRO_GEOM_H
