#include <queue>
#include "retro_model.h"
#include "../serial/retro_import.h"
#include "../common/retro_assert.h"
#include "../physics/retro_collider.h"

#include <string>

using namespace retro3d;

void retro3d::Convert(const int32_t src, retro3d::IndexV &dst)
{
	dst.v = src;
}

void retro3d::Convert(const retro3d::IndexVTN &src, retro3d::IndexV &dst)
{
	dst.v = src.v;
}

void retro3d::Convert(const retro3d::IndexVTN &src, retro3d::IndexVN &dst)
{
	dst.v = src.v;
	dst.n = src.n;
}

void retro3d::Convert(const retro3d::IndexV &src, retro3d::IndexVTN &dst)
{
	dst.v = src.v;
	dst.t = -1;
	dst.n = -1;
}

void retro3d::Convert(const retro3d::IndexV &src, retro3d::IndexVN &dst)
{
	dst.v = src.v;
	dst.n = -1;
}

void retro3d::Convert(const retro3d::IndexVN &src, retro3d::IndexVTN &dst)
{
	dst.v = src.v;
	dst.t = -1;
	dst.n = src.n;
}

void retro3d::Convert(const retro3d::IndexV &src, int32_t &dst)
{
	dst = src.v;
}

void retro3d::Convert(const retro3d::IndexVN &src, retro3d::IndexV &dst)
{
	dst.v = src.v;
}

void retro3d::Convert(const retro3d::Array< int32_t > &src, retro3d::FaceIndexV &dst)
{
	dst.Create(src.GetSize());
	for (int32_t i = 0; i < dst.GetSize(); ++i) {
		Convert(src[i], dst[i]);
	}
}

void retro3d::Convert(const retro3d::FaceIndex &src, retro3d::FaceIndexV &dst)
{
	dst.Create(src.GetSize());
	for (int32_t i = 0; i < dst.GetSize(); ++i) {
		Convert(src[i], dst[i]);
	}
}

void retro3d::Convert(const retro3d::FaceIndex &src, retro3d::FaceIndexVN &dst)
{
	dst.Create(src.GetSize());
	for (int32_t i = 0; i < dst.GetSize(); ++i) {
		Convert(src[i], dst[i]);
	}
}

void retro3d::Convert(const retro3d::FaceIndexV &src, retro3d::Array< int32_t > &dst)
{
	dst.Create(src.GetSize());
	for (int32_t i = 0; i < dst.GetSize(); ++i) {
		Convert(src[i], dst[i]);
	}
}

void retro3d::Convert(const retro3d::FaceIndexV &src, retro3d::FaceIndex &dst)
{
	dst.Create(src.GetSize());
	for (int32_t i = 0; i < dst.GetSize(); ++i) {
		Convert(src[i], dst[i]);
	}
}

void retro3d::Convert(const retro3d::FaceIndexV &src, retro3d::FaceIndexVN &dst)
{
	dst.Create(src.GetSize());
	for (int32_t i = 0; i < dst.GetSize(); ++i) {
		Convert(src[i], dst[i]);
	}
}

void retro3d::Convert(const retro3d::FaceIndexVN &src, retro3d::FaceIndex &dst)
{
	dst.Create(src.GetSize());
	for (int32_t i = 0; i < dst.GetSize(); ++i) {
		Convert(src[i], dst[i]);
	}
}

void retro3d::Convert(const retro3d::FaceIndexVN &src, retro3d::FaceIndexV &dst)
{
	dst.Create(src.GetSize());
	for (int32_t i = 0; i < dst.GetSize(); ++i) {
		Convert(src[i], dst[i]);
	}
}

void retro3d::Convert(const retro3d::Array< retro3d::Array< int32_t > > &src, retro3d::Array< retro3d::FaceIndexV > &dst)
{
	dst.Create(src.GetSize());
	for (int32_t i = 0; i < dst.GetSize(); ++i) {
		Convert(src[i], dst[i]);
	}
}

void retro3d::Convert(const retro3d::Array< retro3d::FaceIndex > &src, retro3d::Array< retro3d::FaceIndexV > &dst)
{
	dst.Create(src.GetSize());
	for (int32_t i = 0; i < dst.GetSize(); ++i) {
		Convert(src[i], dst[i]);
	}
}

void retro3d::Convert(const retro3d::Array< retro3d::FaceIndex > &src, retro3d::Array< retro3d::FaceIndexVN > &dst)
{
	dst.Create(src.GetSize());
	for (int32_t i = 0; i < dst.GetSize(); ++i) {
		Convert(src[i], dst[i]);
	}
}

void retro3d::Convert(const retro3d::Array< retro3d::FaceIndexV > &src, retro3d::Array< retro3d::Array< int32_t > > &dst)
{
	dst.Create(src.GetSize());
	for (int32_t i = 0; i < dst.GetSize(); ++i) {
		Convert(src[i], dst[i]);
	}
}

void retro3d::Convert(const retro3d::Array< retro3d::FaceIndexV > &src, retro3d::Array< retro3d::FaceIndex > &dst)
{
	dst.Create(src.GetSize());
	for (int32_t i = 0; i < dst.GetSize(); ++i) {
		Convert(src[i], dst[i]);
	}
}

void retro3d::Convert(const retro3d::Array< retro3d::FaceIndexV > &src, retro3d::Array< retro3d::FaceIndexVN > &dst)
{
	dst.Create(src.GetSize());
	for (int32_t i = 0; i < dst.GetSize(); ++i) {
		Convert(src[i], dst[i]);
	}
}

void retro3d::Convert(const retro3d::Array< retro3d::FaceIndexVN > &src, retro3d::Array< retro3d::FaceIndex > &dst)
{
	dst.Create(src.GetSize());
	for (int32_t i = 0; i < dst.GetSize(); ++i) {
		Convert(src[i], dst[i]);
	}
}

void retro3d::Convert(const retro3d::Array< retro3d::FaceIndexVN > &src, retro3d::Array< retro3d::FaceIndexV > &dst)
{
	dst.Create(src.GetSize());
	for (int32_t i = 0; i < dst.GetSize(); ++i) {
		Convert(src[i], dst[i]);
	}
}

void retro3d::Model::CenterModel( void )
{
	mmlVector<3> center = aabb.GetCenter();
	for (int32_t i = 0; i < v.GetSize(); ++i) {
		v[i] -= center;
	}
	aabb.SetCenter(mmlVector<3>::Fill(0.0f));
}

void retro3d::Model::ScaleModelToUnitSize( void )
{
	const float c = (aabb.GetHalfExtents() * 2.0f).Len();
	const float scale = 1.0f / c;
	for (int32_t i = 0; i < v.GetSize(); ++i) {
		v[i] *= scale;
	}
	aabb.SetCenter(aabb.GetCenter() * scale);
	aabb.SetHalfExtents(aabb.GetHalfExtents() * scale);
}

void retro3d::Model::ReverseModel( void )
{
	for (int32_t i = 0; i < m.GetSize(); ++i) {
		InvertFaces(m[i].f);
	}
	InvertNormals(n);
	InvertFaces(f);
}

void retro3d::Model::DoubleSideModel( void )
{
	const int32_t n_size = n.GetSize();
	n.Resize(n_size * 2);
	for (int32_t i = 0; i < n_size; ++i) {
		n[i + n_size] = -n[i];
	}

	for (int32_t i = 0; i < m.GetSize(); ++i) {
		retro3d::Material *mat = &m[i];
		const int32_t f_size = mat->f.GetSize();
		mat->f.Resize(f_size * 2);
		for (int32_t j = 0; j < f_size; ++j) {
			const retro3d::FaceIndex *fa = &mat->f[j];
			retro3d::FaceIndex       *fb = &mat->f[j + f_size];
			*fb = *fa;
			const int32_t v_size = fb->GetSize();
			for (int32_t k = 0; k < v_size; ++k) {
				(*fb)[k].n += n_size;
			}
			for (int32_t k = 0; k < v_size / 2; ++k) {
				mmlSwap((*fb)[k], (*fb)[v_size - k - 1]);
			}
		}
	}
}

void retro3d::Model::GenerateVertexNormals( void )
{
	n.Create(v.GetSize());
	for (int i = 0; i < n.GetSize(); ++i) { n[i] = mmlVector<3>::Fill(0.0f); }
	for (int i = 0; i < m.GetSize(); ++i) {
		auto &material = m[i];
		for (int j = 0; j < material.f.GetSize(); ++j) {
			auto &face = material.f[j];
			if (face.GetSize() < 3) { continue; }
			const mmlVector<3> A = v[ face[0].v ];
			mmlVector<3> N = mmlVector<3>::Fill(0.0f);
			for (int k = 1; k < face.GetSize() - 1; ++k) { // check the cross product of all sub-triangles in polygon to make sure there is no sub-triangle small enough to generate NaNs
				const mmlVector<3> B = v[ face[k    ].v ];
				const mmlVector<3> C = v[ face[k + 1].v ];
				mmlVector<3> cross = mmlCross(B - A, C - A);
				if (!std::isnan(cross[0]) && !std::isnan(cross[1]) && !std::isnan(cross[2])) { // skip NaN normal
					N += cross;
				}
			}
			if (mmlIsApproxEqual(N.Len(), 0.0f) == true) { // sub-triangles are too small to determine normal
				N[0] = N[1] = N[2] = 0.0f;
			} else {
				N.Normalize();
			}

			for (int k = 0; k < face.GetSize(); ++k) {
				auto &index = face[k];
				index.n = index.v;
				n[index.n] += N;
			}
		}
	}
	for (int i = 0; i < n.GetSize(); ++i) { n[i].Normalize(); }
}

void retro3d::Model::GenerateFaceNormals( void )
{
	int32_t n_count = 0;
	for (int i = 0; i < m.GetSize(); ++i) { n_count += m[i].f.GetSize(); }
	n.Create(n_count);
	for (int i = 0; i < n.GetSize(); ++i) { n[i] = mmlVector<3>::Fill(0.0f); }

	n_count = 0;
	for (int i = 0; i < m.GetSize(); ++i) {
		auto &material = m[i];
		for (int j = 0; j < material.f.GetSize(); ++j, ++n_count) {
			auto &face = material.f[j];

			if (face.GetSize() < 3) { continue; }
			const mmlVector<3> A = v[ face[0].v ];
			n[n_count] = mmlVector<3>::Fill(0.0f);
			for (int k = 1; k < face.GetSize() - 1; ++k) { // check the cross product of all sub-triangles in polygon to make sure there is no sub-triangle small enough to generate NaNs
				const mmlVector<3> B = v[ face[k    ].v ];
				const mmlVector<3> C = v[ face[k + 1].v ];
				mmlVector<3> cross = mmlCross(B - A, C - A);
				if (!std::isnan(cross[0]) && !std::isnan(cross[1]) && !std::isnan(cross[2])) { // skip NaN normal
					n[n_count] += cross;
				}
			}
			if (mmlIsApproxEqual(n[n_count].Len(), 0.0f) == true) { // sub-triangles are too small to determine normal
				n[n_count][0] = n[n_count][1] = n[n_count][2] = 0.0f;
			} else {
				n[n_count].Normalize();
			}

			for (int k = 0; k < face.GetSize(); ++k) {
				face[k].n = n_count;
			}
		}
	}
}

uint32_t retro3d::Model::GetPolyCount( void ) const
{
	uint32_t polys = 0;
	for (int i = 0; i < m.GetSize(); ++i) {
		polys += uint32_t(m[i].f.GetSize());
	}
	return polys;
}

uint32_t retro3d::Model::GetTriCount( void ) const
{
	uint32_t tris = 0;
	for (int i = 0; i < m.GetSize(); ++i) {
		const retro3d::Material *material = &m[i];
		for (int f = 0; f < material->f.GetSize(); ++f) {
			const retro3d::FaceIndex *face = &material->f[f];
			tris += uint32_t(mmlMax(0, (face->GetSize() - 2)));
		}
	}
	return tris;
}

float retro3d::Model::CalculateSurfaceArea( void ) const
{
	float area = 0.0f;
	for (int i = 0; i < m.GetSize(); ++i) {
		for (int f = 0; f < m[i].f.GetSize(); ++f) {
			for (int j = 1; j < m[i].f[f].GetSize() - 1; ++j) {
				int a = m[i].f[f][0].v;
				int b = m[i].f[f][j].v;
				int c = m[i].f[f][j+1].v;
				area += retro3d::TriangleArea3D(v[a], v[b], v[c]);
			}
		}
	}
	return area;
}

void retro3d::Model::ApplyTransform(const retro3d::Transform &transform)
{
	const mmlMatrix<3,3> N = mmlBasis(transform);
	for (int i = 0; i < v.GetSize(); ++i) {
		v[i] *= transform;
	}
	for (int i = 0; i < n.GetSize(); ++i) {
		n[i] *= N;
		n[i].Normalize();
	}
	aabb.FromGeometry(v);
}

void retro3d::Model::AppendModel(const retro3d::Model &model, const retro3d::Transform &transform)
{
	const int V_SIZE = v.GetSize();
	const int T_SIZE = t.GetSize();
	const int N_SIZE = n.GetSize();
	v.SetCapacity(v.GetSize() + model.v.GetSize());
	t.SetCapacity(t.GetSize() + model.t.GetSize());
	n.SetCapacity(n.GetSize() + model.n.GetSize());
	m.SetCapacity(m.GetSize() + model.m.GetSize());
	const mmlMatrix<3,3> N = mmlBasis(transform);
	for (int i = 0; i < model.v.GetSize(); ++i) { v.Add(model.v[i] * transform); }
	for (int i = 0; i < model.t.GetSize(); ++i) { t.Add(model.t[i]); }
	for (int i = 0; i < model.n.GetSize(); ++i) {
		n.Add(model.n[i] * N);
		n[n.GetSize() - 1].Normalize();
	}
	for (int i = 0; i < model.m.GetSize(); ++i) {
		m.Add(model.m[i]);
		retro3d::Material &mat = m[m.GetSize() - 1];
		for (int j = 0; j < mat.f.GetSize(); ++j) {
			for (int k = 0; k < mat.f[j].GetSize(); ++k) {
				if (mat.f[j][k].v >= 0) { mat.f[j][k].v += V_SIZE; }
				if (mat.f[j][k].t >= 0) { mat.f[j][k].t += T_SIZE; }
				if (mat.f[j][k].n >= 0) { mat.f[j][k].n += N_SIZE; }
			}
		}
	}

	aabb = aabb | model.aabb.ApplyTransform(transform);
}

void retro3d::Model::RefreshConnectivity( void )
{
	int face_count = 0;
	for (int i = 0; i < m.GetSize(); ++i) {
		face_count += m[i].f.GetSize();
	}
	f.Create(face_count);
	int abs_face_index = 0;
	for (int i = 0; i < m.GetSize(); ++i) {
		for (int j = 0; j < m[i].f.GetSize(); ++j) {
			f[abs_face_index] = m[i].f[j];
			++abs_face_index;
		}
	}
}

void retro3d::Model::DefragIndex( void )
{
	// NOTE: Doing this in a roundabout way since unordered_map::iterator::second returns a COPY of the element, not the element itself.
	struct Entry
	{
		mtlShared< mtlList<int32_t*> > l;
		Entry( void ) { l.New(); }
	};

	std::unordered_map< int32_t, Entry > hv;
	std::unordered_map< int32_t, Entry > ht;
	std::unordered_map< int32_t, Entry > hn;

	// build the hash tables...
	for (int mi = 0; mi < m.GetSize(); ++mi) {
		auto *mat = &m[mi];
		for (int fi = 0; fi < mat->f.GetSize(); ++fi) {
			auto *face = &mat->f[fi];
			for (int vi = 0; vi < face->GetSize(); ++vi) {
				auto *i = &(*face)[vi];
				if (i->v >= 0 && i->v < v.GetSize()) { hv[i->v].l->AddLast(&i->v); }
				if (i->t >= 0 && i->t < t.GetSize()) { ht[i->t].l->AddLast(&i->t); }
				if (i->n >= 0 && i->n < n.GetSize()) { hn[i->n].l->AddLast(&i->n); }
			}
		}
	}

	retro3d::Array< mmlVector<3> > v_packed;
	retro3d::Array< mmlVector<2> > t_packed;
	retro3d::Array< mmlVector<3> > n_packed;
	v_packed.Create(int(hv.size()));
	t_packed.Create(int(ht.size()));
	n_packed.Create(int(hn.size()));

	int32_t i = 0;
	for (auto j = hv.begin(); j != hv.end(); ++j) {
		v_packed[i] = v[j->first];
		for (auto k = j->second.l->GetFirst(); k != nullptr; k = k->GetNext()) {
			*k->GetItem() = i;
		}
		++i;
	}

	i = 0;
	for (auto j = ht.begin(); j != ht.end(); ++j) {
		t_packed[i] = t[j->first];
		for (auto k = j->second.l->GetFirst(); k != nullptr; k = k->GetNext()) {
			*k->GetItem() = i;
		}
		++i;
	}

	i = 0;
	for (auto j = hn.begin(); j != hn.end(); ++j) {
		n_packed[i] = n[j->first];
		for (auto k = j->second.l->GetFirst(); k != nullptr; k = k->GetNext()) {
			*k->GetItem() = i;
		}
		++i;
	}

#ifdef RETRO3D_DEBUG
	std::cout << "Defragmented indices: (v:" << v.GetSize() << ",t:" << t.GetSize() << ",n:" << n.GetSize() << ") ->";
#endif

	v = v_packed;
	t = t_packed;
	n = n_packed;

#ifdef RETRO3D_DEBUG
	std::cout << "(v:" << v.GetSize() << ",t:" << t.GetSize() << ",n:" << n.GetSize() << "), ";
#endif
}

void retro3d::TransformVertices(retro3d::Array< mmlVector<3> > &v, const retro3d::Transform &t)
{
	const mmlMatrix<4,4> m = t.GetFinalMatrix();
	for (int i = 0; i < v.GetSize(); ++i) {
		v[i] *= m;
	}
}

void retro3d::TranslateVertices(retro3d::Array< mmlVector<3> > &v, const mmlVector<3> &t)
{
	for (int i = 0; i < v.GetSize(); ++i) {
		v[i] += t;
	}
}

void retro3d::TranslateVertices(retro3d::Array< mmlVector<3> > &v, int32_t axis, float t)
{
	for (int i = 0; i < v.GetSize(); ++i) {
		v[i][axis] += t;
	}
}

void retro3d::CenterVerticesByMass(retro3d::Array< mmlVector<3> > &v, const mmlVector<3> &new_center)
{
	retro3d::TranslateVertices(v, retro3d::VectorFromAToB(retro3d::CalculateCenterOfMass(v), new_center));
}

void retro3d::CenterVerticesByVolume(retro3d::Array< mmlVector<3> > &v, const mmlVector<3> &new_center)
{
	retro3d::TranslateVertices(v, retro3d::VectorFromAToB(retro3d::CalculateCenterOfVolume(v), new_center));
}

float retro3d::CalculateVertexScale(const retro3d::Array< mmlVector<3> > &v)
{
	return mmlMax(retro3d::AABB(v).GetHalfExtents() * 2.0f);
}

void retro3d::ScaleVertices(retro3d::Array< mmlVector<3> > &v, float scale)
{
	for (int i = 0; i < v.GetSize(); ++i) {
		v[i] *= scale;
	}
}

mmlVector<3> retro3d::CalculateCentroid(const retro3d::Array< mmlVector<3> > &v)
{
	mmlVector<3> c = mmlVector<3>::Fill(0.0f);
	for (int i = 0; i < v.GetSize(); ++i) {
		c += v[i];
	}
	c /= float(v.GetSize());
	return c;
}

mmlVector<3> retro3d::CalculateCenterOfMass(const retro3d::Array< mmlVector<3> > &v)
{
	return CalculateCentroid(v);
}

mmlVector<3> retro3d::CalculateCenterOfVolume(const retro3d::Array< mmlVector<3> > &v)
{
	return retro3d::AABB(v).GetCenter();
}

mmlVector<3> retro3d::FindExtremeVertexAlongDirection(const retro3d::Array< mmlVector<3> > &v, const mmlVector<3> &search_dir)
{
	if (v.GetSize() <= 0) { return mmlVector<3>::Fill(0); }

	mmlVector<3> max_v   = v[0];
	float        max_dot = mmlDot(max_v, search_dir);

	for (int32_t i = 1; i < v.GetSize(); ++i) {
		const float dot = mmlDot(v[i], search_dir);
		if (dot > max_dot) {
			max_dot = dot;
			max_v = v[i];
		}
	}

	return max_v;
}

int32_t retro3d::FindExtremeVertexIndexAlongDirection(const retro3d::Array< mmlVector<3> > &v, const mmlVector<3> &search_dir)
{
	if (v.GetSize() <= 0) { return -1; }

	int32_t max_i   = 0;
	float   max_dot = mmlDot(v[0], search_dir);

	for (int32_t i = 1; i < v.GetSize(); ++i) {
		const float dot = mmlDot(v[i], search_dir);
		if (dot > max_dot) {
			max_dot = dot;
			max_i = i;
		}
	}

	return max_i;
}

bool retro3d::IsPointInConvexHull(const retro3d::Array< mmlVector<3> > &hull_v, const mmlVector<3> &p)
{
	const mmlVector<3> c = retro3d::CalculateCentroid(hull_v); // NOTE: This is guaranteed to be inside the hull (if the user has provided a correct convex hull that is).
	const mmlVector<3> d = retro3d::NormalFromAToB(c, p);
	const mmlVector<3> m = retro3d::FindExtremeVertexAlongDirection(hull_v, d);
	return (c - p).Len2() < (c - m).Len2();
}

namespace impl
{
	template < typename index_t >
	void ExtractFaceVertices(const retro3d::Array< mmlVector<3> > &v, const index_t &f, retro3d::Array< mmlVector<3> > &out)
	{
		out.Create(f.GetSize());
		for (int i = 0; i < out.GetSize(); ++i) {
			if (f[i].v >= 0 && f[i].v < v.GetSize()) {
				out[i] = v[f[i].v];
			} else {
				out[i] = mmlVector<3>::Fill(0.0f);
			}
		}
	}
}

void retro3d::ExtractFaceVertices(const retro3d::Array< mmlVector<3> > &v, const retro3d::FaceIndex &f, retro3d::Array< mmlVector<3> > &out)
{
	impl::ExtractFaceVertices(v, f, out);
}

void retro3d::ExtractFaceVertices(const retro3d::Array< mmlVector<3> > &v, const retro3d::FaceIndexV &f, retro3d::Array< mmlVector<3> > &out)
{
	impl::ExtractFaceVertices(v, f, out);
}

void retro3d::ExtractFaceVertices(const retro3d::Array< mmlVector<3> > &v, const retro3d::FaceIndexVN &f, retro3d::Array< mmlVector<3> > &out)
{
	impl::ExtractFaceVertices(v, f, out);
}

namespace impl
{
	template < typename index_t >
	void InsertFaceVertices(retro3d::Array< mmlVector<3> > &v, const index_t &f, const retro3d::Array< mmlVector<3> > &in)
	{
		RETRO3D_ASSERT(f.GetSize() == in.GetSize());
		for (int i = 0; i < in.GetSize(); ++i) {
			if (f[i].v >= 0 && f[i].v < v.GetSize()) {
				v[f[i].v] = in[i];
			}
		}
	}
}

void retro3d::InsertFaceVertices(retro3d::Array< mmlVector<3> > &v, const retro3d::FaceIndex &f, const retro3d::Array< mmlVector<3> > &in)
{
	impl::InsertFaceVertices(v, f, in);
}

void retro3d::InsertFaceVertices(retro3d::Array< mmlVector<3> > &v, const retro3d::FaceIndexV &f, const retro3d::Array< mmlVector<3> > &in)
{
	impl::InsertFaceVertices(v, f, in);
}

void retro3d::InsertFaceVertices(retro3d::Array< mmlVector<3> > &v, const retro3d::FaceIndexVN &f, const retro3d::Array< mmlVector<3> > &in)
{
	impl::InsertFaceVertices(v, f, in);
}

namespace impl
{
	template < typename index_t >
	void InvertFaces(retro3d::Array< index_t > &f) {
		for (int i = 0; i < f.GetSize(); ++i) {
			for (int j = 0; j < f[i].GetSize() / 2; ++j) {
				mmlSwap(f[i][j], f[i][f[i].GetSize() - j - 1]);
			}
		}
	}
}

void retro3d::InvertFaces(retro3d::Array<FaceIndex> &f)
{
	impl::InvertFaces(f);
}

void retro3d::InvertFaces(retro3d::Array<FaceIndexVN> &f)
{
	impl::InvertFaces(f);
}

void retro3d::InvertFaces(retro3d::Array<FaceIndexV> &f)
{
	impl::InvertFaces(f);
}

void retro3d::InvertNormals(retro3d::Array< mmlVector<3> > &n)
{
	for (int i = 0; i < n.GetSize(); ++i) {
		n[i] = -n[i];
	}
}

void retro3d::CalculateVertexNormals(const retro3d::Array< mmlVector<3> > &v, retro3d::Array< retro3d::Material > &inout_materials, retro3d::Array< mmlVector<3> > &out_normals)
{
	out_normals.Create(v.GetSize());
	for (int i = 0; i < out_normals.GetSize(); ++i) { out_normals[i] = mmlVector<3>::Fill(0.0f); }
	for (int i = 0; i < inout_materials.GetSize(); ++i) {
		auto &material = inout_materials[i];
		for (int j = 0; j < material.f.GetSize(); ++j) {
			auto &face = material.f[j];
			if (face.GetSize() < 3) { continue; }
			const mmlVector<3> A = v[ face[0].v ];
			mmlVector<3> N = mmlVector<3>::Fill(0.0f);
			for (int k = 1; k < face.GetSize() - 1; ++k) { // check the cross product of all sub-triangles in polygon to make sure there is no sub-triangle small enough to generate NaNs
				const mmlVector<3> B = v[ face[k    ].v ];
				const mmlVector<3> C = v[ face[k + 1].v ];
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
				out_normals[index.n] += N;
			}
		}
	}
	for (int i = 0; i < out_normals.GetSize(); ++i) { out_normals[i].Normalize(); }
}

void retro3d::CalculateVertexNormals(const retro3d::Array< mmlVector<3> > &v, retro3d::Material &inout_material, retro3d::Array< mmlVector<3> > &out_normals)
{
	out_normals.Create(v.GetSize());
	for (int i = 0; i < out_normals.GetSize(); ++i) { out_normals[i] = mmlVector<3>::Fill(0.0f); }
	auto &material = inout_material;
	for (int j = 0; j < material.f.GetSize(); ++j) {
		auto &face = material.f[j];
		if (face.GetSize() < 3) { continue; }
		const mmlVector<3> A = v[ face[0].v ];
		mmlVector<3> N = mmlVector<3>::Fill(0.0f);
		for (int k = 1; k < face.GetSize() - 1; ++k) { // check the cross product of all sub-triangles in polygon to make sure there is no sub-triangle small enough to generate NaNs
			const mmlVector<3> B = v[ face[k    ].v ];
			const mmlVector<3> C = v[ face[k + 1].v ];
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
			out_normals[index.n] += N;
		}
	}
	for (int i = 0; i < out_normals.GetSize(); ++i) { out_normals[i].Normalize(); }
}

void retro3d::CalculateFaceNormals(const retro3d::Array< mmlVector<3> > &v, retro3d::Array< retro3d::Material > &inout_material, retro3d::Array< mmlVector<3> > &out_normals)
{
	int32_t n_count = 0;
	for (int i = 0; i < inout_material.GetSize(); ++i) { n_count += inout_material[i].f.GetSize(); }
	out_normals.Create(n_count);
	for (int i = 0; i < out_normals.GetSize(); ++i) { out_normals[i] = mmlVector<3>::Fill(0.0f); }

	n_count = 0;
	for (int i = 0; i < inout_material.GetSize(); ++i) {
		auto &material = inout_material[i];
		for (int j = 0; j < material.f.GetSize(); ++j, ++n_count) {
			auto &face = material.f[j];

			if (face.GetSize() < 3) { continue; }
			const mmlVector<3> A = v[ face[0].v ];
			out_normals[n_count] = mmlVector<3>::Fill(0.0f);
			for (int k = 1; k < face.GetSize() - 1; ++k) { // check the cross product of all sub-triangles in polygon to make sure there is no sub-triangle small enough to generate NaNs
				const mmlVector<3> B = v[ face[k    ].v ];
				const mmlVector<3> C = v[ face[k + 1].v ];
				const mmlVector<3> cross = mmlCross(B - A, C - A);
				if (!std::isnan(cross[0]) && !std::isnan(cross[1]) && !std::isnan(cross[2])) { // skip NaN normal
					out_normals[n_count] += cross;
				}
			}
			if (mmlIsApproxEqual(out_normals[n_count].Len(), 0.0f) == true) {
				out_normals[n_count][0] = out_normals[n_count][1] = out_normals[n_count][2] = 0.0f;
			} else {
				out_normals[n_count].Normalize();
			}

			for (int k = 0; k < face.GetSize(); ++k) {
				face[k].n = n_count;
			}
		}
	}
}

void retro3d::CalculateFaceNormals(const retro3d::Array< mmlVector<3> > &v, retro3d::Material &inout_material, retro3d::Array< mmlVector<3> > &out_normals)
{
	int32_t n_count = inout_material.f.GetSize();
	out_normals.Create(n_count);
	for (int i = 0; i < out_normals.GetSize(); ++i) { out_normals[i] = mmlVector<3>::Fill(0.0f); }

	n_count = 0;
	auto &material = inout_material;
	for (int j = 0; j < material.f.GetSize(); ++j, ++n_count) {
		auto &face = material.f[j];

		if (face.GetSize() < 3) { continue; }
		const mmlVector<3> A = v[ face[0].v ];
		out_normals[n_count] = mmlVector<3>::Fill(0.0f);
		for (int k = 1; k < face.GetSize() - 1; ++k) { // check the cross product of all sub-triangles in polygon to make sure there is no sub-triangle small enough to generate NaNs
			const mmlVector<3> B = v[ face[k    ].v ];
			const mmlVector<3> C = v[ face[k + 1].v ];
			const mmlVector<3> cross = mmlCross(B - A, C - A);
			if (!std::isnan(cross[0]) && !std::isnan(cross[1]) && !std::isnan(cross[2])) { // skip NaN normal
				out_normals[n_count] += cross;
			}
		}
		if (mmlIsApproxEqual(out_normals[n_count].Len(), 0.0f) == true) {
			out_normals[n_count][0] = out_normals[n_count][1] = out_normals[n_count][2] = 0.0f;
		} else {
			out_normals[n_count].Normalize();
		}

		for (int k = 0; k < face.GetSize(); ++k) {
			face[k].n = n_count;
		}
	}
}

namespace impl
{
	template < typename index_t >
	void ExtractFaceNormals(const retro3d::Array< mmlVector<3> > &n, const index_t &f, retro3d::Array< mmlVector<3> > &out)
	{
		out.Create(f.GetSize());
		for (int i = 0; i < out.GetSize(); ++i) {
			if (f[i].n >= 0 && f[i].n < n.GetSize()) {
				out[i] = n[f[i].n];
			} else {
				out[i] = mmlVector<3>::Fill(0.0f);
			}
		}
	}
}

void retro3d::ExtractFaceNormals(const retro3d::Array< mmlVector<3> > &n, const retro3d::FaceIndex &f, retro3d::Array< mmlVector<3> > &out)
{
	impl::ExtractFaceNormals(n, f, out);
}
void retro3d::ExtractFaceNormals(const retro3d::Array< mmlVector<3> > &n, const retro3d::FaceIndexVN &f, retro3d::Array< mmlVector<3> > &out)
{
	impl::ExtractFaceNormals(n, f, out);
}

namespace impl
{
	template < typename index_t >
	void InsertFaceNormals(retro3d::Array< mmlVector<3> > &n, const index_t &f, const retro3d::Array< mmlVector<3> > &in)
	{
		RETRO3D_ASSERT(f.GetSize() == in.GetSize());
		for (int i = 0; i < in.GetSize(); ++i) {
			if (f[i].n >= 0 && f[i].n < n.GetSize()) {
				n[f[i].n] = in[i];
			}
		}
	}
}

void retro3d::InsertFaceNormals(retro3d::Array< mmlVector<3> > &n, const retro3d::FaceIndex &f, const retro3d::Array< mmlVector<3> > &in)
{
	impl::InsertFaceNormals(n, f, in);
}

void retro3d::InsertFaceNormal(retro3d::Array< mmlVector<3> > &n, const retro3d::FaceIndexVN &f, const retro3d::Array< mmlVector<3> > &in)
{
	impl::InsertFaceNormals(n, f, in);
}

void retro3d::ExtractFaceTCoords(const retro3d::Array< mmlVector<2> > &t, const retro3d::FaceIndex &f, retro3d::Array< mmlVector<2> > &out)
{
	out.Create(f.GetSize());
	for (int i = 0; i < out.GetSize(); ++i) {
		if (f[i].t >= 0 && f[i].t < t.GetSize()) {
			out[i] = t[f[i].t];
		} else {
			out[i] = mmlVector<2>::Fill(0.0f);
		}
	}
}

void retro3d::InsertFaceTCoords(retro3d::Array< mmlVector<2> > &t, const retro3d::FaceIndex &f, const retro3d::Array< mmlVector<2> > &in)
{
	RETRO3D_ASSERT(f.GetSize() == in.GetSize());
	for (int i = 0; i < in.GetSize(); ++i) {
		if (f[i].t >= 0 && f[i].t < t.GetSize()) {
			t[f[i].t] = in[i];
		}
	}
}

namespace impl
{
	template < typename index_t >
	bool PointInsideConvexHull(const mmlVector<3> &point, const retro3d::Array< mmlVector<3> > &v, const retro3d::Array< index_t > &convex_hull, float FP_EPSILON)
	{
		retro3d::Plane plane;
		for (int32_t f = 0; f < convex_hull.GetSize(); ++f) {
			if (convex_hull[f].GetSize() >= 3) {
				plane = retro3d::Plane(v[convex_hull[f][0].v], v[convex_hull[f][1].v], v[convex_hull[f][2].v]);
				if (plane.DetermineSide(point, FP_EPSILON) > 0) { return false; }
			}
		}
		return true;
	}
}

bool retro3d::PointInsideConvexHull(const mmlVector<3> &point, const retro3d::Array< mmlVector<3> > &v, const retro3d::Array< retro3d::FaceIndex > &convex_hull, float FP_EPSILON)
{
	return impl::PointInsideConvexHull(point, v, convex_hull, FP_EPSILON);
}

bool retro3d::PointInsideConvexHull(const mmlVector<3> &point, const retro3d::Array< mmlVector<3> > &v, const retro3d::Array< retro3d::FaceIndexV > &convex_hull, float FP_EPSILON)
{
	return impl::PointInsideConvexHull(point, v, convex_hull, FP_EPSILON);
}

bool retro3d::PointInsideConvexHull(const mmlVector<3> &point, const retro3d::Array< mmlVector<3> > &v, const retro3d::Array< retro3d::FaceIndexVN > &convex_hull, float FP_EPSILON)
{
	return impl::PointInsideConvexHull(point, v, convex_hull, FP_EPSILON);
}

namespace impl
{
	template < typename index_t >
	bool IsConvex(const retro3d::Array< mmlVector<3> > &v, const retro3d::Array< index_t > &f, bool invertedGeometry, float FP_EPSILON)
	{
		// NOTE: Assertion = For a space to be convex, all polygons must be BEHIND or ON all planes made out of the polygons.
		for (int32_t f0 = 0; f0 < f.GetSize() - 1; ++f0) {
			const index_t *face0 = &f[f0];
			const retro3d::Plane plane(v[(*face0)[0].v], v[(*face0)[1].v], v[(*face0)[2].v]);
			for (int32_t f1 = f0 + 1; f1 < f.GetSize(); ++f1) {
				const index_t *face1 = &f[f1];
				const int32_t vert_count = face1->GetSize();
				for (int32_t v0 = 0; v0 < vert_count; ++v0) {
					const mmlVector<3> vcoord = v[(*face1)[v0].v];
					if (invertedGeometry == false) {
						if (plane.DetermineSide(vcoord, FP_EPSILON) > 0) { return false; } // NOTE: Current vertex is in front of current plane, i.e. not convex.
					} else {
						if (plane.DetermineSide(vcoord, FP_EPSILON) < 0) { return false; } // NOTE: Current vertex is behind current plane, i.e. not convex.
					}
				}
			}
		}
		return true; // NOTE: Passed all tests, i.e. convex.
	}
}

bool retro3d::IsConvex(const retro3d::Array< mmlVector<3> > &v, const retro3d::Array< retro3d::FaceIndexV > &f, float FP_EPSILON)
{
	return impl::IsConvex(v, f, false, FP_EPSILON);
}

bool retro3d::IsConvex(const retro3d::Array< mmlVector<3> > &v, const retro3d::Array< retro3d::FaceIndexVN > &f, float FP_EPSILON)
{
	return impl::IsConvex(v, f, false, FP_EPSILON);
}

bool retro3d::IsConvex(const retro3d::Array< mmlVector<3> > &v, const retro3d::Array< retro3d::FaceIndex > &f, float FP_EPSILON)
{
	return impl::IsConvex(v, f, false, FP_EPSILON);
}

void retro3d::CreateConvexHull(const retro3d::Array< mmlVector<3> > &vert_cloud, retro3d::Array< mmlVector<3> > *hull_verts, retro3d::Array< retro3d::FaceIndexV > *outer_hull_faces, float FP_EPSILON)
{
	// Faster approach:
	// https://gist.github.com/msg555/4963794

	if ((hull_verts == nullptr && outer_hull_faces == nullptr) || vert_cloud.GetSize() < 4) { return; }

	// Find the center of the volume of the shape
	const retro3d::AABB aabb = retro3d::AABB(vert_cloud);
	const mmlVector<3> center = retro3d::CalculateCenterOfVolume(vert_cloud);
	const mmlVector<3> half_extents = aabb.GetHalfExtents();
	int32_t degeneracy = 0;
	for (int32_t i = 0; i < 3; ++i) {
		if (mmlIsApproxEqual(half_extents[i], 0.0f) == true) {
			++degeneracy;
		}
	}
	if (degeneracy > 1) { return; }
	const retro3d::Array< mmlVector<3> > *search_verts = &vert_cloud;

	// Generate a map of the convex points using the support vector from center to each vertex.
	std::unordered_map< int32_t, mmlVector<3> > convex_points;
	convex_points.reserve(size_t(vert_cloud.GetSize()));
	for (int i = 0; i < vert_cloud.GetSize(); ++i) {
		const int32_t extreme_index = retro3d::FindExtremeVertexIndexAlongDirection(vert_cloud, retro3d::NormalFromAToB(center, vert_cloud[i]));
		convex_points[extreme_index] = vert_cloud[extreme_index];
	}

	// Serialize map to linear vertex.
	if (hull_verts != nullptr) {
		hull_verts->SetCapacity(int(convex_points.size()));
		for (auto i = convex_points.begin(); i != convex_points.end(); ++i) {
			hull_verts->Add(i->second);
		}

		// Remap the vertex output to the map.
		if (outer_hull_faces != nullptr) {
			search_verts = hull_verts;
			convex_points.clear();
			convex_points.reserve(size_t(hull_verts->GetSize()));
			for (int32_t i = 0; i < hull_verts->GetSize(); ++i) {
				convex_points[i] = (*hull_verts)[i];
			}
		}
	}

	// Triangulate mesh.
	if (outer_hull_faces != nullptr) {

		// 1) Take a point A from convex points.
		// 2) Double loop through all point pairs BC where B!=A and C!=A.
		// 3) Determine if ABC make a convex plane (i.e. a plane with no points in front).
		// 4) Add ABC to list of convex faces.

		// Improvements
		// Faces with >3 verts will generate several overlapping triangles.

		mtlList<retro3d::FaceIndexV> convex_hull;

		for (size_t i = 0; i < convex_points.size() - 2; ++i) {
			for (size_t j = i + 1; j < convex_points.size() - 1; ++j) {
				for (size_t k = j + 1; k < convex_points.size(); ++k) {

					// If the plane points towards center, flip one of the index pairs
					retro3d::Plane plane = retro3d::Plane(convex_points[i], convex_points[j], convex_points[k]);
					const mmlVector<3> tri_center = (convex_points[i] + convex_points[j] + convex_points[k]) / 3.0f;
					int32_t I = i, J = j, K = k;
					if (mmlDot(plane.GetNormal(), center - tri_center) < 0.0f) { // The winding order is wrong, reverse the plane
						plane = retro3d::Plane(plane.GetPosition(), -plane.GetNormal());
						mmlSwap(I, J); // Swap any pair.
					}

					bool convex_plane = true;

					for (size_t n = 0; n < convex_points.size(); ++n) {

						if (n == i || n == j || n == k) { continue; }

						if (plane.DetermineSide(convex_points[n], FP_EPSILON) < 0) {
							convex_plane = false;
							break;
						}
					}

					if (convex_plane == true) {
						retro3d::FaceIndexV &face = convex_hull.AddLast();
						face.Resize(3);
						face[0].v = I;
						face[1].v = J;
						face[2].v = K;
					}
				}
			}
		}

		outer_hull_faces->SetCapacity(convex_hull.GetSize());
		mtlItem<retro3d::FaceIndexV> *i = convex_hull.GetFirst();
		while (i != nullptr) {
			outer_hull_faces->Add(i->GetItem());
			i = i->GetNext();
		}
	}
}

void retro3d::CreateConvexHull(const retro3d::Array< mmlVector<3> > &vert_cloud, retro3d::Array< mmlVector<3> > &hull_verts, retro3d::Array< retro3d::FaceIndexV > &hull_faces, float FP_EPSILON)
{
	retro3d::CreateConvexHull(vert_cloud, &hull_verts, &hull_faces, FP_EPSILON);
}

void retro3d::CreateConvexHull(const retro3d::Array< mmlVector<3> > &vert_cloud, retro3d::Array< mmlVector<3> > &hull_verts, float FP_EPSILON)
{
	retro3d::CreateConvexHull(vert_cloud, &hull_verts, nullptr, FP_EPSILON);
}

void retro3d::CreateConvexHull(const retro3d::Array< mmlVector<3> > &vert_cloud, retro3d::Array< retro3d::FaceIndexV > &hull_faces, float FP_EPSILON)
{
	retro3d::CreateConvexHull(vert_cloud, nullptr, &hull_faces, FP_EPSILON);
}

void retro3d::FindConcavePoints(const retro3d::Array< mmlVector<3> > &vert_cloud, retro3d::Array< int32_t > &out_index)
{
	const mmlVector<3> center = retro3d::CalculateCentroid(vert_cloud);
	std::unordered_map< int32_t, int32_t > convex_points;
	convex_points.reserve(size_t(vert_cloud.GetSize()));
	for (int32_t i = 0; i < vert_cloud.GetSize(); ++i) {
		const int32_t extreme_index = retro3d::FindExtremeVertexIndexAlongDirection(vert_cloud, retro3d::NormalFromAToB(center, vert_cloud[i]));
		convex_points[extreme_index] = extreme_index;
	}

	out_index.SetCapacity(vert_cloud.GetSize() - convex_points.size());
	for (int i = 0; i < vert_cloud.GetSize(); ++i) {
		if (convex_points.find(i) == convex_points.end()) {
			out_index.Add(i);
		}
	}
}

void retro3d::FindConcavePoints(const retro3d::Array< mmlVector<3> > &vert_cloud, retro3d::Array< mmlVector<3> > &out_vert)
{
	retro3d::Array< int32_t > out_index;
	retro3d::FindConcavePoints(vert_cloud, out_index);
	out_vert.Resize(out_index.GetSize());
	for (int i = 0; i < out_vert.GetSize(); ++i) {
		out_vert[i] = vert_cloud[out_index[i]];
	}
}

uint64_t retro3d::MeshEditor::Hash(int32_t a)
{
	const uint32_t ua = a >= 0 ? *reinterpret_cast<uint32_t*>(&a) : NO_INDEX;
	return uint64_t(ua);
}

uint64_t retro3d::MeshEditor::Hash(int32_t a, int32_t b)
{
	// NOTE: A unique hash of two indices where h(a,b) = h(b,a)
	if (a < 0 || b < 0) { return uint64_t(NO_INDEX); }
	return (Hash(mmlMin(a, b)) << 32) | Hash(mmlMax(a, b));
}

void retro3d::MeshEditor::StoreClippedFace(retro3d::MeshEditor *out, const retro3d::MeshEditor::FaceMap &unclipped_index, int32_t current_material_index, const retro3d::Array< mmlVector<3> > &clipped_v, const retro3d::Array< mmlVector<2> > &clipped_t, const retro3d::Array< mmlVector<3> > &clipped_n)
{
	FaceMap *out_face = &out->m_surfs[current_material_index].faces[Hash(out->m_surfs[current_material_index].faces.size())]; // NOTE: Adding an element last to the table.
	out_face->i.Create(clipped_v.GetSize());

	int32_t c = 0; // NOTE: (c)lipped index.
	int32_t u = 0; // NOTE: (u)nclipped index.

	uint64_t hv = 0;
	uint64_t ht = 0;
	uint64_t hn = 0;

	bool is_clipped = false;

	while (c < clipped_v.GetSize()) {
		if (clipped_v[c] == m_vert[unclipped_index.i[u].v]) {

			hv = unclipped_index.i[u].v;
			ht = unclipped_index.i[u].t;
			hn = unclipped_index.i[u].n;

			u = (u + 1) % unclipped_index.i.GetSize();
		} else {
			// NOTE: c is not in the vertex list at all, check its neighbor indices (a clipped vertex must have 1 neighbor that is clipped and 1 neighbor that is NOT clipped).
			const int32_t c0 = c > 0                       ? c - 1 : clipped_v.GetSize() - 1;
			const int32_t c1 = c < clipped_v.GetSize() - 1 ? c + 1 : 0;

			// NOTE: Iterate u until u matches c0 or c1 (FP inequality is okay since we are looking for bit-identical matches).
			while (clipped_v[c0] != m_vert[unclipped_index.i[u].v] && clipped_v[c1] != m_vert[unclipped_index.i[u].v]) {
				u = (u + 1) % unclipped_index.i.GetSize();
			}
			const int32_t u0 = u > 0 ? (u - 1) % unclipped_index.i.GetSize() : unclipped_index.i.GetSize() - 1;

			// NOTE: Create a hashed index that is a combination of the vertices that form the edge (because the clipped vertex lies on the edge).
			hv = Hash(unclipped_index.i[u].v, unclipped_index.i[u0].v);
			ht = Hash(unclipped_index.i[u].t, unclipped_index.i[u0].t);
			hn = Hash(unclipped_index.i[u].n, unclipped_index.i[u0].n);

			is_clipped = true;
		}

		out->m_vert[hv] = clipped_v[c];
		if (ht != NO_INDEX && clipped_t.GetSize() == clipped_v.GetSize()) {
			out->m_tcoord[ht] = clipped_t[c];
		}
		if (hn != NO_INDEX && clipped_n.GetSize() == clipped_v.GetSize()) {
			out->m_normal[hn] = mmlNormalizeIf(clipped_n[c]);
		}

		out_face->i[c].v = hv;
		out_face->i[c].t = ht;
		out_face->i[c].n = hn;

		++c;
	}

	if (is_clipped) {
		std::cout << "+" << out_face->i.GetSize() << std::endl;
		for (int i = 0; i < out_face->i.GetSize(); ++i) {
			mmlVector<3> v = out->m_vert[out_face->i[i].v];
			std::cout << "  (" << out_face->i[i].v << ") " << v[0] << "," << v[1] << "," << v[2] << std::endl;
		}
	} else {
		//
	}
}

retro3d::AABB retro3d::MeshEditor::CalculateAABB( void ) const
{
	retro3d::AABB aabb;
	if (m_vert.size() > 0) {
		mmlVector<3> min, max;
		min = max = m_vert.begin()->second;
		for (auto it = m_vert.begin(); it != m_vert.end(); ++it) {
			min = mmlMin(min, it->second);
			max = mmlMax(max, it->second);
		}
		aabb.SetExtremes(min, max);
	} else {
		aabb = retro3d::AABB();
	}
	return aabb;
}

mmlVector<3> retro3d::MeshEditor::CalculateCentroid( void ) const
{
	mmlVector<3> c = mmlVector<3>::Fill(0.0f);
	for (auto i = m_vert.begin(); i != m_vert.end(); ++i) {
		c += i->second;
	}
	c /= float(m_vert.size());
	return c;
}

bool retro3d::MeshEditor::CalculateConvexity( void ) const
{
	// NOTE: Assertion = For a space to be convex, all polygons must be BEHIND or ON all planes made out of the polygons.
	size_t counter = 0;
	for (auto f0 = m_hull.begin(); ++counter < m_hull.size(); ++f0) {
		const FaceMap *face0 = &f0->second;
		const retro3d::Plane plane(m_vert.at(face0->i[0].v), m_vert.at(face0->i[1].v), m_vert.at(face0->i[2].v));
		auto f1 = f0;
		++f1;
		for (; f1 != m_hull.end(); ++f1) {
			const FaceMap *face1 = &f1->second;
			const int32_t vert_count = face1->i.GetSize();
			for (int32_t v0 = 0; v0 < vert_count; ++v0) {
				const mmlVector<3> vcoord = m_vert.at(face1->i[v0].v);
				if (plane.DetermineSide(vcoord, m_FP_EPSILON) > 0) { return false; } // NOTE: Current vertex is in front of current plane, i.e. not convex.
			}
		}
	}
	return true; // NOTE: Passed all tests, i.e. convex.
}

void retro3d::MeshEditor::Postprocess( void )
{
	DefragAttributes();

	m_aabb      = CalculateAABB();
	m_centroid  = CalculateCentroid();
	m_is_convex = CalculateConvexity();
}

uint64_t retro3d::MeshEditor::FindExtremeVertexIndexAlongDirection(const mmlVector<3> &search_dir) const
{
	if (m_vert.size() == 0) { return NO_INDEX; }

	uint64_t max_i   = 0;
	float    max_dot = mmlDot(m_vert.begin()->second, search_dir);

	for (auto i = m_vert.begin(); i != m_vert.end(); ++i) {
		const float dot = mmlDot(i->second, search_dir);
		if (dot > max_dot) {
			max_dot = dot;
			max_i = i->first;
		}
	}

	return max_i;
}

void retro3d::MeshEditor::RemoveConcavePoints( void )
{
	if (m_vert.size() < 4) { return; } // NOTE: None of the points can be concave.

	Map< mmlVector<3> > convex_points;
	convex_points.reserve(size_t(m_vert.size()));

	for (auto i = m_vert.begin(); i != m_vert.end(); ++i) {
		const uint64_t extreme_index = FindExtremeVertexIndexAlongDirection(retro3d::NormalFromAToB(m_centroid, i->second));
		convex_points[extreme_index] = m_vert.at(extreme_index);
	}

	m_vert = std::move(convex_points);
	m_centroid = CalculateCentroid();

//	DefragAttributes(); // NOTE: All of the faces in the convex_box model share a point that is non-convex, meaning all faces will be removed, and later all vertices too.
}

void retro3d::MeshEditor::UpdateHull( void )
{
	size_t total_hull_size = 0;
	for (auto s = m_surfs.begin(); s != m_surfs.end(); ++s) {
		total_hull_size += s->second.faces.size();
	}
	m_hull.clear();
	m_hull.reserve(total_hull_size);

	uint64_t i = 0;
	for (auto s = m_surfs.begin(); s != m_surfs.end(); ++s) {
		for (auto f = s->second.faces.begin(); f != s->second.faces.end(); ++f) {
			m_hull[i++].i = f->second.i;
		}
	}
}

retro3d::MeshEditor::MeshEditor(const float FP_EPSILON) : m_centroid(mmlVector<3>::Fill(0.0f)), m_is_convex(false), m_FP_EPSILON(FP_EPSILON)
{}

void retro3d::MeshEditor::ToModel(retro3d::Model &out)
{
	int32_t i;
	Map<int32_t> v_table;
	Map<int32_t> t_table;
	Map<int32_t> n_table;

	out.name     = m_name;
	out.lightmap = m_lightmap;
	out.aabb     = m_aabb;

	v_table.reserve(m_vert.size());
	out.v.Create(int(m_vert.size()));
	i = 0;
	v_table[NO_INDEX] = -1;
	for (auto it = m_vert.begin(); it != m_vert.end(); ++it) {
		out.v[i] = it->second;
		v_table[it->first] = i;
		++i;
	}

	t_table.reserve(m_tcoord.size());
	out.t.Create(int(m_tcoord.size()));
	i = 0;
	t_table[NO_INDEX] = -1;
	for (auto it = m_tcoord.begin(); it != m_tcoord.end(); ++it) {
		if (it->first != NO_INDEX) {
			out.t[i] = it->second;
			t_table[it->first] = i;
			++i;
		}
	}

	n_table.reserve(m_normal.size());
	out.n.Create(int(m_normal.size()));
	i = 0;
	n_table[NO_INDEX] = -1;
	for (auto it = m_normal.begin(); it != m_normal.end(); ++it) {
		if (it->first != NO_INDEX) {
			out.n[i] = it->second;
			n_table[it->first] = i;
			++i;
		}
	}

	out.m.Create(m_surfs.size());
	bool is_clipped = false;
	for (int32_t mit = 0; mit != out.m.GetSize(); ++mit) {
		retro3d::Material *ma = &out.m[mit];
		const SurfaceMap *mb = &m_surfs[mit];
		ma->name = mb->name;
		ma->cd = mb->diffuse_color;
		ma->td = mb->diffuse_texture;
		ma->f.SetCapacity(int(mb->faces.size()));
		ma->f.Resize(0);
		for (auto fit = mb->faces.begin(); fit != mb->faces.end(); ++fit) {
			is_clipped = false;
			retro3d::FaceIndex *fa = &out.m[mit].f.Add();
			const FaceMap      *fb = &fit->second;
			fa->Create(fb->i.GetSize());
			for (int32_t vit = 0; vit < fa->GetSize(); ++vit) {
				retro3d::IndexVTN *ia = &(*fa)[vit];
				const IndexMap    *ib = &fb->i[vit];
				ia->v = v_table[ib->v];
				ia->t = t_table[ib->t];
				ia->n = n_table[ib->n];

				if (ib->v > NO_INDEX) {
					is_clipped = true;
				}
			}
			if (is_clipped) {
				std::cout << "-" << fa->GetSize() << std::endl;
				for (int i = 0; i < fa->GetSize(); ++i) {
					const retro3d::IndexVTN *ia = &(*fa)[i];
					const IndexMap          *ib = &fb->i[i];
					const mmlVector<3> b = m_vert[ib->v];
					const mmlVector<3> a = out.v[ia->v];
					std::cout << "  (" << ib->v << ") " << b[0] << "," << b[1] << "," << b[2] << " -> " << "(" << ia->v << ")" << a[0] << "," << a[1] << "," << a[2] << std::endl;
				}
			}
		}
	}
}

void retro3d::MeshEditor::FromModel(const retro3d::Model &in, const retro3d::Transform &transform)
{
	// NOTE, TODO, BUG: This code does not handle negative indices. Unsure how this will affect converting the mapped model back to linear format.

	m_name     = in.name;
	m_lightmap = in.lightmap;
	m_aabb.FromGeometry(in.v);

	for (int32_t i = 0; i < in.v.GetSize(); ++i) {
		m_vert[Hash(i)] = transform.TransformPoint(in.v[i]);
	}
	for (int32_t i = 0; i < in.t.GetSize(); ++i) {
		m_tcoord[Hash(i)] = in.t[i];
	}
	for (int32_t i = 0; i < in.n.GetSize(); ++i) {
		m_normal[Hash(i)] = transform.TransformNormal(in.n[i]);
	}
	m_surfs.reserve(in.m.GetSize());
	for (int i = 0; i < in.m.GetSize(); ++i) {
		SurfaceMap              *ma = &m_surfs[Hash(i)];
		const retro3d::Material *mb = &in.m[i];

		ma->name            = mb->name;
		ma->diffuse_color   = mb->cd;
		ma->diffuse_texture = mb->td;

		for (int32_t f = 0; f < mb->f.GetSize(); ++f) {
			FaceMap                  *fa = &ma->faces[Hash(f)];
			const retro3d::FaceIndex *fb = &mb->f[f];

			fa->i.Create(fb->GetSize());

			for (int32_t v = 0; v < fa->i.GetSize(); ++v) {
				IndexMap                *ia = &fa->i[v];
				const retro3d::IndexVTN *ib = &(*fb)[v];

				ia->v = Hash(ib->v);
				ia->t = Hash(ib->t);
				ia->n = Hash(ib->n);
			}
		}
	}

	Postprocess(); // Disalbe this if MergeIdenticalVertices is called
	//MergeIdenticalVertices();
}

void retro3d::MeshEditor::MergeApproximateVertices(const float DIST)
{
	// NOTE: Remove all similar vertices in the model by merging the faces together.
	// BUG:  Does not work (a invalidated when b is erased?).

	size_t hack_counter = 0; // HACK: Because I do not know how to get the second to last iterator in an unordered_map.
	const size_t elem_count = m_vert.size();
	for (auto a = m_vert.begin(); hack_counter++ < elem_count - 1; ++a) {
		auto hack_offset = a; // HACK: Because I do not know how to return a+1 without actually incrementing a via ++a.
		for (auto b = ++hack_offset; b != m_vert.end(); ++b) {
			int j = 0;
			for (; j < 3; ++j) {
				if (mmlIsApproxEqual(a->second[j], b->second[j], DIST) == false) {
					break;
				}
			}
			if (j == 3) {
				for (size_t mit = 0; mit < m_surfs.size(); ++mit) {
					SurfaceMap *mat = &m_surfs[mit];
					for (auto fit = mat->faces.begin(); fit != mat->faces.end(); ++fit) {
						FaceMap *face = &fit->second;
						for (int i = 0; i < face->i.GetSize(); ++i) {
							IndexMap *index = &face->i[i];
							if (index->v == b->first) {
								index->v = a->first;
							}
						}
					}
				}
			}
		}
	}

	// TODO: Pass through all faces. Remove all duplicate indices in face. If face is left with 1 or 2 indices, remove face completely.
	FaceMap new_face;
	for (size_t mit = 0; mit < m_surfs.size(); ++mit) {
		SurfaceMap *mat = &m_surfs[mit];
		for (auto fit = mat->faces.begin(); fit != mat->faces.end();) {
			FaceMap *face = &fit->second;
			new_face.i.SetCapacity(face->i.GetSize());
			new_face.i.Resize(0);
			for (int32_t a = 0; a < face->i.GetSize() - 1; ++a) {
				for (int32_t b = a + 1; b < face->i.GetSize(); ++b) {
					if (face->i[a].v == face->i[b].v) {
						// TODO: Match. Do not store vertex index B.
					}
				}
			}

			if (new_face.i.GetSize() < 3) {
				fit = mat->faces.erase(fit);
			} else {
				fit->second = new_face;
				++fit;
			}
		}
	}

	Postprocess();
}

void retro3d::MeshEditor::MergeIdenticalVertices( void )
{
	MergeApproximateVertices(0.0f);
}

template < typename type_t >
void Reindex(std::unordered_map< uint64_t, type_t > &map, std::unordered_map< uint64_t, uint64_t > &key)
{
	std::unordered_map< uint64_t, type_t > out(map.size());
	key.reserve(map.size());
	uint64_t new_i = 0;
	for (auto i = map.begin(); i != map.end(); ++i, ++new_i) {
		key[i->first] = new_i;
		out[new_i] = i->second;
	}
	map = std::move(out);
}

void retro3d::MeshEditor::ReindexFaces(Map<FaceMap> &map, const Map<uint64_t> &v_key, const Map<uint64_t> &t_key, const Map<uint64_t> &n_key)
{
	for (auto f = map.begin(); f != map.end();) {
		bool erase_face = false;
		for (int i = 0; i < f->second.i.GetSize(); ++i) {
			auto v = v_key.find(f->second.i[i].v);
			if (v != v_key.end() || v->second == NO_INDEX) {
				f->second.i[i].v = v_key.at(f->second.i[i].v);
				if (f->second.i[i].t != NO_INDEX) {
					auto t = t_key.find(f->second.i[i].t);
					f->second.i[i].t = t != t_key.end() ? t->second : NO_INDEX;
				}
				if (f->second.i[i].n != NO_INDEX) {
					auto n = n_key.find(f->second.i[i].n);
					f->second.i[i].n = n != n_key.end() ? n->second : NO_INDEX;
				}
			} else {
				// NOTE: This polygon is broken (contains negative positional indices). Remove it.
				erase_face = true;
				break;
			}
		}

		if (erase_face == false) { ++f; }
		else                     { f = map.erase(f); }
	}
}

void retro3d::MeshEditor::DefragAttributes( void )
{
	// Remove faces that refer to attributes that do not exist.
	size_t face_total = 0;
	size_t erased_faces = 0;
	for (auto s = m_surfs.begin(); s != m_surfs.end(); ++s) {
		face_total += s->second.faces.size();
		for (auto f = s->second.faces.begin(); f != s->second.faces.end();) {
			bool erase_face = false;
			for (int i = 0; i < f->second.i.GetSize(); ++i) {
				IndexMap *n = &f->second.i[i];
				auto vi = m_vert.find(n->v);
				auto ti = m_tcoord.find(n->t);
				auto ni = m_normal.find(n->n);
				if ((vi == m_vert.end() || vi->first == NO_INDEX) || (ti == m_tcoord.end() && n->t != NO_INDEX) || (ni == m_normal.end() && n->n != NO_INDEX)) {
					erase_face = true;
					break;
				}
			}

			if (erase_face == false) { ++f; }
			else                     { f = s->second.faces.erase(f); ++erased_faces; }
		}
	}
	std::cout << "erased_faces=" << erased_faces << "/" << face_total << ", " << std::flush;

	// Remove vertex attributes that are not references in a face.
	Map< mmlVector<3> > packed_v;
	Map< mmlVector<2> > packed_t;
	Map< mmlVector<3> > packed_n;

	packed_v.reserve(m_vert.size());
	packed_t.reserve(m_tcoord.size());
	packed_n.reserve(m_normal.size());

	size_t original_v_size = m_vert.size();
	size_t original_t_size = m_tcoord.size();
	size_t original_n_size = m_normal.size();

	for (size_t mit = 0; mit < m_surfs.size(); ++mit) {
		const SurfaceMap *mat = &m_surfs[mit];
		for (auto fit = mat->faces.begin(); fit != mat->faces.end(); ++fit) {
			const FaceMap *face = &fit->second;
			for (int32_t vit = 0; vit < face->i.GetSize(); ++vit) {
				const IndexMap *i = &face->i[vit];
				packed_v[i->v] = m_vert.at(i->v);
				packed_t[i->t] = m_tcoord.at(i->t);
				packed_n[i->n] = m_normal.at(i->n);
			}
		}
	}
	std::cout << "erased_vert=" << original_v_size - packed_v.size() << "/" << original_v_size << ", ";
	std::cout << "erased_tcoord=" << original_t_size - packed_t.size() << "/" << original_t_size << ", ";
	std::cout << "erased_normal=" << original_n_size - packed_n.size() << "/" << original_n_size << ", " << std::flush;

	// Reindex the remaining vertex attributes 0-n
	Map< uint64_t > reindex_v;
	Map< uint64_t > reindex_t;
	Map< uint64_t > reindex_n;

	Reindex(packed_v, reindex_v);
	Reindex(packed_t, reindex_t);
	Reindex(packed_n, reindex_n);

	// Make remaining faces point to new vertex indices.
	for (auto s = m_surfs.begin(); s != m_surfs.end(); ++s) {
		ReindexFaces(s->second.faces, reindex_v, reindex_t, reindex_n);
	}
	UpdateHull();

//#ifdef RETRO3D_DEBUG
//	std::cout << "Attributes defragmented: (v:" << m_vert.size() << ",t:" << m_tcoord.size() << ",n:" << m_normal.size() << ") ->";
//#endif

	// Store the new attributes.
	m_vert = std::move(packed_v);
	m_tcoord = std::move(packed_t);
	m_normal = std::move(packed_n);

//#ifdef RETRO3D_DEBUG
//	std::cout << "(v:" << m_vert.size() << ",t:" << m_tcoord.size() << ",n:" << m_normal.size() << "), ";
//#endif
}

void retro3d::MeshEditor::AxisSubdivide(const float AXIS_SIZE)
{
	const mmlVector<3> min = m_aabb.GetMin();
	const mmlVector<3> max = m_aabb.GetMax();

	const int mins[3] = { mmlFloor(min[0]), mmlFloor(min[1]), mmlFloor(min[2]) };
	const int maxs[3] = { mmlFloor(max[0]), mmlFloor(max[1]), mmlFloor(max[2]) };

	const int chunks[3] = {
		int(float(mmlCeil(float(maxs[0] - mins[0]))) / AXIS_SIZE),
		int(float(mmlCeil(float(maxs[0] - mins[0]))) / AXIS_SIZE),
		int(float(mmlCeil(float(maxs[0] - mins[0]))) / AXIS_SIZE)
	};

	retro3d::MeshEditor final;
	retro3d::MeshEditor left = *this;
	retro3d::MeshEditor front, back;

	mmlVector<3> normal = mmlVector<3>::Fill(0.0f);
	mmlVector<3> pos = normal;
	for (int i = 0; i < 3; ++i) {
		normal[i] = 1.0f;
		pos[i] = float(mins[i]) + AXIS_SIZE;
		for (int n = 1; n <= chunks[i]; ++n) {
			const retro3d::Plane plane(pos, normal);
			left.Split(plane, &front, &back);
			left = front;
			final.AppendGeometry(back);
			pos[i] += AXIS_SIZE;
		}
		normal[i] = 0.0f;
		pos[i] = 0.0f;
	}

	*this = final;

	// NOTE: Do not merge identical vertices automatically (let the user decide).
}

void retro3d::MeshEditor::Destroy( void )
{
	m_name = "";
	m_vert.clear();
	m_tcoord.clear();
	m_normal.clear();
	m_surfs.clear();
	m_lightmap.Delete();
	m_aabb = retro3d::AABB();
}

void retro3d::MeshEditor::Split(const retro3d::Plane &plane, retro3d::MeshEditor *front, retro3d::MeshEditor *back)
{
	if (front == nullptr && back == nullptr) { return; }

	RETRO3D_ASSERT(front != back);
	RETRO3D_ASSERT(front != this && back != this);

	if (front != nullptr) { front->Destroy(); }
	if (back  != nullptr) { back->Destroy(); }

	// NOTE: Determine if the model is wholly on one side.
	{
		mmlVector<3> v[8];
		m_aabb.GetCorners(v);

		uint32_t i = 0, o = 0, x = 0;
		for (uint32_t n = 0; n < 8; ++n) {
			switch (plane.DetermineSide(v[n], m_FP_EPSILON)) {
			case -1: ++o; break;
			case  0: ++x; break;
			case  1: ++i; break;
			}
		}
		if (i > 0 && o == 0 && front != nullptr) {
			*front = *this;
			return;
		}
		if (o > 0 && i == 0 && back != nullptr) {
			*back = *this;
			return;
		}
	}

	retro3d::Array<mmlVector<3>> clip_v, front_v, back_v;
	retro3d::Array<mmlVector<2>> clip_t, front_t, back_t;
	retro3d::Array<mmlVector<3>> clip_n, front_n, back_n;

	if (front != nullptr) { front->m_surfs.reserve(m_surfs.size()); }
	if (back  != nullptr) { back->m_surfs.reserve(m_surfs.size()); }

	for (size_t mit = 0; mit < m_surfs.size(); ++mit) {

		if (front != nullptr) {
			front->m_surfs[mit].name            = m_surfs[mit].name;
			front->m_surfs[mit].diffuse_color   = m_surfs[mit].diffuse_color;
			front->m_surfs[mit].diffuse_texture = m_surfs[mit].diffuse_texture;
			front->m_surfs[mit].faces.reserve(m_surfs[mit].faces.size());
		}
		if (back  != nullptr) {
			back->m_surfs[mit].name            = m_surfs[mit].name;
			back->m_surfs[mit].diffuse_color   = m_surfs[mit].diffuse_color;
			back->m_surfs[mit].diffuse_texture = m_surfs[mit].diffuse_texture;
			back->m_surfs[mit].faces.reserve(m_surfs[mit].faces.size());
		}

		for (auto fit = m_surfs[mit].faces.begin(); fit != m_surfs[mit].faces.end(); ++fit) {

			// NOTE: Load the current face attributes into separate arrays.
			clip_v.Create(fit->second.i.GetSize());
			clip_t.Create(fit->second.i.GetSize());
			clip_n.Create(fit->second.i.GetSize());

			bool has_normals = true;
			bool has_tcoords = true;

			int32_t i = 0;
			for (int32_t vit = 0; vit < fit->second.i.GetSize(); ++vit) {
				clip_v[i] = m_vert[fit->second.i[vit].v];
				if (has_tcoords == true) {
					if (fit->second.i[vit].t != NO_INDEX && has_tcoords == true) { clip_t[i] = m_tcoord[fit->second.i[vit].t]; }
					else { has_tcoords = false; }
				}
				if (has_normals == true) {
					if (fit->second.i[vit].n != NO_INDEX && has_normals == true) { clip_n[i] = m_normal[fit->second.i[vit].n]; }
					else { has_normals = false; }
				}
				++i;
			}

			// NOTE: Clip the polygon's attributes.
			plane.Clip(clip_v, clip_v, (front != nullptr) ? &front_v : nullptr, (back != nullptr) ? &back_v : nullptr, m_FP_EPSILON);
			if (has_tcoords == true) {
				plane.Clip(clip_v, clip_t, (front != nullptr) ? &front_t : nullptr, (back != nullptr) ? &back_t : nullptr, m_FP_EPSILON);
			} else {
				front_t.Free();
				back_t.Free();
			}
			if (has_normals == true) {
				plane.Clip(clip_v, clip_n, (front != nullptr) ? &front_n : nullptr, (back != nullptr) ? &back_n : nullptr, m_FP_EPSILON);
			} else {
				front_n.Free();
				back_n.Free();
			}

			// NOTE: Put the attributes back to appropriate location.
			if (front != nullptr) { StoreClippedFace(front, fit->second, mit, front_v, front_t, front_n); }
			if (back  != nullptr) { StoreClippedFace(back,  fit->second, mit, back_v,  back_t,  back_n);  }
		}
	}

	if (front != nullptr) {
		if (front->m_vert.size() > 0) {
			front->m_name     = m_name;
			front->m_lightmap = m_lightmap;
			front->Postprocess();
		} else {
			front->Destroy();
		}
	}
	if (back != nullptr) {
		if (back->m_vert.size() > 0) {
			back->m_name     = m_name;
			back->m_lightmap = m_lightmap;
			back->Postprocess();
		} else {
			back->Destroy();
		}
	}
}

void retro3d::MeshEditor::Debug_PrintMaterials( void ) const
{
	for (size_t i = 0; i < m_surfs.size(); ++i) {
		std::cout << m_surfs.at(i).name << std::endl;
		std::cout << m_surfs.at(i).diffuse_color[0] << "," << m_surfs.at(i).diffuse_color[1] << "," << m_surfs.at(i).diffuse_color[2] << std::endl;
	}
}

void retro3d::MeshEditor::AppendModel(const retro3d::Model &model, const retro3d::Transform &transform)
{
	const size_t v_size = m_vert.size();
	const size_t t_size = m_tcoord.size();
	const size_t n_size = m_normal.size();
	const size_t m_size = m_surfs.size();
	for (int i = 0; i < model.v.GetSize(); ++i) {
		m_vert[Hash(v_size + i)] = transform.TransformPoint(model.v[i]);
	}
	for (int i = 0; i < model.t.GetSize(); ++i) {
		m_tcoord[Hash(t_size + i)] = model.t[i];
	}
	for (int i = 0; i < model.n.GetSize(); ++i) {
		m_normal[Hash(n_size + i)] = transform.TransformNormal(model.n[i]);
	}
	for (int i = 0; i < model.m.GetSize(); ++i) {
		SurfaceMap *mat = &m_surfs[m_size + i];
		mat->name = model.m[i].name;
		mat->diffuse_color   = model.m[i].cd;
		mat->diffuse_texture = model.m[i].td;
		mat->faces.reserve(model.m[i].f.GetSize());
		for (int n = 0; n < model.m[i].f.GetSize(); ++n) {
			FaceMap *face = &mat->faces[n];
			face->i.Resize(model.m[i].f[n].GetSize());
			for (int j = 0; j < face->i.GetSize(); ++j) {
				face->i[j].v = model.m[i].f[n][j].v >= 0 ? Hash(model.m[i].f[n][j].v + v_size) : NO_INDEX;
				face->i[j].t = model.m[i].f[n][j].t >= 0 ? Hash(model.m[i].f[n][j].t + t_size) : NO_INDEX;
				face->i[j].n = model.m[i].f[n][j].n >= 0 ? Hash(model.m[i].f[n][j].n + n_size) : NO_INDEX;
			}
		}
	}

	Postprocess();
}

void retro3d::MeshEditor::AppendGeometry(const retro3d::MeshEditor &geom, const retro3d::Transform &transform)
{
	const size_t v_size = m_vert.size();
	const size_t t_size = m_tcoord.size();
	const size_t n_size = m_normal.size();
	const size_t m_size = m_surfs.size();

	m_vert.reserve(v_size + geom.m_vert.size());
	m_tcoord.reserve(t_size + geom.m_tcoord.size());
	m_normal.reserve(n_size + geom.m_normal.size());
	m_surfs.reserve(m_size + geom.m_surfs.size());

	for (size_t i = 0; i < geom.m_vert.size(); ++i) {
		m_vert[v_size + i] = transform.TransformPoint(geom.m_vert.at(i));
	}
	for (size_t i = 0; i < geom.m_tcoord.size(); ++i) {
		m_tcoord[t_size + i] = geom.m_tcoord.at(i);
	}
	for (size_t i = 0; i < geom.m_normal.size(); ++i) {
		m_normal[n_size + i] = transform.TransformNormal(geom.m_normal.at(i));
	}
	for (size_t i = 0; i < geom.m_surfs.size(); ++i) {
		SurfaceMap *md = &m_surfs[m_size + i];
		const SurfaceMap *ms = &geom.m_surfs.at(i);
		md->name = ms->name;
		md->diffuse_color   = ms->diffuse_color;
		md->diffuse_texture = ms->diffuse_texture;
		md->faces.reserve(ms->faces.size());
		for (size_t j = 0; j < ms->faces.size(); ++j) {
			FaceMap *fd = &md->faces[j];
			const FaceMap *fs = &ms->faces.at(j);
			fd->i.Resize(fs->i.GetSize());
			for (int k = 0; k < fd->i.GetSize(); ++k) {
				fd->i[k].v = fs->i[k].v != NO_INDEX ? v_size + fs->i[k].v : NO_INDEX;
				fd->i[k].t = fs->i[k].t != NO_INDEX ? t_size + fs->i[k].t : NO_INDEX;
				fd->i[k].n = fs->i[k].n != NO_INDEX ? n_size + fs->i[k].n : NO_INDEX;
			}
		}
	}

	Postprocess();
}

namespace impl
{

struct Face
{
	mmlVector<3> normal;
	float        disc;
	int32_t      I[3];
};

struct TwoSet
{
	int32_t a = -1;
	int32_t b = -1;

	void Insert(int32_t x) { (a == -1 ? a : b) = x; }
	bool Contains(int32_t x) const { return a == x || b == x; }
	void Erase(int32_t x) { (a == x ? a : b) = -1; }
	int32_t GetSize( void ) const { return int32_t(a != -1) + int32_t(b != -1); }
} E[1010][1010];

impl::Face MakeFace(int32_t i, int32_t j, int32_t k, int32_t inside_i, const std::unordered_map<uint64_t, mmlVector<3>> &v)
{
	E[i][j].Insert(k);
	E[i][k].Insert(j);
	E[j][k].Insert(i);

	impl::Face f;
	f.I[0] = i;
	f.I[1] = j;
	f.I[2] = k;
	f.normal = mmlCross(v.at(j) - v.at(i), v.at(k) - v.at(i));
	f.disc = mmlDot(f.normal, v.at(i));
	if (mmlDot(f.normal, v.at(inside_i)) > f.disc) {
		f.normal = -f.normal;
		f.disc = -f.disc;
	}
	return f;
}

}

void retro3d::MeshEditor::MakeConvex( void )
{
	/*
	retro3d: CreateConvexHull does not result in a convex hull (does however output the convex points).
		Take a point of convex points.
		Sort other points by distance.
		Double loop through all additional point pairs.
		Determine if 3 points make a convex plane (i.e. a plane with no points in front).
		If convex, see if other points lie on same plane.
		Add all points on plane to list of convex faces.
	*/

	// https://gist.github.com/msg555/4963794
	// https://www.kiv.zcu.cz/site/documents/verejne/vyzkum/publikace/technicke-zpravy/2002/tr-2002-02.pdf

	// Remove all concave points to speed up process.
	RemoveConcavePoints();

	if (m_vert.size() < 4) { return; }

	// A convex hull using actual surfaces does not make sense, so we just set to use a generic one.
	m_surfs.clear();
	m_surfs.reserve(1);
	SurfaceMap *material = &m_surfs[0];
	material->name = "Default";
	material->diffuse_color = mmlVector<3>::Fill(1.0f);
	material->diffuse_texture.Delete();

	mtlList<impl::Face> faces;

	// Construct an initial tetrahedron from the first 4 points in the geometry.
	for (int32_t i = 0; i < 4; ++i) {
		for (int32_t j = i + 1; j < 4; ++j) {
			for (int32_t k = j + 1; k < 4; ++k) {
				faces.AddLast(impl::MakeFace(i, j, k, 6 - i - j - k, m_vert));
			}
		}
	}

	// Add a point to the hull until no more points are available.
	for (int32_t i = 4; i < (int32_t)m_vert.size(); ++i) {
		mtlItem<impl::Face> *face = faces.GetFirst();
		while (face != nullptr) {
			if (mmlDot(face->GetItem().normal, m_vert.at(i)) > face->GetItem().disc) {
				impl::E[face->GetItem().I[0]][face->GetItem().I[1]].Erase(face->GetItem().I[2]);
				impl::E[face->GetItem().I[0]][face->GetItem().I[2]].Erase(face->GetItem().I[1]);
				impl::E[face->GetItem().I[1]][face->GetItem().I[2]].Erase(face->GetItem().I[0]);
				face = face->Remove();
			} else {
				face = face->GetNext();
			}
		}

		//
		face = faces.GetFirst();
		const int32_t FaceCount = faces.GetSize();
		for (int32_t j = 0; j < FaceCount; ++j) {
			for (int32_t a = 0; a < 3; ++a) {
				for (int32_t b = a + 1; b < 3; ++b) {
					const int32_t c = 3 - a - b;
					if (impl::E[face->GetItem().I[a]][face->GetItem().I[b]].GetSize() == 2) {
						continue;
					}
					faces.AddLast(impl::MakeFace(face->GetItem().I[a], face->GetItem().I[b], i, face->GetItem().I[c], m_vert));
				}
			}
			face = face->GetNext();
		}
	}

	m_hull = m_surfs[0].faces;
	Postprocess();
}

bool retro3d::MeshEditor::IsConvex( void ) const
{
	return m_is_convex;
}

retro3d::DisplayModel::GeometryQueue::GeometryQueue( void ) : m_data(nullptr), m_items(nullptr), m_stack(nullptr), m_item_count(0), m_max_items(0), m_max_stack(0), m_stack_count(0)
{}

void retro3d::DisplayModel::GeometryQueue::Create(uint32_t max_items, uint32_t max_stack)
{
	if (max_items > m_max_items) {
		m_max_items = max_items;
		m_max_stack = max_stack;
		delete [] m_data;
		m_data = new uint8_t[ m_max_items * sizeof(const GeometryNode*) + m_max_stack * sizeof(StackItem) ];
		m_items = reinterpret_cast<const GeometryNode**>(m_data);
		m_stack = reinterpret_cast<StackItem*>(m_data + (m_max_items * sizeof(const GeometryNode*)));
	}
	m_item_count = 0;
	m_stack_count = 0;
}

void retro3d::DisplayModel::GeometryQueue::Push(const retro3d::DisplayModel::GeometryNode *node)
{
	RETRO3D_ASSERT(m_item_count < m_max_items);
	m_items[m_item_count++] = node;
}

void retro3d::DisplayModel::GeometryQueue::PushStack(const retro3d::DisplayModel::GeometryNode *node, retro3d::Contain vis, uint8_t dir)
{
	m_stack[m_stack_count++] = { node, vis, dir };
}

void retro3d::DisplayModel::GeometryQueue::PopStack( void )
{
	--m_stack_count;
}

const retro3d::DisplayModel::GeometryQueue::StackItem *retro3d::DisplayModel::GeometryQueue::TopStack( void ) const
{
	return &m_stack[m_stack_count - 1];
}

uint32_t retro3d::DisplayModel::GeometryQueue::GetStackCount( void ) const
{
	return m_stack_count;
}

retro3d::DisplayModel::GeometryQueue::~GeometryQueue( void )
{
	delete [] m_data;
}

retro3d::DisplayModel::GeometryQueue::GeometryQueue(retro3d::DisplayModel::GeometryQueue &&stack)
{
	m_data        = stack.m_data;
	m_items       = stack.m_items;
	m_stack       = stack.m_stack;
	m_item_count  = stack.m_item_count;
	m_max_items   = stack.m_max_items;
	m_max_stack   = stack.m_max_stack;
	m_stack_count = stack.m_stack_count;

	stack.m_data        = nullptr;
	stack.m_items       = nullptr;
	stack.m_stack       = nullptr;
	stack.m_item_count  = 0;
	stack.m_max_items   = 0;
	stack.m_max_stack   = 0;
	stack.m_stack_count = 0;
}

const retro3d::DisplayModel::GeometryNode *retro3d::DisplayModel::GeometryQueue::Pop( void )
{
	return IsEmpty() ? nullptr : m_items[--m_item_count];
}

const retro3d::DisplayModel::GeometryNode *retro3d::DisplayModel::GeometryQueue::Peek( void ) const
{
	return IsEmpty() ? nullptr: m_items[m_item_count - 1];
}

bool retro3d::DisplayModel::GeometryQueue::IsEmpty( void ) const
{
	return m_item_count == 0;
}

void retro3d::DisplayModel::Convert(retro3d::DisplayModel::ModelData &out, const retro3d::Array< mmlVector<3> > &vertex_pos, const retro3d::Array< mmlVector<2> > &tcoord, const retro3d::Array< mmlVector<3> > &normal, const retro3d::Array< retro3d::Material > &materials)
{
	for (int m = 0; m < materials.GetSize(); ++m) {
		const retro3d::Material *material = &materials[m];
		for (int f = 0; f < material->f.GetSize(); ++f) {
			const retro3d::FaceIndex *face_index = &material->f[f];
			ModelData::Polygon *md_poly = &out.polys.AddLast();
			md_poly->v.Create(face_index->GetSize());
			md_poly->m = uint32_t(m);
			for (int v = 0; v < md_poly->v.GetSize(); ++v) {
				md_poly->v[v].v = vertex_pos[(*face_index)[v].v];
				md_poly->v[v].t = ((*face_index)[v].t >= 0 && (*face_index)[v].t < tcoord.GetSize()) ? tcoord[(*face_index)[v].t] : mmlVector<2>::Fill(0.0f);
				md_poly->v[v].n = ((*face_index)[v].n >= 0 && (*face_index)[v].n < normal.GetSize()) ? normal[(*face_index)[v].n] : mmlVector<3>::Fill(0.0f);
			}
			md_poly->n = mmlVector<3>::Fill(0.0f);
			for (int v = 1; v < md_poly->v.GetSize() - 1; ++v) {
				mmlVector<3> N = mmlSurfaceNormal(md_poly->v[0].v, md_poly->v[v].v, md_poly->v[v + 1].v);
				if (!std::isnan(N[0]) && !std::isnan(N[1]) && !std::isnan(N[2])) { // sub-triangles are too small to determine normal
					md_poly->n += N;
				}
			}
			if (mmlIsApproxEqual(md_poly->n.Len(), 0.0f) == true) { // sub-triangles are too small to determine normal
				md_poly->n[0] = md_poly->n[1] = md_poly->n[2] = 0.0f;
			} else {
				md_poly->n.Normalize();
			}
		}
	}
}

bool retro3d::DisplayModel::IsConvex(const retro3d::DisplayModel::ModelData &model_data)
{
	// Algorithm:
		// For a space to be convex, all polygons must be BEHIND or ON all planes made out of the polygons.
	const mtlItem<ModelData::Polygon> *split_poly = model_data.polys.GetFirst();
	while (split_poly != nullptr) {
		const retro3d::Plane plane(split_poly->GetItem().v[0].v, split_poly->GetItem().n);
		const mtlItem<ModelData::Polygon> *poly = model_data.polys.GetFirst();
		while (poly != nullptr) {
			if (poly != split_poly) {
				for (int v = 0; v < poly->GetItem().v.GetSize(); ++v) {
					mmlVector<3> vcoord = poly->GetItem().v[v].v;
					if (plane.DetermineSide(vcoord) > 0) { return false; } // current vertex is in front of current plane, i.e. not convex
				}
			}
			poly = poly->GetNext();
		}
		split_poly = split_poly->GetNext();
	}
	return true; // passed all tests, i.e. convex
}

bool retro3d::DisplayModel::FindSplittingPoly(retro3d::DisplayModel::ModelData &model_data)
{
	// TODO:
		// Try using only polygons that make the shape non-convex as candidates for splitting
		// Finally settle on the polygon that splits the node in as equal pieces as possible

	if (model_data.polys.GetSize() <= 1) { return false; }

	uint32_t best_score = 0;
	bool     first_run  = true;

	mtlItem<ModelData::Polygon> *split_poly = model_data.polys.GetFirst();

	while (split_poly != nullptr) {

		if (mmlIsApproxEqual(split_poly->GetItem().n.Len(), 0.0f) == true) {
			// If there is no valid normal then we can not use this as a splitting plane
			split_poly = split_poly->GetNext();
			continue;
		}

		retro3d::Plane splitter = retro3d::Plane(split_poly->GetItem().v[0].v, split_poly->GetItem().n);

		int32_t  num_poly_in = 0;
		int32_t  num_poly_out = 0;
		uint32_t num_poly_split = 0;

		const mtlItem<ModelData::Polygon> *poly = model_data.polys.GetFirst();
		while (poly != nullptr) {
			if (poly != split_poly) {
				// Classify the current polygon by test-splitting it
				uint32_t vert_back = 0;
				uint32_t vert_front = 0;
				for (int v = 0; v < poly->GetItem().v.GetSize(); ++v) {
					mmlVector<3> vcoord = poly->GetItem().v[v].v;
					switch (splitter.DetermineSide(vcoord)) {
					case -1: ++vert_back; break;
					case  1: ++vert_front; break;
					default: break;
					}
				}

				// Carry statistics over how the polygon was split
				if (vert_back > 0 && vert_front > 0) { ++num_poly_split; }
				else if (vert_back > 0)              { ++num_poly_out; }
				else if (vert_front  > 0)            { ++num_poly_in; }
				// Do not add any points for polygons wholly on plane
			}
			poly = poly->GetNext();
		}

		// Calculate splitting plane score
		uint32_t current_score = uint32_t(mmlAbs(num_poly_in - num_poly_out)) + 2 * num_poly_split;

		// Keep track of current splitting plane if current score is better than best score
		if (current_score < best_score || first_run == true) {
			first_run = false;
			model_data.splitting_plane = splitter;
			model_data.splitting_poly = split_poly;
			best_score = current_score;
		}

		split_poly = split_poly->GetNext();
	}
	return first_run == false;
}

void InsertVertex(retro3d::Array<retro3d::DisplayModel::Vertex> &poly_vert, int at_index, const retro3d::DisplayModel::Vertex &vert)
{
	retro3d::Array<retro3d::DisplayModel::Vertex> new_vert;
	new_vert.Create(poly_vert.GetSize() + 1);

	for (int i = 0; i < at_index; ++i) {
		new_vert[i] = poly_vert[i];
	}
	new_vert[at_index] = vert;
	for (int i = at_index; i < poly_vert.GetSize(); ++i) {
		new_vert[i+1] = poly_vert[i];
	}

	poly_vert = new_vert;
}

void retro3d::DisplayModel::FixTJunction(retro3d::DisplayModel::ModelData *node, const retro3d::DisplayModel::ModelData::Polygon *div_poly, const mmlVector<3> &v1, const mmlVector<3> &v2, const mmlVector<3> &vx, float x)
{
	if (node == nullptr) { return; }

	mtlItem<ModelData::Polygon> *pi = node->polys.GetFirst();
	while (pi != nullptr) {
		ModelData::Polygon *poly = &pi->GetItem();
		if (poly != div_poly) {

			for (int i = 0, j = poly->v.GetSize() - 1; i < poly->v.GetSize(); j = i++) {

				if (poly->v[i].v == v1 && poly->v[j].v == v2) {

					DisplayModel::Vertex v;
					v.v = vx;
					v.t = mmlLerp(poly->v[i].t, poly->v[j].t, x);
					v.n = mmlLerp(poly->v[i].n, poly->v[j].n, x);

					InsertVertex(poly->v, i, v);

					return;

				} else if (poly->v[i].v == v2 && poly->v[j].v == v1) {

					DisplayModel::Vertex v;
					v.v = vx;
					v.t = mmlLerp(poly->v[j].t, poly->v[i].t, x);
					v.n = mmlLerp(poly->v[j].n, poly->v[i].n, x);

					InsertVertex(poly->v, i, v);

					return;
				}
			}
		}
		pi = pi->GetNext();
	}

	FixTJunction(node->back, div_poly, v1, v2, vx, x);
	FixTJunction(node->front, div_poly, v1, v2, vx, x);
}

void retro3d::DisplayModel::Split(retro3d::DisplayModel::ModelData &model_data, retro3d::DisplayModel::ModelData &root)
{
	// TODO: Find and fix T-junctions

	if (IsConvex(model_data) == true) { return; } // if the model is convex then there is no need to split it into further pieces (render order will be correct regardless of point of view)

	// Find best splitting plane + polygon
	if (FindSplittingPoly(model_data) == false) { return; }

	// List to keep track of
	model_data.front = new ModelData;
	model_data.back = new ModelData;

	// Iterate through polygons in node
	mtlItem<ModelData::Polygon> *pi = model_data.polys.GetFirst();
	while (pi != nullptr) {

		ModelData::Polygon *poly = &pi->GetItem();

		bool on_plane = true;

		if (pi != model_data.splitting_poly) {

			// Determine if polygon is wholly on plane
			for (int v = 0; v < poly->v.GetSize() && on_plane == true; ++v) {
				if (model_data.splitting_plane.DetermineSide(poly->v[v].v) != 0) {
					on_plane = false;
				}
			}

			if (on_plane == false) { // Store polygon in front and/or back node

				// NOTE: Is there no way to use generic clipping functions instead of repeating code here?

				mtlArray<float>                 dist(poly->v.GetSize());
				mtlArray<int32_t>               side(poly->v.GetSize());
				ModelData::Polygon             *front_poly = &model_data.front->polys.AddLast();
				ModelData::Polygon             *back_poly  = &model_data.back->polys.AddLast();
				retro3d::Array< mmlVector<3> >  front_vert, back_vert;

				const int MAX_VERT = poly->v.GetSize() + 1;
				front_poly->v.SetCapacity(MAX_VERT);
				back_poly->v.SetCapacity(MAX_VERT);
				front_vert.SetCapacity(MAX_VERT);
				back_vert.SetCapacity(MAX_VERT);

				for (int i = 0; i < poly->v.GetSize(); ++i) {
					dist[i] = mmlDot(poly->v[i].v - model_data.splitting_plane.GetPosition(), model_data.splitting_plane.GetNormal());
					side[i] = model_data.splitting_plane.DetermineSide(dist[i]);
				}

				for (int i = 0, j = poly->v.GetSize() - 1; i < poly->v.GetSize(); j=i, ++i) {

					if ((side[i] < 0) == (side[j] >= 0)) {
						const float                x   = dist[i] / (dist[i] - dist[j]);
						const DisplayModel::Vertex vtx = mmlLerp(poly->v[i], poly->v[j], x);
						const mmlVector<3>         v   = vtx.v;

						// TODO: If normals are interpolated then we need to renormalize, but not if they are colors.

						front_poly->v.Add(vtx);
						back_poly->v.Add(vtx);

						front_vert.Add(v);
						back_vert.Add(v);

						// Must emit control point on neighboring polygons immediately:
							// http://underpop.online.fr/j/java/help/implementing-a-2d-bsp-tree-d-scene-management-using-bsp-trees-java.html.gz
						FixTJunction(&root, poly, poly->v[i].v, poly->v[j].v, v, x);
					}
					if (side[i] >= 0) {
						front_poly->v.Add(poly->v[i]);
						front_vert.Add(poly->v[i].v);
					}
					if (side[i] < 0) {
						back_poly->v.Add(poly->v[i]);
						back_vert.Add(poly->v[i].v);
					}
				}

				if (front_vert.GetSize() <= 2 || retro3d::IsVerySmallPolygon(front_vert) == true) {
					*back_poly = *poly;
					model_data.front->polys.RemoveLast();
				} else if (back_vert.GetSize() <= 2 || retro3d::IsVerySmallPolygon(back_vert) == true) {
					*front_poly = *poly;
					model_data.back->polys.RemoveLast();
				} else {
					front_poly->m = poly->m;
					front_poly->n = poly->n;
					back_poly->m  = poly->m;
					back_poly->n  = poly->n;
				}
			}
		}

		if (on_plane == true) {
			pi = pi->GetNext();
		} else {
			pi = pi->Remove();
		}
	}

	// Recursively process front and back nodes
	if (model_data.front->polys.GetSize() > 0) {
		Split(*model_data.front, root);
	} else {
		delete model_data.front;
		model_data.front = nullptr;
	}

	if (model_data.back->polys.GetSize() > 0) {
		Split(*model_data.back, root);
	} else {
		delete model_data.back;
		model_data.back = nullptr;
	}
}

void retro3d::DisplayModel::ChunkySplit(retro3d::DisplayModel::ModelData &model_data, retro3d::DisplayModel::ModelData &root, retro3d::Array< mmlVector<3> > &concave_points)
{
	if (concave_points.GetSize() >= 3) {

		// Pick any splitting plane.
		model_data.splitting_plane = retro3d::Plane(concave_points[0], concave_points[1], concave_points[2]);

		// Create two new arrays of concave points lying on each side of the splitting plane (omit points on the plane).
		int concave_front_count = 0;
		int concave_back_count = 0;
		for (int i = 0; i < concave_points.GetSize(); ++i) {
			switch (model_data.splitting_plane.DetermineSide(concave_points[i])) {
			case 1:
				++concave_front_count;
				break;
			case -1:
				++concave_back_count;
				break;
			default: break;
			}
		}
		retro3d::Array< mmlVector<3> > concave_front, concave_back;
		concave_front.SetCapacity(concave_front_count);
		concave_back.SetCapacity(concave_back_count);
		for (int i = 0; i < concave_points.GetSize(); ++i) {
			switch (model_data.splitting_plane.DetermineSide(concave_points[i])) {
			case 1:
				concave_front.Add(concave_points[i]);
				break;
			case -1:
				concave_back.Add(concave_points[i]);
				break;
			default: break;
			}
		}

		concave_points.Free();

		model_data.front = new ModelData;
		model_data.back = new ModelData;
		ChunkySplit(*model_data.front, root, concave_front);
		ChunkySplit(*model_data.back, root, concave_back);

	} else if (concave_points.GetSize() > 0) {
		// pick the 1 or 2 points left inside the shape and group together with another (remaining vertex) that splits the volume roughly in two equally sized halves
	} else {
		// write to model_data.front
		// write to model_data.back
	}
}

void retro3d::DisplayModel::AllocateSpace(const retro3d::DisplayModel::ModelData &root)
{
	// Count elements: Traverse in order
	uint32_t node_count    = 0;
	uint32_t surface_count = 0;
	uint32_t poly_count    = 0;
	uint32_t vertex_count  = 0;
	uint32_t tri_count     = 0;

	const uint32_t    tree_depth = root.GetDepth();
	const ModelData  *current    = &root;
	const ModelData **stack      = new const ModelData*[tree_depth];
	uint32_t          stack_size = 0;

	while (current != nullptr || stack_size != 0) {

		while (current != nullptr) {
			stack[stack_size++] = current;
			current = current->back;
		}

		current = stack[--stack_size];

		poly_count += uint32_t(current->polys.GetSize());
		++node_count;
		++surface_count; // there is at least one surface per node
		const mtlItem<ModelData::Polygon> *p = current->polys.GetFirst();
		uint32_t last_material = p->GetItem().m;
		while (p != nullptr) {
			// Any time the material index changes we add another Surface to the GeometryNode
			if (last_material != p->GetItem().m) {
				last_material = p->GetItem().m;
				++surface_count;
			}
			vertex_count += uint32_t(p->GetItem().v.GetSize());
			tri_count += vertex_count - 2;
			p = p->GetNext();
		}

		current = current->front;
	}

	// Create data arrays
	const uint32_t pdata_byte_size = sizeof(GeometryNode) * node_count + sizeof(Surface) * surface_count;
	const uint32_t vdata_byte_size = sizeof(Polygon) * poly_count + sizeof(DisplayModel::Vertex) * vertex_count;
	const uint32_t byte_size = pdata_byte_size + vdata_byte_size;
	if (byte_size > (uint32_t)std::numeric_limits<int>::max()) {
		std::cout << "[DisplayModel.AllocateSpace:ERROR] Too large array. ";
		return;
	}
	m_data.Create(int(byte_size));
	m_pdata = &m_data[0];
	m_vdata = &m_data[pdata_byte_size];
	m_p_size = pdata_byte_size;
	m_v_size = vdata_byte_size;
	m_depth = tree_depth;
	m_node_count = node_count;
	m_poly_count = poly_count;
	m_vert_count = vertex_count;
	m_tri_count  = tri_count;
	m_area = 0.0f;
	m_volume = 0.0f;
	m_vertex_format = VertexFormat_Position|VertexFormat_TexCoord|VertexFormat_Normal;

	delete [] stack;
}

void retro3d::DisplayModel::BreadthFirstWrite(const retro3d::DisplayModel::ModelData &root)
{
	// Assign data: Traverse in order
	uint32_t pdata_byte_index = 0;
	uint32_t vdata_byte_index = 0;

	struct QueueItem
	{
		const ModelData *node;
		uint32_t         parent_pindex;
		uint32_t        *rel_index_ptr;
	};

	std::queue<QueueItem> q;
	q.push({ &root, 0, nullptr });

	while (q.empty() == false) {

		const uint32_t parent_pindex = pdata_byte_index;

		QueueItem current = q.front();
		q.pop();

		GeometryNode *geom_node = reinterpret_cast<GeometryNode*>(&m_pdata[pdata_byte_index]);
		geom_node->poly_index = (m_p_size - pdata_byte_index) + vdata_byte_index; // polygon index relative to current node
		geom_node->node_normal = current.node->splitting_plane.GetNormal();
		geom_node->surface_count = 1;
		geom_node->poly_count = 0;
		geom_node->aabb = retro3d::AABB(mmlVector<3>::Fill(0.0f), mmlVector<3>::Fill(0.0f));
		geom_node->back_node_index = geom_node->front_node_index = 0; // set these to 0 now - may get overwritten later if not a leaf
		if (current.rel_index_ptr != nullptr) {
			*current.rel_index_ptr = pdata_byte_index - current.parent_pindex; // sets the parent relative index to the difference between parent index and current index
		}
		pdata_byte_index += sizeof(GeometryNode);

		const mtlItem<ModelData::Polygon> *p = current.node->polys.GetFirst();
		uint32_t prev_material_index = p->GetItem().m;
		Surface *surface = reinterpret_cast<Surface*>(&m_pdata[pdata_byte_index]);
		surface->poly_count = 0;
		surface->material_index = prev_material_index;
		while (p != nullptr) {
			if (prev_material_index != p->GetItem().m) {
				++geom_node->surface_count;
				pdata_byte_index += sizeof(Surface);
				prev_material_index = p->GetItem().m;
				surface = reinterpret_cast<Surface*>(&m_pdata[pdata_byte_index]);
				surface->poly_count = 0;
				surface->material_index = prev_material_index;
			}
			++surface->poly_count;
			++geom_node->poly_count;
			Polygon *poly = reinterpret_cast<Polygon*>(&m_vdata[vdata_byte_index]);
			poly->vert_count = uint32_t(p->GetItem().v.GetSize());
			vdata_byte_index += sizeof(Polygon);
			for (int i = 0; i < p->GetItem().v.GetSize(); ++i) {
				DisplayModel::Vertex *vert = reinterpret_cast<Vertex*>(&m_vdata[vdata_byte_index]);
				vert->v = p->GetItem().v[i].v;
				vert->t = p->GetItem().v[i].t;
				vert->n = p->GetItem().v[i].n;
				vdata_byte_index += sizeof(DisplayModel::Vertex);
			}
			p = p->GetNext();
		}
		pdata_byte_index += sizeof(Surface);

		if (current.node->back  != nullptr) { q.push({ current.node->back,  parent_pindex, &geom_node->back_node_index  }); }
		if (current.node->front != nullptr) { q.push({ current.node->front, parent_pindex, &geom_node->front_node_index }); }
	}
}

void retro3d::DisplayModel::UpdateAABBs(retro3d::DisplayModel::GeometryNode *geom_node)
{
	if (geom_node == nullptr) { return; }

	UpdateAABBs(geom_node->GetBackNode());
	UpdateAABBs(geom_node->GetFrontNode());

	mmlVector<3> min = geom_node->GetPolygons()->GetVerts()->v;
	mmlVector<3> max = min;

	Polygon *polygon = geom_node->GetPolygons();
	for (uint32_t p = 0; p < geom_node->poly_count; ++p) {
		for (uint32_t v = 0; v < polygon->vert_count; ++v) {
			min = mmlMin(min, polygon->GetVerts()[v].v);
			max = mmlMax(max, polygon->GetVerts()[v].v);
		}
		polygon = polygon->GetNext();
	}
	geom_node->aabb.SetExtremes(min, max);

	if (geom_node->GetBackNode()  != nullptr) { geom_node->aabb = geom_node->aabb | geom_node->GetBackNode()->aabb;  }
	if (geom_node->GetFrontNode() != nullptr) { geom_node->aabb = geom_node->aabb | geom_node->GetFrontNode()->aabb; }
}

void retro3d::DisplayModel::FromModelData(const retro3d::DisplayModel::ModelData &root)
{
	Destroy();

	AllocateSpace(root);

	BreadthFirstWrite(root);

	UpdateAABBs(rw_GetGeometryNode());
}

void retro3d::DisplayModel::CopyMaterials(const retro3d::Array< retro3d::Material > &materials)
{
	m_materials.Create(materials.GetSize());
	for (int i = 0; i < m_materials.GetSize(); ++i) {
		m_materials[i].color   = materials[i].cd;
		m_materials[i].texture = materials[i].td;
	}
}

retro3d::DisplayModel::GeometryNode *retro3d::DisplayModel::rw_GetGeometryNode( void )
{
	return reinterpret_cast<GeometryNode*>(m_pdata);
}

retro3d::DisplayModel::Polygon *retro3d::DisplayModel::rw_GetPolygons( void )
{
	return reinterpret_cast<Polygon*>(m_vdata);
}

retro3d::DisplayModel::DisplayModel( void ) : m_data(), m_p_size(0), m_v_size(0), m_pdata(nullptr), m_depth(0), m_node_count(0), m_vdata(nullptr), m_poly_count(0), m_tri_count(0), m_vert_count(0), m_materials(), m_volume(0.0f), m_area(0.0f), m_vertex_format(VertexFormat_Position|VertexFormat_TexCoord|VertexFormat_Normal)
{}

void retro3d::DisplayModel::CreateGeometryNode(const retro3d::Array< mmlVector<3> > &vertex_pos, const retro3d::Array< mmlVector<2> > &tcoord, const retro3d::Array< mmlVector<3> > &normal, const retro3d::Array<retro3d::Material> &materials)
{
//	ModelData mdata;
//	Convert(vertex_pos, tcoord, normal, materials, mdata);
//	FromModelData(mdata);
//	CopyMaterials(materials);

	Destroy();

	if (vertex_pos.GetSize() > std::numeric_limits<uint16_t>::max() || tcoord.GetSize() > std::numeric_limits<uint16_t>::max() || normal.GetSize() > std::numeric_limits<uint16_t>::max() || materials.GetSize() > std::numeric_limits<uint16_t>::max()) {
		std::cout << "[DisplayModel.CreateGeometryNode:ERROR] One or more arrays containing more than " << std::numeric_limits<uint16_t>::max() << " elements. ";
		return;
	}

	m_v_size = 0;
	for (int i = 0; i < materials.GetSize(); ++i) {
		for (int j = 0; j < materials[i].f.GetSize(); ++j) {
			m_v_size += sizeof(Polygon);
			m_v_size += uint32_t(materials[i].f[j].GetSize()) * sizeof(retro3d::DisplayModel::Vertex);

			++m_poly_count;
			m_tri_count += uint32_t(materials[i].f[j].GetSize() - 2);
			m_vert_count += uint32_t(materials[i].f[j].GetSize());
		}
	}

	retro3d::Array<uint8_t> vdata;
	vdata.Create(int(m_v_size));
	{
		retro3d::DisplayModel::Vertex *vert = reinterpret_cast<retro3d::DisplayModel::Vertex*>(&vdata[0]);
		for (int i = 0; i < materials.GetSize(); ++i) {
			const retro3d::Material *material = &materials[i];
			for (int j = 0; j < material->f.GetSize(); ++j) {
				const retro3d::FaceIndex *face = &material->f[j];
				Polygon *poly = reinterpret_cast<Polygon*>(vert);
				poly->vert_count = uint32_t(material->f[j].GetSize());
				vert = poly->GetVerts();
				for (uint32_t n = 0; n < poly->vert_count; ++n) {
					const retro3d::IndexVTN *index = &(*face)[n];
					vert->v = vertex_pos[index->v];
					vert->t = index->t >= 0 ? tcoord[index->t] : mmlVector<2>::Fill(0.0f);
					vert->n = index->n >= 0 ? normal[index->n] : mmlVector<3>::Fill(0.0f);
					vert = vert->GetNext();
				}
			}
		}
	}

	m_materials.Create(materials.GetSize());
	for (int i = 0; i < m_materials.GetSize(); ++i) {
		m_materials[i].color   = materials[i].cd;
		m_materials[i].texture = materials[i].td;
	}

	m_p_size = sizeof(GeometryNode) + sizeof(Surface) * uint32_t(materials.GetSize());

	retro3d::Array<uint8_t> pdata;
	pdata.Create(int(m_p_size));
	GeometryNode *node = reinterpret_cast<GeometryNode*>(&pdata[0]);
	*node = GeometryNode{
		0, 0,
		uint32_t(materials.GetSize()),
		m_p_size,
		m_poly_count,
		retro3d::Transform::GetWorldForward(),
		retro3d::AABB(vertex_pos)
	};
	Surface *surf = node->GetSurfaces();
	for (uint32_t i = 0; i < node->surface_count; ++i) {
		surf->material_index = i;
		surf->poly_count = uint32_t(materials[i].f.GetSize());
		surf = surf->GetNext();
	}

	m_depth = 1;
	m_node_count = 1;

	m_data.Create(int(m_p_size) + int(m_v_size));
	for (uint32_t i = 0; i < m_p_size; ++i) {
		m_data[i] = pdata[i];
	}
	for (uint32_t i = 0; i < m_v_size; ++i) {
		m_data[i + m_p_size] = vdata[i];
	}

	m_pdata = &m_data[0];
	m_vdata = &m_data[m_p_size];
}

void retro3d::DisplayModel::CreateGeometryTree(const retro3d::Array< mmlVector<3> > &vertex_pos, const retro3d::Array< mmlVector<2> > &tcoord, const retro3d::Array< mmlVector<3> > &normal, const retro3d::Array< retro3d::Material > &materials)
{
	// NOTE: Another way of splitting...
	// Idea:
		// Generate a convex hull (C) based on the input hull (I)
		// If there are points that are not included on C (i.e. points inside C) do:
			// >= 3 points inside C: use 3 points to generate a splitting plane and split I (or maybe C?).
			// < 3 points inside C: use available points inside C and together point(s) on C to generate splitting plane and split I (or maybe C?)
		// Apply algorithm recursively to the two new parts of I.

	ModelData mdata;
	Convert(mdata, vertex_pos, tcoord, normal, materials);

	Split(mdata, mdata);

	FromModelData(mdata);

	CopyMaterials(materials);
}

void retro3d::DisplayModel::Destroy( void )
{
	m_data.Free();
	m_p_size     = 0;
	m_v_size     = 0;
	m_pdata      = nullptr;
	m_vdata      = nullptr;
	m_depth      = 0;
	m_node_count = 0;
	m_poly_count = 0;
	m_tri_count  = 0;
	m_vert_count = 0;
}

uint32_t retro3d::DisplayModel::GetPolyCount( void ) const
{
	return m_poly_count;
}

uint32_t retro3d::DisplayModel::GetTriCount( void ) const
{
	return m_tri_count;
}

uint32_t retro3d::DisplayModel::GetVertexCount( void ) const
{
	return m_vert_count;
}

uint32_t retro3d::DisplayModel::GetMaterialCount( void ) const
{
	return uint32_t(m_materials.GetSize());
}

retro3d::AABB retro3d::DisplayModel::GetAABB( void ) const
{
	return m_pdata != nullptr ? GetGeometryNode()->aabb : retro3d::AABB(mmlVector<3>::Fill(0.0f), mmlVector<3>::Fill(0.0f));
}

float retro3d::DisplayModel::CalculateArea( void ) const
{
	return 0.0f;
}

float retro3d::DisplayModel::CalculateVolume( void ) const
{
	return 0.0f;
}

void retro3d::DisplayModel::ApplyTransform(const mmlMatrix<4,4> &transform)
{
	const mmlMatrix<3,3> normal_transform = mmlRotation(transform);

	Polygon *poly = rw_GetPolygons();
	for (uint32_t i = 0; i < m_poly_count; ++i) {
		Vertex *vert = poly->GetVerts();
		for (uint32_t j = 0; j < poly->vert_count; ++j) {
			vert->v *= transform;
			vert->n *= normal_transform;
		}
		poly = poly->GetNext();
	}
}

void retro3d::DisplayModel::ApplyOriginCentering( void )
{
	// move model to origin (get data from AABB)
}

void retro3d::DisplayModel::ApplyUnitScaling( void )
{
	// scale model so that length = 1 (get data from AABB)
}

void retro3d::DisplayModel::ApplyInversion( void )
{
	Polygon *poly = rw_GetPolygons();
	for (uint32_t i = 0; i < m_poly_count; ++i) {
		DisplayModel::Vertex *vert = poly->GetVerts();
		const uint32_t half_verts = poly->vert_count / 2;
		for (uint32_t j = 0; j < half_verts; ++j) {
			mmlSwap(vert[j], vert[poly->vert_count - j - 1]);
		}
		for (uint32_t j = 0; j < poly->vert_count; ++j) {
			vert[j].n = -vert[j].n;
		}
		poly = poly->GetNext();
	}

	GeometryNode *node = rw_GetGeometryNode();
	for (uint32_t i = 0; i < m_node_count; ++i) {
		node->node_normal = -node->node_normal;
		mmlSwap(node->front_node_index, node->back_node_index);
		node = node->GetNext();
	}
}

const retro3d::DisplayModel::GeometryNode *retro3d::DisplayModel::GetGeometryNode( void ) const
{
	return reinterpret_cast<const GeometryNode*>(m_pdata);
}

const retro3d::DisplayModel::Polygon *retro3d::DisplayModel::GetPolygons( void ) const
{
	return reinterpret_cast<const Polygon*>(m_vdata);
}

void retro3d::DisplayModel::BuildViewOrder(const mmlVector<3> &view_normal, retro3d::DisplayModel::GeometryQueue &queue) const
{
	queue.Create(m_node_count, m_depth);

	const GeometryNode *current_node = GetGeometryNode(); // the root node
	while (current_node != nullptr || queue.GetStackCount() > 0) {
		while (current_node != nullptr) {
			const float DOT = mmlDot(current_node->node_normal, view_normal);
			if (DOT < 0.0f) {
				queue.PushStack(current_node, retro3d::Contain_Full, GeometryQueue::Back);
				current_node = current_node->GetBackNode();
			} else {
				queue.PushStack(current_node, retro3d::Contain_Full, GeometryQueue::Front);
				current_node = current_node->GetFrontNode();
			}
		}

		current_node = queue.TopStack()->node;
		queue.Push(current_node);

		if (queue.TopStack()->dir == GeometryQueue::Back) {
			current_node = current_node->GetFrontNode();
		} else {
			current_node = current_node->GetBackNode();
		}
		queue.PopStack();
	}

	// Reference in-order traversal
//	std::vector<const BinTree::Node*> stack;
//	std::vector<const BinTree::Node*> queue;
//	while (n != nullptr || stack.empty() == false) {
//		while (n != nullptr) {
//			stack.push_back(n);
//			n = n->left;
//		}
//
//		n = stack.back();
//		stack.pop_back();
//
//		queue.push_back(n);
//
//		n = n->right;
//	}
//
//	for (size_t i = 0; i < queue.size(); ++i) {
//		std::cout << queue[i]->value << ",";
//	}
//
//	return queue;
}

retro3d::DisplayModel::GeometryQueue retro3d::DisplayModel::BuildViewOrder(const mmlVector<3> &view_normal) const
{
	GeometryQueue queue;
	BuildViewOrder(view_normal, queue);
	return queue;
}

void retro3d::DisplayModel::BuildViewOrder(const retro3d::Frustum &view_frustum, retro3d::DisplayModel::GeometryQueue &queue) const
{
	queue.Create(m_node_count, m_depth);

	const mmlVector<3>  view_normal  = view_frustum.GetViewDirection();
	const GeometryNode *current_node = GetGeometryNode();
	retro3d::Contain    current_vis  = retro3d::Contain_Partial;
	while (true) {
		while (current_node != nullptr) {
			if (current_vis != retro3d::Contain_Full) { // If current_vis == Contain_Full then all child nodes are visible too, so there is no need to test the child nodes.
				current_vis = view_frustum.Contains(current_node->aabb);
			}
			if (current_vis != retro3d::Contain_False) { // If current_vis == Contain_False then all child nodes are out of view too, so there is no need to traverse those nodes.
				const float DOT = mmlDot(current_node->node_normal, view_normal);
				if (DOT < 0.0f) {
					queue.PushStack(current_node, current_vis, GeometryQueue::Back);
					current_node = current_node->GetBackNode();
				} else {
					queue.PushStack(current_node, current_vis, GeometryQueue::Front);
					current_node = current_node->GetFrontNode();
				}
			} else {
				current_node = nullptr;
			}
		}

		if (current_node == nullptr && queue.GetStackCount() == 0) { break; }

		current_node = queue.TopStack()->node;
		current_vis  = queue.TopStack()->vis;
		queue.Push(current_node);

		if (queue.TopStack()->dir == GeometryQueue::Back) {
			current_node = current_node->GetFrontNode();
		} else {
			current_node = current_node->GetBackNode();
		}
		queue.PopStack();
	}
}

retro3d::DisplayModel::GeometryQueue retro3d::DisplayModel::BuildViewOrder(const retro3d::Frustum &view_frustum) const
{
	GeometryQueue queue;
	BuildViewOrder(view_frustum, queue);
	return queue;
}

const retro3d::DisplayModel::Material &retro3d::DisplayModel::GetMaterial(uint32_t index) const
{
	return m_materials[index];
}

uint32_t retro3d::DisplayModel::GetPartitionDepth( void ) const
{
	return m_depth;
}

uint32_t retro3d::DisplayModel::GetGeometryNodeCount( void ) const
{
	return m_node_count;
}
