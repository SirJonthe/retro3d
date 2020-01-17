#include "../common/MiniLib/MGL/mglCollision.h"
#include "retro_collider.h"

retro3d::Ray::Ray( void ) :
	origin(mmlVector<3>::Fill(0.0f)), direction(retro3d::Transform::GetWorldForward()), length(-1.0f)
{}

retro3d::Ray::Ray(const mmlVector<3> &p_origin, const mmlVector<3> &p_direction, float p_length) :
	origin(p_origin), direction(p_direction), length(p_length)
{}

retro3d::Ray retro3d::Ray::ApplyTransform(const retro3d::Transform &transform) const
{
	Ray r;
	r.origin    = origin * transform;
	r.direction = direction * transform.GetRotation();
	return r;
}

bool retro3d::Collider::AABB_DetectCollision(const retro3d::Collider &a, const retro3d::Ray &r, retro3d::Ray::Contact *contact_info)
{
	const retro3d::AABB world_aabb = a.m_obj_aabb.ApplyTransform(a.GetTransform());
	const mmlVector<3> aabb_min = world_aabb.GetMin();
	const mmlVector<3> aabb_max = world_aabb.GetMax();
	bool hit = mglCollision::Ray_AABB(r.origin, r.direction, aabb_min, aabb_max, contact_info);
	if (hit == true && contact_info != nullptr && r.length > 0.0f) {
		if (contact_info->entry.distance > r.length) {
			hit = false;
		} else if (contact_info->exit.distance > r.length) {
			// clip exit point
			contact_info->exit.point    = r.origin;
			contact_info->exit.normal   = r.direction;
			contact_info->exit.distance = r.length;
		}
	}

	return hit;
}

bool retro3d::Collider::AABB_DetectCollision(const retro3d::Collider &a, const retro3d::Collider &b)
{
	const AABB a_box = a.m_obj_aabb.ApplyTransform(a.GetTransform());
	const AABB b_box = b.m_obj_aabb.ApplyTransform(b.GetTransform());
	return a_box.Overlaps(b_box) == true;
}

struct Simplex
{
	mmlVector<3> a, b, c, d;
	uint32_t dimension;
};

constexpr int32_t GJK_MAX_NUM_ITERATIONS = 64;

void update_simplex3(Simplex &s, mmlVector<3> &search_dir)
{
	// Required winding order:
	//  b
	//  |\
	//  | \
	//  |  a
	//  | /
	//  |/
	//  c
	mmlVector<3> n = mmlCross(s.b - s.a, s.c - s.a); //triangle's normal
	mmlVector<3> AO = -s.a; //direction to origin

	//Determine which feature is closest to origin, make that the new simplex

	s.dimension = 2;
	if (mmlDot(mmlCross(s.b - s.a, n), AO) > 0.0f) { //Closest to edge AB
		s.c = s.a;
		search_dir = mmlCross(mmlCross(s.b - s.a, AO), s.b - s.a);
		return;
	}
	if (mmlDot(mmlCross(n, s.c - s.a), AO) > 0.0f) { //Closest to edge AC
		s.b = s.a;
		search_dir = mmlCross(mmlCross(s.c - s.a, AO), s.c - s.a);
		return;
	}

	s.dimension = 3;
	if (mmlDot(n, AO) > 0.0f) { //Above triangle
		s.d = s.c;
		s.c = s.b;
		s.b = s.a;
		search_dir = n;
		return;
	}
	//else //Below triangle
	s.d = s.b;
	s.b = s.a;
	search_dir = -n;
	return;
}

bool update_simplex4(Simplex &s, mmlVector<3> &search_dir)
{
	// Simplex::a is the peak/tip of the pyramid, BCD is the base (counterclockwise winding order).
	// Going into this function we already know origin is below A and above BCD.

	// Get normals of three new faces.
	mmlVector<3> ABC = mmlCross(s.b - s.a, s.c - s.a);
	mmlVector<3> ACD = mmlCross(s.c - s.a, s.d - s.a);
	mmlVector<3> ADB = mmlCross(s.d - s.a, s.b - s.a);

	mmlVector<3> AO = -s.a; // Direction to origin.
	s.dimension = 3; // Hoisting this just cause (?)

	//Plane-test origin with 3 faces
	// Note: Kind of primitive approach used here; If origin is in front of a face, just use it as the new simplex.
	// We just go through the faces sequentially and exit at the first one which satisfies dot product. Not sure this
	// is optimal or if edges should be considered as possible simplices? Thinking this through in my head I feel like
	// this method is good enough. Makes no difference for AABBS, should test with more complex colliders.
	if (mmlDot(ABC, AO) > 0.0f) { //In front of ABC
		s.d = s.c;
		s.c = s.b;
		s.b = s.a;
		search_dir = ABC;
		return false;
	}
	if (mmlDot(ACD, AO) > 0.0f) { // In front of ACD
		s.b = s.a;
		search_dir = ACD;
		return false;
	}
	if (mmlDot(ADB, AO) > 0.0f) { // In front of ADB
		s.c = s.d;
		s.d = s.b;
		s.b = s.a;
		search_dir = ADB;
		return false;
	}

	// Point is inside simplex.
	return true;
}

// Expanding Polytope Algorithm
// Find minimum translation vector to resolve collision.
constexpr float EPA_TOLERANCE = 0.0001f;
constexpr int EPA_MAX_NUM_FACES = 64;
constexpr int EPA_MAX_NUM_LOOSE_EDGES = 32;
constexpr int EPA_MAX_NUM_ITERATIONS = 64;

mmlVector<3> EPA(const Simplex &s, const retro3d::Collider &coll1, const retro3d::Collider &coll2, mmlVector<3> &avg_col_point)
{
	// Finding the contact point:
	// http://hacktank.net/blog/?p=119

	mmlVector<3> faces[EPA_MAX_NUM_FACES][4]; //Array of faces, each with 3 verts and a normal

	// Initialize with final output simplex from GJK.
	faces[0][0] = s.a;
	faces[0][1] = s.b;
	faces[0][2] = s.c;
	faces[0][3] = mmlNormalize(mmlCross(s.b - s.a, s.c - s.a)); //ABC
	faces[1][0] = s.a;
	faces[1][1] = s.c;
	faces[1][2] = s.d;
	faces[1][3] = mmlNormalize(mmlCross(s.c - s.a, s.d - s.a)); //ACD
	faces[2][0] = s.a;
	faces[2][1] = s.d;
	faces[2][2] = s.b;
	faces[2][3] = mmlNormalize(mmlCross(s.d - s.a, s.b - s.a)); //ADB
	faces[3][0] = s.b;
	faces[3][1] = s.d;
	faces[3][2] = s.c;
	faces[3][3] = mmlNormalize(mmlCross(s.d - s.b, s.c - s.b)); //BDC

	int num_faces = 4;
	int closest_face = 0;

	for (int iterations = 0; iterations < EPA_MAX_NUM_ITERATIONS; ++iterations) {
		//Find face that's closest to origin
		float min_dist = mmlDot(faces[0][0], faces[0][3]);
		closest_face = 0;
		for (int i = 1; i < num_faces; ++i) {
			float dist = mmlDot(faces[i][0], faces[i][3]);
			if (dist < min_dist) {
				min_dist = dist;
				closest_face = i;
			}
		}

		//search normal to face that's closest to origin
		mmlVector<3> search_dir = faces[closest_face][3];
		mmlVector<3> ae = coll1.FindExtremePoint(-search_dir);
		mmlVector<3> be = coll2.FindExtremePoint(search_dir);
		mmlVector<3> p = be - ae;

		if (mmlDot(p, search_dir) - min_dist < EPA_TOLERANCE) {
			//Convergence (new point is not significantly further from origin)
			avg_col_point = ae;
			return faces[closest_face][3] * mmlDot(p, search_dir); //dot vertex with normal to resolve collision along normal!
		}

		mmlVector<3> loose_edges[EPA_MAX_NUM_LOOSE_EDGES][2]; //keep track of edges we need to fix after removing faces
		int num_loose_edges = 0;

		//Find all triangles that are facing p
		for (int i = 0; i < num_faces; ++i) {
			if (mmlDot(faces[i][3], p - faces[i][0]) > 0.0f) { //triangle i faces p, remove it
				//Add removed triangle's edges to loose edge list.
				//If it's already there, remove it (both triangles it belonged to are gone)
				for (int j = 0; j < 3; ++j) { //Three edges per face
					mmlVector<3> current_edge[2] = {
						faces[i][j],
						faces[i][(j + 1) % 3]
					};
					bool found_edge = false;
					for (int k = 0; k < num_loose_edges; ++k) { //Check if current edge is already in list
						if (loose_edges[k][1] == current_edge[0] && loose_edges[k][0] == current_edge[1]) {
							//Edge is already in the list, remove it
							//THIS ASSUMES EDGE CAN ONLY BE SHARED BY 2 TRIANGLES (which should be true)
							//THIS ALSO ASSUMES SHARED EDGE WILL BE REVERSED IN THE TRIANGLES (which
							//should be true provided every triangle is wound CCW)
							loose_edges[k][0] = loose_edges[num_loose_edges - 1][0]; //Overwrite current edge
							loose_edges[k][1] = loose_edges[num_loose_edges - 1][1]; //with last edge in list
							--num_loose_edges;
							found_edge = true;
							k = num_loose_edges; //exit loop because edge can only be shared once
						}
					}

					if (found_edge == false) { //add current edge to list
						// assert(num_loose_edges<EPA_MAX_NUM_LOOSE_EDGES);
						if (num_loose_edges >= EPA_MAX_NUM_LOOSE_EDGES) {
							break;
						}
						loose_edges[num_loose_edges][0] = current_edge[0];
						loose_edges[num_loose_edges][1] = current_edge[1];
						++num_loose_edges;
					}
				}

				//Remove triangle i from list
				faces[i][0] = faces[num_faces - 1][0];
				faces[i][1] = faces[num_faces - 1][1];
				faces[i][2] = faces[num_faces - 1][2];
				faces[i][3] = faces[num_faces - 1][3];
				--num_faces;
				--i;
			}
		}

		//Reconstruct polytope with p added
		for (int i = 0; i < num_loose_edges; ++i) {
			// assert(num_faces<EPA_MAX_NUM_FACES);
			if(num_faces >= EPA_MAX_NUM_FACES) {
				break;
			}
			faces[num_faces][0] = loose_edges[i][0];
			faces[num_faces][1] = loose_edges[i][1];
			faces[num_faces][2] = p;
			faces[num_faces][3] = mmlNormalize(mmlCross(loose_edges[i][0] - loose_edges[i][1], loose_edges[i][0] - p));

			// Check for wrong normal to maintain CCW winding
//			const float bias = 0.000001f; //in case dot result is only slightly < 0 (because origin is on face)
//			if (mmlDot(faces[num_faces][0], faces[num_faces][3]) + bias < 0.0f) {
			const float dot = mmlDot(faces[num_faces][0], faces[num_faces][3]);
			if (dot < 0.0f && mmlIsApproxEqual(dot, 0.0f, 0.000001f) == false) {
				mmlVector<3> temp = faces[num_faces][0];
				faces[num_faces][0] = faces[num_faces][1];
				faces[num_faces][1] = temp;
				faces[num_faces][3] = -faces[num_faces][3];
			}
			++num_faces;
		}
	}
	std::cout << "EPA did not converge" << std::endl;
	// EPA did not converge. Return most recent closest point.
	avg_col_point = faces[closest_face][0];
	return faces[closest_face][3] * mmlDot(faces[closest_face][0], faces[closest_face][3]);
}

bool retro3d::Collider::GJK_DetectCollision(const retro3d::Collider &a, const retro3d::Collider &b, retro3d::Collider::Contact *contact_info)
{
	// http://hitokageproduction.com/article/11#collisions

	// Try GJK
	// https://en.wikipedia.org/wiki/Gilbert%E2%80%93Johnson%E2%80%93Keerthi_distance_algorithm
	// https://github.com/kevinmoran/GJK/blob/master/GJK.h

	const retro3d::Transform *at = &a.GetTransform();
	const retro3d::Transform *bt = &b.GetTransform();

	Simplex s; // Simplex: A set of a maximum of 4 points (the simplest of 3D shapes). Simplex::a is always the most recently added point.
	mmlVector<3> search_dir = at->GetPosition() - bt->GetPosition(); // Get an initial search direction (can be any direction, but initialized to the direction between colliders).

	// Find an initial simplex point.
	s.c = b.FindExtremePoint(search_dir) - a.FindExtremePoint(-search_dir);
	search_dir = -s.c; //search in direction of origin

	// Find a second simplex point (we now form a line in 3D space).
	s.b = b.FindExtremePoint(search_dir) - a.FindExtremePoint(-search_dir);

	if (mmlDot(s.b, search_dir) < 0.0f) { // Origin not enclosed in the simplex, i.e. no overlap.
		return false;
	}

	search_dir = mmlCross(mmlCross(s.c - s.b, -s.b), s.c - s.b); // Search perpendicular to line segment towards origin.
	if (search_dir == mmlVector<3>::Fill(0.0f)) { // The origin is on the simplex line segment.
		search_dir = mmlCross(s.c - s.b, retro3d::Transform::GetWorldRight()); // Normal with x-axis.
		if (search_dir == mmlVector<3>::Fill(0.0f)) {
			search_dir = mmlCross(s.c - s.b, -retro3d::Transform::GetWorldForward());
		} // Normal with z-axis.
	}
	s.dimension = 2; // Initialize simplex dimension to 2.

	for (int i = 0; i < GJK_MAX_NUM_ITERATIONS; ++i) {
		s.a = b.FindExtremePoint(search_dir) - a.FindExtremePoint(-search_dir);
		if (mmlDot(s.a, search_dir) < 0.0f) { // Origin not enclosed in the simplex, i.e. no overlap.
			return false;
		}

		++s.dimension;
		if (s.dimension == 3) {
			update_simplex3(s, search_dir);
		} else if (update_simplex4(s, search_dir) == true) {
			mmlVector<3> epa = EPA(s, a, b, contact_info->point);
			contact_info->depth = epa.Len();
			contact_info->normal = mmlIsApproxEqual(contact_info->depth, 0.0f) == false ? epa / contact_info->depth : mmlVector<3>::Fill(0.0f);
			return true;
		}
	}
	return false;
}

retro3d::Collider::Collider( void ) :
	m_transform(),
	m_obj_aabb{ mmlVector<3>::Fill(0.0f), mmlVector<3>::Fill(0.0f) }
{
	m_transform.New();
	*m_transform.GetShared() = mmlMatrix<4,4>::Identity();
}

retro3d::Collider::~Collider( void )
{}

void retro3d::Collider::AttachTransform(retro3d::SharedTransform transform)
{
	m_transform = transform;
	if (m_transform.IsNull()) {
		m_transform.New();
	}
}

const retro3d::Transform &retro3d::Collider::GetTransform( void ) const
{
	return *m_transform.GetShared();
}

retro3d::Transform &retro3d::Collider::GetTransform( void )
{
	return *m_transform.GetShared();
}

retro3d::SharedTransform retro3d::Collider::GetSharedTransform( void ) const
{
	return m_transform;
}

void retro3d::Collider::SetTransform(const retro3d::Transform &transform)
{
	*m_transform.GetShared() = transform;
}

retro3d::AABB retro3d::Collider::GetLocalAABB( void ) const
{
	return m_obj_aabb;
}

void retro3d::Collider::SetLocalAABB(const retro3d::AABB &obj_aabb)
{
	m_obj_aabb = obj_aabb;
}

retro3d::AABB retro3d::Collider::GetAABB( void ) const
{
	return m_obj_aabb.ApplyTransform(*m_transform.GetShared());
}

bool retro3d::AABBCollider::DD_IsColliding(const retro3d::AABBCollider &c, retro3d::Collider::Contact *out) const
{
	// NOTE: Is this the correct order of inputs?
	return GJK_DetectCollision(c, *this, out);
}

bool retro3d::AABBCollider::DD_IsColliding(const retro3d::PolyCollider &c, retro3d::Collider::Contact *out) const
{
	// NOTE: Is this the correct order of inputs?
	return GJK_DetectCollision(c, *this, out);
}

bool retro3d::AABBCollider::IsColliding(const retro3d::Ray &world_ray, retro3d::Ray::Contact *out) const
{
	retro3d::Ray::Contact temp;
	retro3d::Ray::Contact *ct = (out != nullptr) ? &temp : nullptr;
	bool is_colliding =	AABB_DetectCollision(*this, world_ray, ct);
	if (is_colliding == true && out != nullptr) { *out = *ct; }
	return is_colliding;
}

bool retro3d::AABBCollider::IsColliding(const retro3d::Collider &c, retro3d::Collider::Contact *out) const
{
	return AABB_DetectCollision(*this, c) == true && c.DD_IsColliding(*this, out) == true;
}

mmlVector<3> retro3d::AABBCollider::FindExtremePoint(const mmlVector<3> &world_search_direction) const
{
	// NOTE: We need to transform the AABB to world space, not the direction to object space.

	mmlVector<3> v[8];
	GetAABB().GetCorners(v);

	const mmlVector<3> dir = m_transform->TransformNormal(world_search_direction);
	mmlVector<3> max_v = v[0];
	float max_dot = mmlDot(max_v, dir);

	for (int i = 1; i < 8; ++i) {
		const float dot = mmlDot(v[i], dir);
		if (dot > max_dot) {
			max_dot = dot;
			max_v = v[i];
		}
	}

	return max_v;
}

bool retro3d::PolyCollider::IsCollidingRay(const retro3d::Ray &world_ray, retro3d::Ray::Contact *out) const
{
	// NOTE:
	// WARNING (READ BEFORE MODIFYING):
	// Ray_Tri returns a distance to the impact point based on the scale of the normal.
	// Transforming the world_ray direction to local_ray direction using ROTATION of local transform (inverse world transform) yields distance to impact in LOCAL units.
		// This is NOT the number we are interested in returning to the user.
		// Correct number requres recalculation via mmlDist or via some sort of scaling of distance value based on transform scale.
	// Transforming the world_ray direction to local_ray direction using BASIS of local transform (inverse world transform) yields distance to impact in WORLD units.
		// THIS is the number we are interested in returning to the user.

	// TODO: This needs a rewrite for speed. Use some form of octree stored in Morton order? Can use mglGridWalker to traverse tree. Pack tightly.

	const mmlMatrix<4,4> Inv = m_transform->GetInvMatrix();
	const Ray local_ray = { // Why are we not using ApplyTransform? See note above.
		world_ray.origin * Inv,
		world_ray.direction * mmlBasis(Inv) // mmlBasis instead of mmlRotation
	};
	bool found_collision = false;
	for (int f = 0; f < m_face.GetSize(); ++f) {
		for (int i = 0; i < m_face[f].GetSize() - 2; ++i) {
			mglRayCollisionPoint3D p;
			const mmlVector<3> a = m_vert[ tiny3d::UInt(m_face[f][0].v) ];
			const mmlVector<3> b = m_vert[ tiny3d::UInt(m_face[f][i + 1].v) ];
			const mmlVector<3> c = m_vert[ tiny3d::UInt(m_face[f][i + 2].v) ];
			if (found_collision == false) {
				if (mglCollision::Ray_Tri(local_ray.origin, local_ray.direction, a, b, c, &p) == true) {
					found_collision = true;
					if (out != nullptr) {
						out->entry = out->exit = p;
						out->exit.normal = -p.normal;
					} else {
						return true;
					}
				} else if (mglCollision::Ray_Tri(local_ray.origin, local_ray.direction, c, b, a, &p) == true) {
					found_collision = true;
					if (out != nullptr) {
						out->exit = out->entry = p;
						out->entry.normal = -p.normal;
					} else {
						return true;
					}
				}
			} else if (mglCollision::Ray_Tri(local_ray.origin, local_ray.direction, a, b, c, &p) == true) {
				if (p.distance < out->entry.distance) {
					out->entry = p;
				}
			} else if (mglCollision::Ray_Tri(local_ray.origin, local_ray.direction, c, b, a, &p) == true) {
				if (p.distance > out->exit.distance) {
					out->exit = p;
				}
			}
		}
	}
	if (found_collision == true && out != nullptr) {
		if (world_ray.length <= 0.0f || out->entry.distance <= world_ray.length) {
			// Transform the collision data to world space
			const mmlMatrix<3,3> r = m_transform->GetRotation();
			out->entry.point  *= *m_transform.GetShared();
			out->entry.normal *= r;

			if (world_ray.length <= 0.0f || out->exit.distance <= world_ray.length) {
				out->exit.point   *= *m_transform.GetShared();
				out->exit.normal  *= r;
			} else { // the exit point is further away than ray length, just return projected ray point at distance = ray length
				out->exit.point    = world_ray.origin + world_ray.direction * world_ray.length;
				out->exit.normal   = world_ray.direction;
				out->exit.distance = world_ray.length; // clip the exit distance
			}

			// NOTE: If the ray direction is transformed to local space using normalized local transform (mmlRotation instead of mmlBasis), then distance results need to be scaled here *somehow*.
		} else {
			found_collision = false;
		}
	}

	return found_collision;
}

bool retro3d::PolyCollider::DD_IsColliding(const retro3d::AABBCollider &c, retro3d::Collider::Contact *out) const
{
	// NOTE: Is this the correct order of inputs?
	return GJK_DetectCollision(c, *this, out);
}

bool retro3d::PolyCollider::DD_IsColliding(const retro3d::PolyCollider &c, retro3d::Collider::Contact *out) const
{
	// NOTE: Is this the correct order of inputs?
	return GJK_DetectCollision(c, *this, out);
}

bool retro3d::PolyCollider::IsColliding(const retro3d::Ray &world_ray, retro3d::Ray::Contact *out) const
{
	retro3d::Ray::Contact temp;
	retro3d::Ray::Contact *ct = (out != nullptr) ? &temp : nullptr;
	bool is_colliding =
		AABB_DetectCollision(*this, world_ray, ct) &&
		IsCollidingRay(world_ray, ct);
	if (is_colliding == true && out != nullptr) { *out = *ct; }
	return is_colliding;
}

bool retro3d::PolyCollider::IsColliding(const retro3d::Collider &c, retro3d::Collider::Contact *out) const
{
	retro3d::Collider::Contact temp;
	retro3d::Collider::Contact *ct = (out != nullptr) ? &temp : nullptr;
	bool is_colliding =
		AABB_DetectCollision(*this, c) &&
		c.DD_IsColliding(*this, ct);
	if (is_colliding == true && out != nullptr) { *out = *ct; }
	return is_colliding;
}

void retro3d::PolyCollider::SetGeometry(const retro3d::Array< mmlVector<3> > &vert, const retro3d::Array< retro3d::FaceIndexV > &faces)
{
	m_vert = vert;
	m_face = faces;
	m_obj_aabb.FromGeometry(m_vert);
}

void retro3d::PolyCollider::SetGeometry(const retro3d::Array< mmlVector<3> > &vert, const retro3d::Array< retro3d::FaceIndexVN > &faces)
{
	m_vert = vert;
	m_face.Create(faces.GetSize());
	for (int f = 0; f < m_face.GetSize(); ++f) {
		m_face[f].Create(faces[f].GetSize());
		for (int v = 0; v < m_face[f].GetSize(); ++v) {
			m_face[f][v].v = faces[f][v].v;
		}
	}
	m_obj_aabb.FromGeometry(m_vert);
}

void retro3d::PolyCollider::SetGeometry(const retro3d::Array< mmlVector<3> > &vert, const retro3d::Array< retro3d::FaceIndex > &faces)
{
	m_vert = vert;
	m_face.Create(faces.GetSize());
	for (int f = 0; f < m_face.GetSize(); ++f) {
		m_face[f].Create(faces[f].GetSize());
		for (int v = 0; v < m_face[f].GetSize(); ++v) {
			m_face[f][v].v = faces[f][v].v;
		}
	}
	m_obj_aabb.FromGeometry(m_vert);
}

mmlVector<3> retro3d::PolyCollider::FindExtremePoint(const mmlVector<3> &world_search_direction) const
{
	// TODO: Do this search on the convex hull, not the concave hull since concave points can never be extremes.

	/*if (m_vert.GetSize() <= 0) { return mmlVector<3>::Fill(0.0f); }

	const mmlVector<3> local_dir = m_transform->InverseTransformNormal(world_search_direction);
	mmlVector<3> max_v = m_vert[0];
	float max_dot = mmlDot(max_v, local_dir);

	for (int i = 1; i < m_vert.GetSize(); ++i) {
		const float dot = mmlDot(m_vert[i], local_dir);
		if (dot > max_dot) {
			max_dot = dot;
			max_v = m_vert[i];
		}
	}

	return m_transform->TransformPoint(max_v);*/

	if (m_vert.GetSize() <= 0) { return mmlVector<3>::Fill(0.0f); }
	const mmlVector<3> local_dir = m_transform->InverseTransformNormal(world_search_direction);
	const mmlVector<3> max_v = retro3d::FindExtremeVertexAlongDirection(m_vert, local_dir);
	return m_transform->TransformPoint(max_v);
}
