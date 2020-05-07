#include "retro_geom.h"
#include "MiniLib/MGL/mglCollision.h"
#include "retro_transform.h"

using namespace retro3d;
using namespace tiny3d;

retro3d::AABB::AABB( void ) :
	m_center(mmlVector<3>::Fill(0.0f)), m_half_extents(mmlVector<3>::Fill(0.0f))
{}

retro3d::AABB::AABB(const mmlVector<3> &p_center, const mmlVector<3> &p_half_extents) :
	m_center(p_center), m_half_extents(mmlAbs(p_half_extents))
{}

retro3d::AABB::AABB(const retro3d::Array< mmlVector<3> > &v)
{
	FromGeometry(v);
}

mmlVector<3> retro3d::AABB::GetCenter( void ) const
{
	return m_center;
}

void retro3d::AABB::SetCenter(const mmlVector<3> &center)
{
	m_center = center;
}

mmlVector<3> retro3d::AABB::GetHalfExtents( void ) const
{
	return m_half_extents;
}

void retro3d::AABB::SetHalfExtents(const mmlVector<3> &half_extents)
{
	m_half_extents = mmlAbs(half_extents);
}

mmlVector<3> retro3d::AABB::GetExtents( void ) const
{
	return m_half_extents * 2.0f;
}

mmlVector<3> retro3d::AABB::GetMin( void ) const
{
	return m_center - m_half_extents;
}

void retro3d::AABB::SetMin(mmlVector<3> min)
{
	mmlVector<3> max = GetMax();
	min = mmlMin(min, max);
	max = mmlMax(min, max);
	m_center = mmlLerp(max, min, 0.5f);
	m_half_extents = max - m_center;
}

mmlVector<3> retro3d::AABB::GetMax( void ) const
{
	return m_center + m_half_extents;
}

void retro3d::AABB::SetMax(mmlVector<3> max)
{
	mmlVector<3> min = GetMin();
	min = mmlMin(min, max);
	max = mmlMax(min, max);
	m_center = mmlLerp(max, min, 0.5f);
	m_half_extents = max - m_center;
}

void retro3d::AABB::SetExtremes(mmlVector<3> min, mmlVector<3> max)
{
	min = mmlMin(min, max);
	max = mmlMax(min, max);
	m_center = mmlLerp(min, max, 0.5f);
	m_half_extents = max - m_center;
}

retro3d::AABB retro3d::AABB::ApplyTransform(const retro3d::Transform &transform) const
{
	retro3d::Array< mmlVector<3> > aabb_corners;
	GetCorners(aabb_corners);
	for (int i = 0; i < aabb_corners.GetSize(); ++i) {
		aabb_corners[i] *= transform;
	}
	AABB aabb_out;
	aabb_out.FromGeometry(aabb_corners);
	return aabb_out;
}

bool retro3d::AABB::Overlaps(const retro3d::AABB &aabb) const
{
	for (int i = 0; i < 3; ++i) {
		if (mmlAbs(aabb.m_center[i] - m_center[i]) > (aabb.m_half_extents[i] + m_half_extents[i])) { return false; }
	}
	return true;
}

retro3d::Contain retro3d::AABB::Contains(const retro3d::AABB &aabb) const
{
	if (Overlaps(aabb) == false) { return retro3d::Contain_False; }

	for (int i = 0; i < 3; ++i) {
		if (aabb.m_half_extents[i] > m_half_extents[i]) { return retro3d::Contain_Partial; } // if the half extents are greather than the AABB, then it can not possibly fit inside container
		if (aabb.m_center[i] - m_center[i] + aabb.m_half_extents[i] > m_half_extents[i]) { return retro3d::Contain_Partial; }
	}
	return retro3d::Contain_Full;
}

retro3d::Contain retro3d::AABB::Contains(const mmlVector<3> &point) const
{
	mmlVector<3> min = GetMin();
	mmlVector<3> max = GetMax();
	for (int i = 0; i < 3; ++i) {
		if (point[i] < min[i] || point[i] > max[i]) { return retro3d::Contain_False; }
	}
	return retro3d::Contain_Full;
}

float retro3d::AABB::GetArea( void ) const
{
	return (m_half_extents[0]*m_half_extents[1] + m_half_extents[1]*m_half_extents[2] + m_half_extents[0]*m_half_extents[2]) * 8.0f;
}

float retro3d::AABB::GetVolume( void ) const
{
	return 8.0f * m_half_extents[0] * m_half_extents[1] * m_half_extents[2]; // 8 = 2*2*2
}

void retro3d::AABB::FromGeometry(const retro3d::Array< mmlVector<3> > &v)
{
	if (v.GetSize() > 0) {
		mmlVector<3> min = v[0];
		mmlVector<3> max = min;
		for (int i = 1; i < v.GetSize(); ++i) {
			mmlVector<3> x = v[i];
			min = mmlMin(min, x);
			max = mmlMax(max, x);
		}
		SetExtremes(min, max);
	} else {
		m_center = m_half_extents = mmlVector<3>::Fill(0.0f);
	}
}

void retro3d::AABB::GetCorners(mmlVector<3> *out) const
{
	mmlVector<3> min = GetMin();
	mmlVector<3> max = GetMax();

	out[0] = min;
	out[1] = mmlVector<3>(double(max[0]), double(min[1]), double(min[2]));
	out[2] = mmlVector<3>(double(max[0]), double(max[1]), double(min[2]));
	out[3] = mmlVector<3>(double(min[0]), double(max[1]), double(min[2]));

	out[4] = mmlVector<3>(double(min[0]), double(min[1]), double(max[2]));
	out[5] = mmlVector<3>(double(max[0]), double(min[1]), double(max[2]));
	out[6] = max;
	out[7] = mmlVector<3>(double(min[0]), double(max[1]), double(max[2]));
}

void retro3d::AABB::GetCorners(retro3d::Array< mmlVector<3> > &out) const
{
	out.Create(8);
	GetCorners(&out[0]);
}

float retro3d::AABB::GetDistance(const retro3d::Plane &plane) const
{
	mmlVector<3> v[8];
	GetCorners(v);
	float md = plane.GetDistance(v[0]);
	for (int i = 1; i < 8; ++i) {
		md = mmlMin(md, plane.GetDistance(v[i]));
	}
	return md;
}

float retro3d::AABB::GetInnerRadius( void ) const
{
	return mmlAbs(mmlMin(m_half_extents[0], m_half_extents[1], m_half_extents[2]));
}

float retro3d::AABB::GetOuterRadius( void ) const
{
	return m_half_extents.Len();
}

retro3d::AABB retro3d::operator&(const retro3d::AABB &l, const retro3d::AABB &r)
{
	mmlVector<3> a = mmlMax(l.GetMin(), r.GetMin());
	mmlVector<3> b = mmlMin(l.GetMax(), r.GetMax());
	retro3d::AABB aabb;
	aabb.SetExtremes(a, b);
	return aabb;
}

retro3d::AABB retro3d::operator|(const retro3d::AABB &l, const retro3d::AABB &r)
{
	mmlVector<3> a = mmlMin(l.GetMin(), r.GetMin());
	mmlVector<3> b = mmlMax(l.GetMax(), r.GetMax());
	retro3d::AABB aabb;
	aabb.SetExtremes(a, b);
	return aabb;
}

retro3d::Vertex::Vertex( void ) :
	v(mmlVector<3>::Fill(0.0f)), t(mmlVector<2>::Fill(0.0f)), c(mmlVector<3>::Fill(1.0f))
{}

retro3d::Vertex::Vertex(const mmlVector<3> &position, const mmlVector<2> &tex_coord, const mmlVector<3> &color) :
	v(position), t(tex_coord), c(color)
{}

mmlVector<3> retro3d::NormalFromAToB(const mmlVector<3> &a, const mmlVector<3> &b)
{
	return mmlNormalizeIf(b - a);
}

mmlVector<3> retro3d::VectorFromAToB(const mmlVector<3> &a, const mmlVector<3> &b)
{
	return b - a;
}

float retro3d::TriangleArea2D(const mmlVector<2> &a, const mmlVector<2> &b, const mmlVector<2> &c)
{
	return ((a[0]*b[1] - a[1]*b[0]) + (b[0]*c[1] - b[1]*c[0]) + (c[0]*a[1] - c[1]*a[0])) / 2.0f;
}

float retro3d::TriangleArea3D(const mmlVector<3> &a, const mmlVector<3> &b, const mmlVector<3> &c)
{
	return mmlCross(b - a, c - a).Len() / 2.0f;
}

bool retro3d::IsVerySmallPolygon(const retro3d::Array<retro3d::Vertex> &poly)
{
	float area = 0.0f;
	for (int v = 1; v < poly.GetSize() - 1; ++v) {
		area += retro3d::TriangleArea3D(poly[0].v, poly[v].v, poly[v+1].v);
	}
	return std::isnan(area) || mmlIsApproxEqual(area, 0.0f);
}

bool retro3d::IsVerySmallPolygon(const retro3d::Array< mmlVector<3> > &poly)
{
	float area = 0.0f;
	for (int v = 1; v < poly.GetSize() - 1; ++v) {
		area += retro3d::TriangleArea3D(poly[0], poly[v], poly[v+1]);
	}
	return std::isnan(area) || mmlIsApproxEqual(area, 0.0f);
}

bool retro3d::IsVerySmallTriangle(const mmlVector<3> &a, const mmlVector<3> &b, const mmlVector<3> &c)
{
	const float area = retro3d::TriangleArea3D(a, b, c);
	return std::isnan(area) || mmlIsApproxEqual(area, 0.0f);
}

float retro3d::GetTriangleDegenerativity(const mmlVector<3> &a, const mmlVector<3> &b, const mmlVector<3> &c)
{
	const float l1 = (a - b).Len();
	const float l2 = (a - c).Len();
	const float l3 = (b - c).Len();
	float A = 0.0f;
	float X = 0.0f;
	if (l1 >= l2 && l1 >= l3) {
		A = l1;
		X = l2 + l3;
	} else if (l2 >= l1 && l2 >= l3) {
		A = l2;
		X = l1 + l3;
	} else if (l3 >= l1 && l3 >= l2) {
		A = l3;
		X = l1 + l2;
	}
	const float B = sqrtf((A * A) / 2.0f) * 2.0f;
	const float SPAN = mmlAbs(B - A);
	const float d = (X - A) / SPAN;
	return 1.0f - d;
}

bool retro3d::IsDegenerateTriangle(const mmlVector<3> &a, const mmlVector<3> &b, const mmlVector<3> &c)
{
	return mmlIsApproxEqual(GetTriangleDegenerativity(a, b, c), 1.0f);
}

mmlVector<3> retro3d::CartesianToBarycentric(const mmlVector<2> &coord, const mmlVector<2> &a, const mmlVector<2> &b, const mmlVector<2> &c)
{
	const float INV_DET = 1.0f / ((b[1] - c[1]) * (a[0] - c[0]) + (c[0] - b[0]) * (a[1] - c[1]));
	mmlVector<3> bc;
	bc[0] = ((b[1] - c[1]) * (coord[0] - c[0]) + (c[0] - b[0]) * (coord[1] - c[1])) * INV_DET;
	bc[1] = ((c[1] - a[1]) * (coord[0] - c[0]) + (a[0] - c[0]) * (coord[1] - c[1])) * INV_DET;
	bc[2] = 1.0f - bc[0] - bc[1];
	return bc;
}

mmlVector<2> retro3d::BarycentricToCartesian(const mmlVector<3> &coord, const mmlVector<2> &a, const mmlVector<2> &b, const mmlVector<2> &c)
{
	mmlVector<2> cc;
	cc[0] = coord[0]*a[0] + coord[1]*b[0] + coord[2]*c[0];
	cc[1] = coord[0]*a[1] + coord[1]*b[1] + coord[2]*c[1];
	return cc;
}

bool retro3d::IsInTriangle(const mmlVector<2> &point, const mmlVector<2> &a, const mmlVector<2> &b, const mmlVector<2> &c)
{
	mmlVector<3> bc = retro3d::CartesianToBarycentric(point, a, b, c);
	return bc[0] >= 0.0f && bc[0] <= 1.0f && bc[1] >= 0.0f && bc[1] <= 1.0f && bc[2] >= 0.0f && bc[1] <= 1.0f;
}

void retro3d::VertexArray::Refresh( void )
{
	m_center_of_mass = mmlVector<3>::Fill(0.0f);
	const float InvCount = 1.0f / m_points.GetSize();
	for (int i = 0; i < m_points.GetSize(); ++i) {
		m_center_of_mass += m_points[i] / InvCount;
	}
	m_aabb.FromGeometry(m_points);
}

retro3d::VertexArray::VertexArray( void ) : m_points(), m_center_of_mass(mmlVector<3>::Fill(0.0f)), m_aabb()
{}

retro3d::VertexArray::VertexArray(const mmlVector<3> *points, uint32_t point_count) : VertexArray()
{
	m_points.Copy(points, int(point_count));
	Refresh();
}

void retro3d::VertexArray::Append(const mmlVector<3> *points, uint32_t point_count)
{
	int a_size = m_points.GetSize();
	int b_size = int(point_count);
	m_points.Resize(a_size + b_size);
	for (int i = 0; i < b_size; ++i) {
		m_points[i + a_size] = points[i];
	}

	// TODO: Find new center and AABB
}

void retro3d::VertexArray::Append(const retro3d::VertexArray &points)
{
	int a_size = m_points.GetSize();
	int b_size = points.m_points.GetSize();
	m_points.Resize(a_size + b_size);
	for (int i = 0; i < b_size; ++i) {
		m_points[i + a_size] = points[i];
	}

	m_center_of_mass = mmlLerp(m_center_of_mass, points.m_center_of_mass, b_size / float(m_points.GetSize()));
	m_aabb = m_aabb | points.m_aabb;
}

void retro3d::VertexArray::Create(uint32_t count)
{
	Destroy();
	m_points.Create(int(count));
}

void retro3d::VertexArray::Destroy( void )
{
	m_points.Free();
	m_center_of_mass = mmlVector<3>::Fill(0.0f);
	m_aabb = retro3d::AABB();
}

void retro3d::VertexArray::AlignToOrigin( void )
{
	mmlVector<3> c = GetCenterOfVolume();
	for (int i = 0; i < m_points.GetSize(); ++i) {
		m_points[i] -= c;
	}
	m_center_of_mass -= c;
	m_aabb.SetCenter(mmlVector<3>::Fill(0.0f));
}

void retro3d::VertexArray::AxisAlign(int32_t x, int32_t y, int32_t z)
{
	mmlVector<3> c = m_aabb.GetCenter();
	mmlVector<3> e = m_aabb.GetHalfExtents();
	mmlVector<3> t;
	t[0] = (x < 0) ? (c[0] + e[0]) : (x > 0 ? c[0] - e[0] : c[0]);
	t[1] = (y < 0) ? (c[1] + e[1]) : (y > 0 ? c[1] - e[1] : c[1]);
	t[2] = (z < 0) ? (c[2] + e[2]) : (z > 0 ? c[2] - e[2] : c[2]);
	for (int i = 0; i < m_points.GetSize(); ++i) {
		m_points[i] -= t;
	}
	m_center_of_mass -= t;
	m_aabb.SetCenter(m_aabb.GetCenter() - t);
}

void retro3d::VertexArray::Scale(float scalar)
{
	for (int i = 0; i < m_points.GetSize(); ++i) {
		m_points[i] *= scalar;
	}
	m_center_of_mass *= scalar;
	m_aabb.SetCenter(m_aabb.GetCenter() * scalar);
	m_aabb.SetHalfExtents(m_aabb.GetHalfExtents() * scalar);
}

void retro3d::VertexArray::FlipXAxis( void )
{
	for (int i = 0; i < m_points.GetSize(); ++i) {
		m_points[i][0] = -m_points[i][0];
	}
	m_center_of_mass[0] = -m_center_of_mass[0];
	mmlVector<3> c = m_aabb.GetCenter();
	c[0] = -c[0];
	m_aabb.SetCenter(c);
}

void retro3d::VertexArray::FlipYAxis( void )
{
	for (int i = 0; i < m_points.GetSize(); ++i) {
		m_points[i][1] = -m_points[i][1];
	}
	m_center_of_mass[1] = -m_center_of_mass[1];
	mmlVector<3> c = m_aabb.GetCenter();
	c[1] = -c[1];
	m_aabb.SetCenter(c);
}


void retro3d::VertexArray::FlipZAxis( void )
{
	for (int i = 0; i < m_points.GetSize(); ++i) {
		m_points[i][2] = -m_points[i][2];
	}
	m_center_of_mass[2] = -m_center_of_mass[2];
	mmlVector<3> c = m_aabb.GetCenter();
	c[2] = -c[2];
	m_aabb.SetCenter(c);
}


void retro3d::VertexArray::FlipAxis( void )
{
	for (int i = 0; i < m_points.GetSize(); ++i) {
		m_points[i] = -m_points[i];
	}
	m_center_of_mass = -m_center_of_mass;
	m_aabb.SetCenter(-m_aabb.GetCenter());
}

void retro3d::VertexArray::ApplyTransform(const retro3d::Transform &transform)
{
	const mmlMatrix<4,4> m = transform.GetFinalMatrix();
	for (int i = 0; i < m_points.GetSize(); ++i) {
		m_points[i] *= m;
	}
	Refresh();
}

void retro3d::VertexArray::CalculateConvexHull(retro3d::VertexArray *convex_points, retro3d::PolyHull *convex_hull)
{
}

void retro3d::VertexArray::Split(const retro3d::Plane &p, retro3d::VertexArray *front, retro3d::VertexArray *back)
{
	uint32_t front_count = 0;
	uint32_t back_count = 0;
	for (int i = 0; i < m_points.GetSize(); ++i) {
		p.DetermineSide(m_points[i]) >= 0 ? ++front_count : ++back_count;
	}

	if (front != nullptr) {
		front->Create(front_count);
		front_count = 0;
	}
	if (back != nullptr) {
		back->Create(back_count);
		back_count = 0;
	}

	for (int i = 0; i < m_points.GetSize(); ++i) {
		if (p.DetermineSide(m_points[i]) >= 0) {
			if (front != nullptr) {
				(*front)[front_count++] = m_points[i];
			}
		} else {
			if (back != nullptr) {
				(*back)[back_count++] = m_points[i];
			}
		}
	}
}

retro3d::AABB retro3d::VertexArray::GetAABB( void ) const
{
	return m_aabb;
}

mmlVector<3> retro3d::VertexArray::GetCenterOfVolume( void ) const
{
	return GetAABB().GetCenter();
}

mmlVector<3> retro3d::VertexArray::GetCenterOfMass( void ) const
{
	return m_center_of_mass;
}

float retro3d::VertexArray::GetScale( void ) const
{
	return mmlMax(mmlAbs(m_aabb.GetHalfExtents())) * 2.0f;
}

uint32_t retro3d::VertexArray::GetSize( void ) const
{
	return uint32_t(m_points.GetSize());
}

retro3d::VertexArray::operator mmlVector<3>*( void )
{
	return m_points;
}

retro3d::VertexArray::operator const mmlVector<3>*( void ) const
{
	return m_points;
}

retro3d::UVArray::UVArray(const mmlVector<2> *uvs, uint32_t uv_count) :
	m_uvs()
{
	m_uvs.Copy(uvs, int(uv_count));
}

void retro3d::UVArray::Append(const retro3d::UVArray &uvs)
{
	Append(uvs, uvs.GetSize());
}

void retro3d::UVArray::Append(const mmlVector<2> *uvs, uint32_t uv_count)
{
	int a_count = m_uvs.GetSize();
	int b_count = int(uv_count);
	m_uvs.Resize(a_count + b_count);
	for (int i = 0; i < b_count; ++i) {
		m_uvs[a_count + i] = uvs[i];
	}
}

void retro3d::UVArray::Create(uint32_t count)
{
	m_uvs.Create(int(count));
}

void retro3d::UVArray::Destroy( void )
{
	m_uvs.Free();
}

void retro3d::UVArray::FlipU( void )
{
	for (int i = 0; i < m_uvs.GetSize(); ++i) {
		m_uvs[i][0] = -m_uvs[i][0];
	}
}

void retro3d::UVArray::FlipV( void )
{
	for (int i = 0; i < m_uvs.GetSize(); ++i) {
		m_uvs[i][1] = -m_uvs[i][1];
	}
}

void retro3d::UVArray::FlipUV( void )
{
	for (int i = 0; i < m_uvs.GetSize(); ++i) {
		m_uvs[i] = -m_uvs[i];
	}
}

uint32_t retro3d::UVArray::GetSize( void ) const
{
	return uint32_t(m_uvs.GetSize());
}

retro3d::UVArray::operator mmlVector<2>*( void )
{
	return m_uvs;
}

retro3d::UVArray::operator const mmlVector<2>*( void ) const
{
	return m_uvs;
}

retro3d::NormalArray::NormalArray(const mmlVector<3> *normals, uint32_t normal_count)
{
	m_normals.Copy(normals, int(normal_count));
}

void retro3d::NormalArray::Append(const retro3d::NormalArray &normals)
{
	Append(normals, normals.GetSize());
}

void retro3d::NormalArray::Append(const mmlVector<3> *normals, uint32_t normal_count)
{
	int a_count = m_normals.GetSize();
	int b_count = int(normal_count);
	m_normals.Resize(a_count + b_count);
	for (int i = 0; i < b_count; ++i) {
		m_normals[a_count + i] = normals[i];
	}
}

void retro3d::NormalArray::Create(uint32_t count)
{
	m_normals.Create(int(count));
}

void retro3d::NormalArray::Destroy( void )
{
	m_normals.Free();
}

void retro3d::NormalArray::ApplyTransform(const retro3d::Transform &transform)
{
	const mmlMatrix<3,3> m = mmlRotation(transform.GetFinalMatrix());
	for (int i = 0; i < m_normals.GetSize(); ++i) {
		m_normals[i] *= m;
	}
}

/*void retro3d::NormalArray::CreateVertexNormals(const retro3d::VertexArray &points, const retro3d::PolyHull &hull)
{
	m_normals.Create(points.GetSize());

	for (int i = 0; i < m_normals.GetSize(); ++i) {
		m_normals[i] = mmlVector<3>::Fill(0.0f);
	}
	auto &material = inout_material;
	for (int j = 0; j < material.f.GetSize(); ++j) {
		auto &face = material.f[j];
		if (face.GetSize() < 3) { continue; }
		const mmlVector<3> A = points[ face[0].v ];
		mmlVector<3> N = mmlVector<3>::Fill(0.0f);
		for (int k = 1; k < face.GetSize() - 1; ++k) { // check the cross product of all sub-triangles in polygon to make sure there is no sub-triangle small enough to generate NaNs
			const mmlVector<3> B = points[ face[k    ].v ];
			const mmlVector<3> C = points[ face[k + 1].v ];
			const mmlVector<3> cross = mmlCross(B - A, C - A);
			if (!std::isnan(cross[0]) && !std::isnan(cross[1]) && !std::isnan(cross[2])) { // skip NaN normal
				N += cross;
			}
		}
		if (mmlIsApproxEqual(N.Len(), 0.0f) == true) {
			N[0] = N[1] = N[2] = 0.0f;
		} else {
			N.Normalize();
		}

		for (int k = 0; k < face.GetSize(); ++k) {
			auto &index = face[k];
			index.n = index.v;
			m_normals[index.n] += N;
		}
	}
	for (int i = 0; i < m_normals.GetSize(); ++i) {
		m_normals[i].NormalizeIf();
	}
}*/

/*void retro3d::NormalArray::CreateFaceNormals(const retro3d::VertexArray &points, const retro3d::PolyHull &hull)
{
	int32_t n_count = 0;
	for (int i = 0; i < inout_material.GetSize(); ++i) {
		n_count += inout_material[i].f.GetSize();
	}
	m_normals.Create(n_count);
	for (int i = 0; i < m_normals.GetSize(); ++i) { m_normals[i] = mmlVector<3>::Fill(0.0f); }

	n_count = 0;
	for (int i = 0; i < inout_material.GetSize(); ++i) {
		auto &material = inout_material[i];
		for (int j = 0; j < material.f.GetSize(); ++j, ++n_count) {
			auto &face = material.f[j];

			if (face.GetSize() < 3) { continue; }
			const mmlVector<3> A = points[ face[0].v ];
			m_normals[n_count] = mmlVector<3>::Fill(0.0f);
			for (int k = 1; k < face.GetSize() - 1; ++k) { // check the cross product of all sub-triangles in polygon to make sure there is no sub-triangle small enough to generate NaNs
				const mmlVector<3> B = points[ face[k    ].v ];
				const mmlVector<3> C = points[ face[k + 1].v ];
				const mmlVector<3> cross = mmlCross(B - A, C - A);
				if (!std::isnan(cross[0]) && !std::isnan(cross[1]) && !std::isnan(cross[2])) { // skip NaN normal
					m_normals[n_count] += cross;
				}
			}
			if (mmlIsApproxEqual(m_normals[n_count].Len(), 0.0f) == true) {
				m_normals[n_count] = mmlVector<3>::Fill(0.0f);
			} else {
				m_normals[n_count].NormalizeIf();
			}

			for (int k = 0; k < face.GetSize(); ++k) {
				face[k].n = n_count;
			}
		}
	}
}*/

void retro3d::NormalArray::FlipXAxis( void )
{
	for (int i = 0; i < m_normals.GetSize(); ++i) {
		m_normals[i][0] = -m_normals[i][0];
	}
}

void retro3d::NormalArray::FlipYAxis( void )
{
	for (int i = 0; i < m_normals.GetSize(); ++i) {
		m_normals[i][1] = -m_normals[i][1];
	}
}


void retro3d::NormalArray::FlipZAxis( void )
{
	for (int i = 0; i < m_normals.GetSize(); ++i) {
		m_normals[i][2] = -m_normals[i][2];
	}
}


void retro3d::NormalArray::FlipAxis( void )
{
	for (int i = 0; i < m_normals.GetSize(); ++i) {
		m_normals[i] = -m_normals[i];
	}
}

uint32_t retro3d::NormalArray::GetSize( void ) const
{
	return uint32_t(m_normals.GetSize());
}

retro3d::NormalArray::operator mmlVector<3>*( void )
{
	return m_normals;
}

retro3d::NormalArray::operator const mmlVector<3>*( void ) const
{
	return m_normals;
}

retro3d::Plane::Plane( void ) :
	m_position(mmlVector<3>::Fill(0.0f)), m_normal(retro3d::Transform::GetWorldForward())
{}

retro3d::Plane::Plane(const mmlVector<3> &p_position, const mmlVector<3> &p_normal) :
	m_position(p_position), m_normal(mmlNormalize(p_normal))
{}

retro3d::Plane::Plane(const mmlVector<3> &a, const mmlVector<3> &b, const mmlVector<3> &c) :
	m_position(a), m_normal(mmlSurfaceNormal(a, b, c))
{}

int32_t retro3d::Plane::DetermineSide(float dist, float FP_EPSILON) const
{
	return mmlIsApproxEqual(dist, 0.0f, FP_EPSILON) ? 0 : (dist > 0.0f ? 1 : -1);
}

float retro3d::Plane::GetDistance(const mmlVector<3> &v) const
{
	return mmlDot(v - m_position, m_normal);
}

int32_t retro3d::Plane::DetermineSide(const retro3d::Vertex &v, float FP_EPSILON) const
{
	return DetermineSide(v.v, FP_EPSILON);
}

int32_t retro3d::Plane::DetermineSide(const mmlVector<3> &v, float FP_EPSILON) const
{
	float dist = GetDistance(v);
	return DetermineSide(dist, FP_EPSILON);
}

uint32_t retro3d::Plane::Clip(const retro3d::Vertex &a, const retro3d::Vertex &b, const retro3d::Vertex &c, retro3d::Vertex (&out)[4], float FP_EPSILON) const
{
	constexpr uint32_t TVERT = 3;
	const Vertex (&t)[TVERT] = { a, b, c };
	float dist[TVERT];
	int32_t side[TVERT];
	uint32_t num_in = 0;
	uint32_t num_out = 0;

	for (uint32_t i = 0; i < TVERT; ++i) {
		dist[i] = mmlDot(t[i].v - m_position, m_normal);
		side[i] = DetermineSide(dist[i], FP_EPSILON);
		if (side[i] < 0) { ++num_out; }
	}

	if (num_out == TVERT) { return num_in; }

	for (uint32_t i = 0, j = TVERT-1; i < TVERT; j=i, ++i) {

		if ((side[i] < 0) == (side[j] >= 0)) {
			const float x = dist[i] / (dist[i] - dist[j]);
			out[num_in++] = {
				mmlLerp(t[i].v, t[j].v, x),
				mmlLerp(t[i].t, t[j].t, x),
				mmlLerp(t[i].c, t[j].c, x)
			};
		}
		if (side[i] >= 0) {
			out[num_in++] = { t[i].v, t[i].t, t[i].c };
		}
	}

	return num_in;
}

mmlVector<3> retro3d::Plane::GetClosestPoint(const mmlVector<3> &pt) const
{
	return pt - m_normal * GetDistance(pt);
}

const mmlVector<3> &retro3d::Plane::GetPosition( void ) const
{
	return m_position;
}

const mmlVector<3> &retro3d::Plane::GetNormal( void ) const
{
	return m_normal;
}

float retro3d::Plane::GetDistance( void ) const
{
	return -mmlDot(m_position, m_normal);
}

mmlVector<3> retro3d::Plane::ProjectPoint(const mmlVector<3> &pt) const
{
	return pt - mmlDot(pt - m_position, m_normal) * m_normal;
}

mmlVector<2> retro3d::Plane::ProjectPointTo2D(const mmlVector<3> &pt, const mmlVector<3> &x_normal) const
{
	// NOTE, ERROR: m_normal = x_normal will fail

	mmlVector<3> projected_pt = ProjectPoint(pt);
	mmlVector<3> y_normal = mmlCross(x_normal, m_normal);
	return mmlVector<2>(
		double(mmlDot(projected_pt, x_normal)),
		double(mmlDot(projected_pt, y_normal))
	);
}

retro3d::Plane retro3d::Plane::ApplyTransform(const retro3d::Transform &transform) const
{
	return Plane(transform.TransformPoint(m_position), transform.TransformNormal(m_normal));
}

retro3d::Frustum::Frustum( void ) : m_planes(), m_verts(), m_origin(mmlVector<3>::Fill(0.0f))
{}

retro3d::Frustum::Frustum(const mmlVector<3> &view_point, const retro3d::Array< mmlVector<3> > &port_verts, float zfar) : Frustum()
{
	SetFrustum(view_point, port_verts, zfar);
}

retro3d::Frustum::Frustum(const mmlVector<3> &view_point, const mmlVector<3> *port_verts, int32_t port_vert_count, float zfar) : Frustum()
{
	SetFrustum(view_point, port_verts, port_vert_count, zfar);
}

void retro3d::Frustum::SetFrustum(const mmlVector<3> &view_point, const retro3d::Array< mmlVector<3> > &port_verts, float zfar)
{
	SetFrustum(view_point, &port_verts[0], port_verts.GetSize(), zfar);
}

void retro3d::Frustum::SetFrustum(const mmlVector<3> &view_point, const mmlVector<3> *port_verts, int port_vert_count, float zfar)
{
	if (port_vert_count < 3) { return; }

	mmlVector<3> portal_normal = mmlSurfaceNormal(port_verts[0], port_verts[1], port_verts[2]);
	mmlVector<3> portal_center = mmlVector<3>::Fill(0.0f);
	for (int i = 0; i < port_vert_count; ++i) {
		portal_center += port_verts[i];
	}
	portal_center /= float(port_vert_count);

	m_origin = view_point;
	m_zfar = mmlAbs(zfar);
	if (port_verts != &m_verts[0]) { // NOTE: This is a hack to allow SetOrigin by feeding in the original vertices back into SetFrustum as port_verts.
		m_verts.Create(port_vert_count * 2);
	}

	const float DOT = mmlDot(portal_normal, retro3d::NormalFromAToB(view_point, portal_center));

	if (DOT > 0.0f) { // NOTE: If view normal and portal normal point in same direction portal normal is correct and we proceed as is.
		if (port_verts != &m_verts[0]) {  // NOTE: This is a hack to allow SetOrigin by feeding in the original vertices back into SetFrustum as port_verts. m_verts is already equal to elements in port_verts.
			for (int i = 0; i < port_vert_count; ++i) {
				m_verts[i] = port_verts[i];
			}
		}
	} else { // NOTE: If vew normal and portal normal point in opposide directions the view point is on the other side of the portal, so the vertices have to be reversed.
		if (port_verts != &m_verts[0]) {
			for (int i = 0; i < port_vert_count; ++i) {
				m_verts[i] = port_verts[port_vert_count - i - 1];
			}
		} else { // NOTE: This is a hack to allow SetOrigin by feeding in the original vertices back into SetFrustum as port_verts. Since the portal is inverted we need to reverse the vertex order in the first half of m_verts.
			for (int i = 0; i < port_vert_count / 2; ++i) {
				mmlSwap(m_verts[i], m_verts[port_vert_count - i - 1]);
			}
		}
		portal_normal = -portal_normal;
	}

	m_planes.Create(2 + port_vert_count);
	m_planes[Plane_Near] = retro3d::Plane(portal_center, portal_normal);
	m_planes[Plane_Far] = retro3d::Plane(m_planes[Plane_Near].GetPosition() + m_planes[Plane_Near].GetNormal() * m_zfar, -m_planes[Plane_Near].GetNormal());

	for (int i = 0; i < port_vert_count; ++i) {


		mglRayCollisionPoint3D out;

		if (mglCollision::Ray_Plane(m_origin, retro3d::NormalFromAToB(m_origin, m_verts[i]), m_planes[Plane_Far].GetNormal(), m_planes[Plane_Far].GetDistance(), &out) == true) {
			m_verts[i+port_vert_count] = out.point;
		} else {
			// NOTE: This should never happen, so we revert to a way of calculating far points by projecting near points the input z_far distance (which is not correct, but does not require a plane intersection).
			m_verts[i+port_vert_count] = m_verts[i] + retro3d::NormalFromAToB(m_origin, m_verts[i]) * m_zfar;
		}
	}

	for (int p = Plane_FixedCount, i = 0, j = port_vert_count - 1; p < m_planes.GetSize(); ++p, j=i, ++i) {
		m_planes[p] = retro3d::Plane(m_verts[i + port_vert_count], m_verts[i], m_verts[j]);
	}
}

void retro3d::Frustum::SetOrigin(const mmlVector<3> &view_point)
{
	SetFrustum(view_point, &m_verts[0], m_verts.GetSize() / 2, m_zfar);
}

retro3d::Contain retro3d::Frustum::Contains(const mmlVector<3> &pt, bool disregard_near, bool disregard_far) const
{
	if (disregard_near == false && m_planes[Plane_Near].GetDistance(pt) < 0.0f) { return retro3d::Contain_False; }
	if (disregard_far == false && m_planes[Plane_Far].GetDistance(pt) < 0.0f)   { return retro3d::Contain_False; }
	for (int i = Plane_FixedCount; i < m_planes.GetSize(); ++i) {
		if (m_planes[i].GetDistance(pt) < 0.0f) { return retro3d::Contain_False; }
	}
	return retro3d::Contain_Full;
}

retro3d::Contain retro3d::Frustum::Contains(const retro3d::AABB &aabb, bool disregard_near, bool disregard_far) const
{
	mmlVector<3> v[8];
	aabb.GetCorners(v);

	retro3d::Contain c = retro3d::Contain_Full;
	for (int p = 0; p < m_planes.GetSize(); ++p) {
		if (p == int(Plane_Near) && disregard_near == true)    { continue; }
		else if (p == int(Plane_Far) && disregard_far == true) { continue; }
		int side = 0;
		for (int i = 0; i < 8; ++i) {
			if (m_planes[p].GetDistance(v[i]) < 0.0f) {
				--side;
			} else {
				++side;
			}
		}
		if (side == -8) { return retro3d::Contain_False; }
		if (side !=  8) { c = retro3d::Contain_Partial; }
	}

	return c;
}

mmlVector<3> retro3d::Frustum::GetViewDirection( void ) const
{
	mmlVector<3> v = mmlVector<3>::Fill(0.0f);
	for (int i = 0; i < m_verts.GetSize(); ++i) {
		v += m_verts[i];
	}
	v /= float(m_verts.GetSize());
	return retro3d::NormalFromAToB(m_origin, v);
}

mmlVector<3> retro3d::Frustum::GetPortalDirection( void ) const
{
	return m_planes.GetSize() > 0 ? m_planes[Plane_Near].GetNormal() : mmlVector<3>::Fill(0.0f);
}

mmlVector<3> retro3d::Frustum::GetOrigin( void ) const
{
	return m_origin;
}

uint32_t retro3d::Frustum::GetPlaneCount( void ) const
{
	return uint32_t(m_planes.GetSize());
}

uint32_t retro3d::Frustum::GetPortalVertexCount( void ) const
{
	return uint32_t(m_verts.GetSize() / 2);
}

uint32_t retro3d::Frustum::GetFrustumVertexCount( void ) const
{
	return uint32_t(m_verts.GetSize());
}

mmlVector<3> retro3d::Frustum::GetPortalVertex(uint32_t i) const
{
	RETRO3D_ASSERT(i < GetPortalVertexCount());
	return m_verts[i];
}

retro3d::Frustum retro3d::Frustum::ApplyTransform(const retro3d::Transform &transform) const
{
	retro3d::Frustum out = *this;
	for (int i = 0; i < out.m_verts.GetSize(); ++i) {
		out.m_verts[i] *= transform;
	}
	for (int i = 0; i < out.m_planes.GetSize(); ++i) {
		out.m_planes[i] = out.m_planes[i].ApplyTransform(transform);
	}
	out.m_origin *= transform;

	return out;
}

void retro3d::Frustum::GetCorners(mmlVector<3> *out) const
{
	for (int i = 0; i < m_verts.GetSize(); ++i) {
		out[i] = m_verts[i];
	}
}

void retro3d::Frustum::GetCorners(retro3d::Array< mmlVector<3> > &out) const
{
	out.Create(int(GetFrustumVertexCount()));
	GetCorners(&out[0]);
}

retro3d::Plane retro3d::Frustum::GetPortalPlane( void ) const
{
	return m_planes[Plane_Near];
}
