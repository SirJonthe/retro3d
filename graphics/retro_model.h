#ifndef RETRO_MODEL_H
#define RETRO_MODEL_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include "../common/retro_geom.h"
#include "../common/retro_assets.h"
#include "retro_texture.h"
#include "../common/retro_containers.h"

namespace retro3d
{

struct IndexVTN
{
	int32_t v;
	int32_t t;
	int32_t n;
};

struct IndexVN
{
	int32_t v;
	int32_t n;
};

struct IndexV
{
	int32_t v;
};

typedef retro3d::Array<retro3d::IndexVTN> FaceIndex;
typedef retro3d::Array<retro3d::IndexVN>  FaceIndexVN;
typedef retro3d::Array<retro3d::IndexV>   FaceIndexV;

void Convert(const retro3d::IndexVTN &src, retro3d::IndexV &dst);
void Convert(const retro3d::IndexVTN &src, retro3d::IndexVN &dst);
void Convert(const retro3d::IndexV &src, retro3d::IndexVTN &dst);
void Convert(const retro3d::IndexV &src, retro3d::IndexVN &dst);
void Convert(const retro3d::IndexVN &src, retro3d::IndexVTN &dst);
void Convert(const retro3d::IndexVN &src, retro3d::IndexV &dst);

void Convert(const retro3d::FaceIndex &src, retro3d::FaceIndexV &dst);
void Convert(const retro3d::FaceIndex &src, retro3d::FaceIndexVN &dst);
void Convert(const retro3d::FaceIndexV &src, retro3d::FaceIndex &dst);
void Convert(const retro3d::FaceIndexV &src, retro3d::FaceIndexVN &dst);
void Convert(const retro3d::FaceIndexVN &src, retro3d::FaceIndex &dst);
void Convert(const retro3d::FaceIndexVN &src, retro3d::FaceIndexV &dst);

void Convert(const retro3d::Array< retro3d::FaceIndex > &src, retro3d::Array< retro3d::FaceIndexV > &dst);
void Convert(const retro3d::Array< retro3d::FaceIndex > &src, retro3d::Array< retro3d::FaceIndexVN > &dst);
void Convert(const retro3d::Array< retro3d::FaceIndexV > &src, retro3d::Array< retro3d::FaceIndex > &dst);
void Convert(const retro3d::Array< retro3d::FaceIndexV > &src, retro3d::Array< retro3d::FaceIndexVN > &dst);
void Convert(const retro3d::Array< retro3d::FaceIndexVN > &src, retro3d::Array< retro3d::FaceIndex > &dst);
void Convert(const retro3d::Array< retro3d::FaceIndexVN > &src, retro3d::Array< retro3d::FaceIndexV > &dst);


struct Material : public retro3d::Asset<Material>
{
	std::string                        name;
	retro3d::Array<retro3d::FaceIndex> f;
	mmlVector<3>                       cd;
	mtlShared<retro3d::Texture>        td;
};

struct Model : public retro3d::Asset<Model>
{
	std::string                        name;
	retro3d::Array< mmlVector<3> >     v; // NOTE, TODO: v, t, n *might* benefit from being ordered in a Hamiltonian cycle to preserve spatial locality (???).
	retro3d::Array< mmlVector<2> >     t;
	retro3d::Array< mmlVector<3> >     n;
	retro3d::Array<retro3d::Material>  m;
	retro3d::Array<retro3d::FaceIndex> f;
	mtlShared<retro3d::Texture>        lightmap;
	retro3d::AABB                      aabb;

	// NOTE: When lightmap is available (i.e. not null):
	//  1) empty normal array
	//  2) use the 'n' index to refer to lightmap uvs
	//  3) store lightmap uvs in texture coordinate array (append last to array)

	void     CenterModel( void );
	void     ScaleModelToUnitSize( void );
	void     ReverseModel( void );
	void     DoubleSideModel( void );
	void     GenerateVertexNormals( void );
	void     GenerateFaceNormals( void );
	uint32_t GetPolyCount( void ) const;
	uint32_t GetTriCount( void ) const;
	float    CalculateSurfaceArea( void ) const;
	// float CalculateVolume( void ) const;
	void     ApplyTransform(const retro3d::Transform &transform);
	void     AppendModel(const retro3d::Model &model, const retro3d::Transform &transform);
	void     RefreshConnectivity( void );
	void     DefragIndex( void );
};

// LoadOBJ -> Returns a series of ModelObjects depending on number of 'o' in OBJ file.

// Vertex
void         TransformVertices(retro3d::Array< mmlVector<3> > &v, const retro3d::Transform &t);
void         TranslateVertices(retro3d::Array< mmlVector<3> > &v, const mmlVector<3> &t);
void         TranslateVertices(retro3d::Array< mmlVector<3> > &v, int32_t axis, float t);
void         CenterVerticesByMass(retro3d::Array< mmlVector<3> > &v, const mmlVector<3> &new_center);
void         CenterVerticesByVolume(retro3d::Array< mmlVector<3> > &v, const mmlVector<3> &new_center);
float        CalculateVertexScale(const retro3d::Array< mmlVector<3> > &v);
void         ScaleVertices(retro3d::Array< mmlVector<3> > &v, float scale);
mmlVector<3> CalculateCentroid(const retro3d::Array< mmlVector<3> > &v);
mmlVector<3> CalculateCenterOfMass(const retro3d::Array< mmlVector<3> > &v);
mmlVector<3> CalculateCenterOfVolume(const retro3d::Array< mmlVector<3> > &v);
mmlVector<3> FindExtremeVertexAlongDirection(const retro3d::Array< mmlVector<3> > &v, const mmlVector<3> &dir);
int          FindExtremeVertexIndexAlongDirection(const retro3d::Array< mmlVector<3> > &v, const mmlVector<3> &dir);
bool         CalculatePointInConvexHull(const retro3d::Array< mmlVector<3> > &hull_v, const mmlVector<3> &p);
void         ExtractFaceVertices(const retro3d::Array< mmlVector<3> > &v, const retro3d::FaceIndex &f, retro3d::Array< mmlVector<3> > &out);
void         ExtractFaceVertices(const retro3d::Array< mmlVector<3> > &v, const retro3d::FaceIndexV &f, retro3d::Array< mmlVector<3> > &out);
void         ExtractFaceVertices(const retro3d::Array< mmlVector<3> > &v, const retro3d::FaceIndexVN &f, retro3d::Array< mmlVector<3> > &out);
void         InsertFaceVertices(retro3d::Array< mmlVector<3> > &v, const retro3d::FaceIndex &f, const retro3d::Array< mmlVector<3> > &in);
void         InsertFaceVertices(retro3d::Array< mmlVector<3> > &v, const retro3d::FaceIndexV &f, const retro3d::Array< mmlVector<3> > &in);
void         InsertFaceVertices(retro3d::Array< mmlVector<3> > &v, const retro3d::FaceIndexVN &f, const retro3d::Array< mmlVector<3> > &in);

// General
enum Align { Align_Left, Align_Center, Align_Right };
template < int n > mmlVector<n> CalculateMin(const retro3d::Array< mmlVector<n> > &v) {
	mmlVector<n> m = v[0];
	for (int i = 0; i < v.GetSize(); ++i) {
		m = mmlMin(m, v[i]);
	}
	return m;
}
template < int n > float CalculateMin(const retro3d::Array< mmlVector<n> > &v, int32_t axis) {
	float m = v[0][axis];
	for (int i = 0; i < v.GetSize(); ++i) {
		m = mmlMin(m, v[i][axis]);
	}
	return m;
}
template < int n > mmlVector<n> CalculateMax(const retro3d::Array< mmlVector<n> > &v) {
	mmlVector<n> m = v[0];
	for (int i = 0; i < v.GetSize(); ++i) {
		m = mmlMax(m, v[i]);
	}
	return m;
}
template < int n > float CalculateMax(const retro3d::Array< mmlVector<n> > &v, int32_t axis) {
	float m = v[0][axis];
	for (int i = 0; i < v.GetSize(); ++i) {
		m = mmlMax(m, v[i][axis]);
	}
	return m;
}
template < int n > void FlipAxis(retro3d::Array< mmlVector<n> > &v, int32_t axis)                       { for (int i = 0; i < v.GetSize(); ++i) { v[i][axis] = -v[i][axis]; } }
template < int n > void FlipAxis(retro3d::Array< mmlVector<n> > &v)                                     { for (int i = 0; i < v.GetSize(); ++i) { v[i] = -v[i]; } }
template < int n > void AlignAxis(retro3d::Array< mmlVector<n> > &v, int32_t axis, retro3d::Align mode) {
	float t = 0.0f;
	switch (mode) {
	case Align_Left:
		t = retro3d::CalculateMin(v, axis);
		break;
	case Align_Center: {
		const float m0 = retro3d::CalculateMin(v, axis);
		t = (retro3d::CalculateMax(v, axis) - m0) / 2.0f + m0;
		}
		break;
	case Align_Right:
		t = retro3d::CalculateMax(v, axis);
		break;
	}
	retro3d::TranslateVertices(v, axis, t);
}

// Face
void         InvertFaces(retro3d::Array< FaceIndex > &f);
void         InvertFaces(retro3d::Array< FaceIndexVN > &f);
void         InvertFaces(retro3d::Array< FaceIndexV > &f);

// Normals
void         InvertNormals(retro3d::Array< mmlVector<3> > &n);
void         CalculateVertexNormals(const retro3d::Array< mmlVector<3> > &v, retro3d::Array< retro3d::Material > &m, retro3d::Array< mmlVector<3> > &out_normals);
void         CalculateVertexNormals(const retro3d::Array< mmlVector<3> > &v, retro3d::Material &m, retro3d::Array< mmlVector<3> > &out_normals);
void         CalculateFaceNormals(const retro3d::Array< mmlVector<3> > &v, retro3d::Array< retro3d::Material > &m, retro3d::Array< mmlVector<3> > &out_normals);
void         CalculateFaceNormals(const retro3d::Array< mmlVector<3> > &v, retro3d::Material &m, retro3d::Array< mmlVector<3> > &out_normals);
void         ExtractFaceNormals(const retro3d::Array< mmlVector<3> > &n, const retro3d::FaceIndex &f, retro3d::Array< mmlVector<3> > &out);
void         ExtractFaceNormals(const retro3d::Array< mmlVector<3> > &n, const retro3d::FaceIndexVN &f, retro3d::Array< mmlVector<3> > &out);
void         InsertFaceNormals(retro3d::Array< mmlVector<3> > &n, const retro3d::FaceIndex &f, const retro3d::Array< mmlVector<3> > &in);
void         InsertFaceNormal(retro3d::Array< mmlVector<3> > &n, const retro3d::FaceIndexVN &f, const retro3d::Array< mmlVector<3> > &in);

// Texture coords
void         ExtractFaceTCoords(const retro3d::Array< mmlVector<2> > &t, const retro3d::FaceIndex &f, retro3d::Array< mmlVector<2> > &out);
void         InsertFaceTCoords(retro3d::Array< mmlVector<2> > &t, const retro3d::FaceIndex &f, const retro3d::Array< mmlVector<2> > &in);

// Topography
bool         PointInsideConvexHull(const mmlVector<3> &point, const retro3d::Array< mmlVector<3> > &v, const retro3d::Array< retro3d::FaceIndex > &convex_hull, float FP_EPSILON = std::numeric_limits<float>::epsilon());
bool         PointInsideConvexHull(const mmlVector<3> &point, const retro3d::Array< mmlVector<3> > &v, const retro3d::Array< retro3d::FaceIndexV > &convex_hull, float FP_EPSILON = std::numeric_limits<float>::epsilon());
bool         PointInsideConvexHull(const mmlVector<3> &point, const retro3d::Array< mmlVector<3> > &v, const retro3d::Array< retro3d::FaceIndexVN > &convex_hull, float FP_EPSILON = std::numeric_limits<float>::epsilon());
bool         IsConvex(const retro3d::Array< mmlVector<3> > &v, const retro3d::Array< retro3d::FaceIndexV > &f, float FP_EPSILON = std::numeric_limits<float>::epsilon());
bool         IsConvex(const retro3d::Array< mmlVector<3> > &v, const retro3d::Array< retro3d::FaceIndexVN > &f, float FP_EPSILON = std::numeric_limits<float>::epsilon());
bool         IsConvex(const retro3d::Array< mmlVector<3> > &v, const retro3d::Array< retro3d::FaceIndex > &f, float FP_EPSILON = std::numeric_limits<float>::epsilon());
void         CreateConvexHull(const retro3d::Array< mmlVector<3> > &vert_cloud, retro3d::Array< mmlVector<3> > *hull_verts, retro3d::Array< retro3d::FaceIndexV > *outer_hull_faces);
void         CreateConvexHull(const retro3d::Array< mmlVector<3> > &vert_cloud, retro3d::Array< mmlVector<3> > &hull_verts, retro3d::Array< retro3d::FaceIndexV > &outer_hull_faces);
void         CreateConvexHull(const retro3d::Array< mmlVector<3> > &vert_cloud, retro3d::Array< mmlVector<3> > &hull_verts);
void         CreateConvexHull(const retro3d::Array< mmlVector<3> > &vert_cloud, retro3d::Array< retro3d::FaceIndexV > &outer_hull_faces);
void         FindConcavePoints(const retro3d::Array< mmlVector<3> > &vert_cloud, retro3d::Array< int32_t > &out_index);
void         FindConcavePoints(const retro3d::Array< mmlVector<3> > &vert_cloud, retro3d::Array< mmlVector<3> > &out_vert);

/*struct ModelExchange
{
	retro3d::Array< mmlVector<3> > verts;
	retro3d::Array< mmlVector<2> > tcoords;
	retro3d::Array< mmlVector<3> > normals;
	retro3d::Array< Surface > surfaces;
};*/

class GeometryEditor
{
private:
	template < typename type_t >
	using Map = std::unordered_map<uint64_t, type_t >;

	static constexpr uint32_t NO_INDEX = -uint32_t(1);
	struct IndexMap { uint64_t v, t, n; };
	struct FaceMap  { retro3d::Array<IndexMap> i; };
	struct MaterialMap
	{
		std::string                 name;
		Map<FaceMap>                f;
		mmlVector<3>                cd;
		mtlShared<retro3d::Texture> td;
	};

private:
	std::string                 name;
	Map< mmlVector<3> >         v;
	Map< mmlVector<2> >         t;
	Map< mmlVector<3> >         n;
	Map<MaterialMap>            m;
	Map<FaceMap>                f;
	mtlShared<retro3d::Texture> lightmap;
	retro3d::AABB               aabb;

private:
	static uint64_t Hash(int32_t a);
	static uint64_t Hash(int32_t a, int32_t b);
	void            StoreClippedFace(GeometryEditor *out, const FaceMap &unclipped_index, int32_t current_material_index, const retro3d::Array< mmlVector<3> > &clipped_v, const retro3d::Array< mmlVector<2> > &clipped_t, const retro3d::Array< mmlVector<3> > &clipped_n);
	void            UpdateAABB( void );

public:
//	void ExtractMaterial(retro3d::Model &out);
	void ToModel(retro3d::Model &out);
	void FromModel(const retro3d::Model &model, const retro3d::Transform &transform = mmlMatrix<4,4>::Identity());
	void MergeApproximateVertices(const float DIST = std::numeric_limits<float>::epsilon());
	void MergeIdenticalVertices( void );
	void AxisSubdivide(const float AXIS_SIZE = 1.0f, const float FP_EPSILON = std::numeric_limits<float>::epsilon());
//	void SeparateFaces( void ); // NOTE: Store vertices in faces separately from eachother
//	void FlipFaces( void );
	void DefragAttributes( void );
//	void ExtractMaterial();
//	void InsertMaterial();
	void Destroy( void );
	void Split(const retro3d::Plane &plane, retro3d::GeometryEditor *front, retro3d::GeometryEditor *back, const float FP_EPSILON = std::numeric_limits<float>::epsilon());
	bool CalculateConvexity(const float FP_EPSILON = std::numeric_limits<float>::epsilon()) const;
	void Debug_PrintMaterials( void ) const;
	void AppendModel(const retro3d::Model &model, const retro3d::Transform &transform = mmlMatrix<4,4>::Identity());
	void AppendGeometry(const retro3d::GeometryEditor &geom, const retro3d::Transform &transform = mmlMatrix<4,4>::Identity());
};

class DisplayModel : public retro3d::Asset<DisplayModel>
{
public:
	struct Vertex
	{
		mmlVector<3> v;
		mmlVector<2> t;
		union {
			mmlVector<3> n;  // normal
			mmlVector<3> c;  // color
			mmlVector<2> lt; // lightmap uv
		};

		const Vertex *GetNext( void ) const { return this + 1; }
		Vertex       *GetNext( void )       { return this + 1; }

		Vertex( void ) : v(mmlVector<3>::Fill(0.0f)), t(mmlVector<2>::Fill(0.0f)), n(retro3d::Transform::GetWorldForward()) {}
		Vertex(const DisplayModel::Vertex &vtx) : v(vtx.v), t(vtx.t), n(vtx.n) {}
		DisplayModel::Vertex &operator=(const DisplayModel::Vertex &vtx) {
			if (this != &vtx) {
				v = vtx.v;
				t = vtx.t;
				n = vtx.n;
			}
			return *this;
		}

		DisplayModel::Vertex operator+(const DisplayModel::Vertex &vtx) const {
			DisplayModel::Vertex out;
			out.v = v + vtx.v;
			out.t = t + vtx.t;
			out.n = n + vtx.n;
			return out;
		}

		DisplayModel::Vertex operator-(const DisplayModel::Vertex &vtx) const {
			DisplayModel::Vertex out;
			out.v = v - vtx.v;
			out.t = t - vtx.t;
			out.n = n - vtx.n;
			return out;
		}

		DisplayModel::Vertex operator*(float s) const {
			DisplayModel::Vertex out;
			out.v = v * s;
			out.t = t * s;
			out.n = n * s;
			return out;
		}
	};

	struct Polygon
	{
		uint32_t vert_count;

		// The data following this will be 'vert_count' Vertex structs.

		const Vertex *GetVerts( void ) const { return reinterpret_cast<const Vertex*>(reinterpret_cast<const uint8_t*>(this) + sizeof(Polygon)); }
		Vertex       *GetVerts( void )       { return reinterpret_cast<Vertex*>(reinterpret_cast<uint8_t*>(this) + sizeof(Polygon)); }

		const Polygon *GetNext( void ) const { return reinterpret_cast<const Polygon*>(GetVerts() + vert_count); }
		Polygon       *GetNext( void )       { return reinterpret_cast<Polygon*>(GetVerts() + vert_count); }
	};

	// A group of polygons with the same material
	struct Surface
	{
		uint32_t  material_index;
		uint32_t  poly_count;

		const Surface *GetNext( void ) const { return this + 1; }
		Surface       *GetNext( void )       { return this + 1; }
	};

	// A group of polygon groups with indices to other polygon groups
	struct GeometryNode
	{
		uint32_t      front_node_index; // relative index of front node in m_pdata (in bytes, (byte*)this + front_node_index = front_node)
		uint32_t      back_node_index;  // relative index of back node in m_pdata (in bytes, (byte*)this + back_node_index = back_node)
		uint32_t      surface_count;    // number of material groups
		uint32_t      poly_index;       // relative index of first group's first polygon in m_vdata (in bytes, (byte*)this + poly_index = poly)
		uint32_t      poly_count;       // number of polygons in this node
		mmlVector<3>  node_normal;      // dot(node_normal, view_normal) decides which way we traverse this tree
		retro3d::AABB aabb;             // contains all of the current and child geometry

		bool IsLeaf( void ) const { return front_node_index == 0 && back_node_index == 0; }

		const Surface *GetSurfaces( void ) const { return reinterpret_cast<const Surface*>(reinterpret_cast<const uint8_t*>(this) + sizeof(GeometryNode)); }
		Surface       *GetSurfaces( void )       { return reinterpret_cast<Surface*>(reinterpret_cast<uint8_t*>(this) + sizeof(GeometryNode)); }

		const GeometryNode *GetNext( void ) const { return reinterpret_cast<const GeometryNode*>(GetSurfaces() + surface_count); }
		GeometryNode       *GetNext( void )       { return reinterpret_cast<GeometryNode*>(GetSurfaces() + surface_count); }

		const GeometryNode *GetFrontNode( void ) const { return front_node_index != 0 ? reinterpret_cast<const GeometryNode*>(reinterpret_cast<const uint8_t*>(this) + front_node_index) : nullptr; }
		GeometryNode       *GetFrontNode( void )       { return front_node_index != 0 ? reinterpret_cast<GeometryNode*>(reinterpret_cast<uint8_t*>(this) + front_node_index) : nullptr; }

		const GeometryNode *GetBackNode( void ) const { return back_node_index != 0 ? reinterpret_cast<const GeometryNode*>(reinterpret_cast<const uint8_t*>(this) + back_node_index) : nullptr; }
		GeometryNode       *GetBackNode( void )       { return back_node_index != 0 ? reinterpret_cast<GeometryNode*>(reinterpret_cast<uint8_t*>(this) + back_node_index) : nullptr; }

		const Polygon *GetPolygons( void ) const { return poly_index != 0 ? reinterpret_cast<const Polygon*>(reinterpret_cast<const uint8_t*>(this) + poly_index) : nullptr; }
		Polygon       *GetPolygons( void )       { return poly_index != 0 ? reinterpret_cast<Polygon*>(reinterpret_cast<uint8_t*>(this) + poly_index) : nullptr; }

		// The data following this will be 'group_count' MaterialGroup structs.
	};

	struct Material
	{
		mmlVector<3>                   color;
		mtlShared< retro3d::Texture >  texture;
	};

	class GeometryQueue
	{
		friend class DisplayModel;

	private:
		enum Direction { Back, Front };
		struct StackItem
		{
			const GeometryNode *node;
			retro3d::Contain    vis;
			uint8_t             dir;
		};

	private:
		uint8_t             *m_data;
		const GeometryNode **m_items;
		StackItem           *m_stack;
		uint32_t             m_item_count;
		uint32_t             m_max_items;
		uint32_t             m_max_stack;
		uint32_t             m_stack_count;

	private:
		GeometryQueue( void );

		void Create(uint32_t max_items, uint32_t max_stack);
		void Push(const GeometryNode *node);

		void             PushStack(const GeometryNode *node, retro3d::Contain vis, uint8_t dir);
		void             PopStack( void );
		const StackItem *TopStack( void ) const;
		uint32_t         GetStackCount( void ) const;

	public:
		~GeometryQueue( void );
		GeometryQueue(GeometryQueue &&stack);
		GeometryQueue(const GeometryQueue &stack) = delete;
		GeometryQueue &operator=(const GeometryQueue &stack) = delete;

		const GeometryNode *Pop( void );
		const GeometryNode *Peek( void ) const;
		bool                IsEmpty( void ) const;
	};

private:
	// Internal use for splitting the model into halves
	struct ModelData
	{
	public:
		struct Polygon
		{
			retro3d::Array<DisplayModel::Vertex> v;
			mmlVector<3>                         n;
			uint32_t                             m;
		};

		ModelData *back;
		ModelData *front;

		mtlList<ModelData::Polygon> polys;

		// Speed up T-junction control point emission by building connectivity mesh?
		// edges[index_a][index_b] = polygon
		// edges[index_b][index_a] = neighbor

		mtlItem<ModelData::Polygon> *splitting_poly;
		retro3d::Plane               splitting_plane;

	private:
		static uint32_t CalcDepth(const ModelData *n, uint32_t curr_depth)
		{
			if (n == nullptr) { return curr_depth; }
			uint32_t b = CalcDepth(n->back, curr_depth + 1);
			uint32_t f = CalcDepth(n->front, curr_depth + 1);
			return mmlMax(b, f);
		}

	public:
		ModelData( void ) : back(nullptr), front(nullptr), polys() {}
		~ModelData( void ) { delete back; delete front; }

		uint32_t GetDepth( void ) const {
			return CalcDepth(this, 0);
		}
	};

	enum VertexFormat
	{
		VertexFormat_Position = 1,
		VertexFormat_TexCoord = VertexFormat_Position << 1,
		VertexFormat_Normal   = VertexFormat_Position << 2,
		VertexFormat_Color    = VertexFormat_Position << 3
	};

private:
	retro3d::Array<uint8_t>  m_data; // A master array containing all dynamically allocated data for the model.
	uint32_t                 m_p_size; // Size in bytes
	uint32_t                 m_v_size; // Size in bytes

	uint8_t                 *m_pdata;      // Polygon data, containing GeometryNode followed by a specified amount of MaterialGroup headers (stored breadth-first)
	uint32_t                 m_depth;
	uint32_t                 m_node_count;

	uint8_t                 *m_vdata; // polygon header and vertices stored linearly [Polygon]+[Vertex]*vertex_count
	uint32_t                 m_poly_count;
	uint32_t                 m_tri_count;
	uint32_t                 m_vert_count;

	retro3d::Array<Material>  m_materials; // (change this to pointer to m_data)

	float  m_volume;
	float  m_area;

	uint64_t  m_vertex_format; // Let's just assume Position|TexCoord|Normal for now...

private:
	static void    Convert(ModelData &out, const retro3d::Array< mmlVector<3> > &vertex_pos, const retro3d::Array< mmlVector<2> > &tcoord, const retro3d::Array< mmlVector<3> > &normal, const retro3d::Array< retro3d::Material > &material);
	static bool    IsConvex(const ModelData &model_data);
	static bool    FindSplittingPoly(ModelData &model_data);
	static void    FixTJunction(ModelData *node, const ModelData::Polygon *div_poly, const mmlVector<3> &v1, const mmlVector<3> &v2, const mmlVector<3> &vx, float x);
	static void    Split(ModelData &model_data, ModelData &root);
	static void    ChunkySplit(ModelData &model_data, ModelData &root, retro3d::Array< mmlVector<3> > &concave_points);
	void           AllocateSpace(const ModelData &root);
	void           BreadthFirstWrite(const ModelData &root);
	void           UpdateAABBs(retro3d::DisplayModel::GeometryNode *geom_node);
	void           FromModelData(const ModelData &root);
	void           FromModelData(const ModelData *leaf, uint8_t *topo_data, uint8_t *poly_data);
	void           CopyMaterials(const retro3d::Array< retro3d::Material > &materials);
	GeometryNode  *rw_GetGeometryNode( void );
	Polygon       *rw_GetPolygons( void );

public:
	DisplayModel( void );

	void     CreateGeometryNode(const retro3d::Array< mmlVector<3> > &vertex_pos, const retro3d::Array< mmlVector<2> > &tcoord, const retro3d::Array< mmlVector<3> > &normal, const retro3d::Array< retro3d::Material > &material);
	void     CreateGeometryTree(const retro3d::Array< mmlVector<3> > &vertex_pos, const retro3d::Array< mmlVector<2> > &tcoord, const retro3d::Array< mmlVector<3> > &normal, const retro3d::Array< retro3d::Material > &material);
	void     Destroy( void );
	uint32_t GetPolyCount( void ) const;
	uint32_t GetTriCount( void ) const;
	uint32_t GetVertexCount( void ) const;
	uint32_t GetMaterialCount( void ) const;
	AABB     GetAABB( void ) const;
	float    CalculateArea( void ) const;
	float    CalculateVolume( void ) const;
	void     ApplyTransform(const mmlMatrix<4,4> &transform);
	void     ApplyOriginCentering( void );
	void     ApplyUnitScaling( void );
	void     ApplyInversion( void );

	const GeometryNode *GetGeometryNode( void ) const;
	const Polygon      *GetPolygons( void ) const;

	void          BuildViewOrder(const mmlVector<3> &view_normal, GeometryQueue &out) const;
	GeometryQueue BuildViewOrder(const mmlVector<3> &view_normal) const;
	void          BuildViewOrder(const retro3d::Frustum &view_frustum, GeometryQueue &out) const;
	GeometryQueue BuildViewOrder(const retro3d::Frustum &view_frustum) const;

	const Material &GetMaterial(uint32_t index) const;

	uint32_t GetPartitionDepth( void ) const;
	uint32_t GetGeometryNodeCount( void ) const;
};

}

#endif // RETRO_MODEL_H
