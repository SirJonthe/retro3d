#ifndef RETRO_COLLIDER_TREE_H
#define RETRO_COLLIDER_TREE_H

#include "../common/retro_geom.h"
#include "../common/MiniLib/MTL/mtlType.h"
#include "../physics/retro_collider.h"
#include "../common/retro_assert.h"
#include <stack>
#include <queue>

// http://allenchou.net/2014/02/game-physics-broadphase-dynamic-aabb-tree/
// http://box2d.org/files/GDC2019/ErinCatto_DynamicBVH_GDC2019.pdf

// NOTE: ColliderTree = Bounding Volume Hierarchy (BVH)

namespace retro3d
{

template < typename leafdata_t >
class ColliderTree
{
public:
	struct Container
	{
		leafdata_t        *user_data;
		retro3d::Collider *collider;
	};

	struct Contact
	{
		Container a;
		Container b;
		bool a_is_static;
		bool b_is_static;
		uint64_t a_filter_flags;
		uint64_t b_filter_flags;

		Contact( void );
	};
	struct Debug_AABB : public retro3d::AABB
	{
		enum Type {
			Node_Dynamic,
			Node_Static,
			Leaf_Dynamic,
			Leaf_Static
		} type;
	};

	typedef mtlList<typename retro3d::ColliderTree<leafdata_t>::Contact> Contacts;

private:
	struct Node
	{
		Node          *parent;
		Node          *child0;
		Node          *child1;
		Container      container;
		retro3d::AABB  world_aabb;
		uint64_t       filter_flags;
		bool           is_static;

		Node( void );
		Node(leafdata_t *l, retro3d::Collider *c, float movement_margin, uint64_t filterFlags, bool isStatic);
		Node(Node *c0, Node *c1);
		~Node( void );
		void Destroy( void );
		bool IsLeaf( void ) const;
		void SetLeaf(leafdata_t *l, retro3d::Collider *c, float movement_margin, uint64_t filterFlags, bool isStatic);
		void SetBranch(Node *c0, Node *c1);
		void UpdateParentData( void );
		retro3d::AABB GetColliderWorldAABB( void ) const;
		Node *GetRoot( void );
		bool IsRoot( void ) const;
		Node *FindLeaf(const leafdata_t *l);
		Node *FindLeaf(const retro3d::Collider *c);
	};

	typedef retro3d::Array<Node> Nodes;
	typedef retro3d::Array<const retro3d::Collider*> Colliders;

	struct NodeRemoval
	{
		Node      *node;
		Container  container;
		uint64_t   filter_flags;
		bool       is_static; // should almost always be TRUE (static objects should not be able to move outside of their boxes)
	};

	typedef retro3d::Array<NodeRemoval> Removals;

private:
	Node      *m_root;
	Nodes      m_nodes;
	Colliders  m_colliders;
	Contacts   m_contacts;
	Removals   m_removal_list;
	uint32_t   m_collider_count;
	float      m_movement_margin;
	int32_t    m_memory_growth;
	bool       m_check_contacts;

private:
	float  TestCostUp(const Node *parent, const retro3d::AABB &aabb) const;
	float  GetUpCost(const Node *n) const;
	float  GetCost(const Node *n) const;
	float  InheritedCost(const Node *parent, retro3d::AABB world_aabb) const;
	Node  *FindBestSibling(const Node *n);
	void   AddContacts(const Node *n);
	void   RemoveContacts(const Node *n);
	void   Insert(Node *&n, leafdata_t *l, const retro3d::Collider *c, const retro3d::AABB &c_world_aabb);
	void   Debug_PrintTree(const Node *n, int depth) const;
	void   Debug_Tabs(int depth) const;
	bool   Debug_VerifyIntegrity( void ) const;
	void   Remove(Node *n);
	void   Reinsert( void );

public:
	ColliderTree( void );
	~ColliderTree( void );

	void Insert(leafdata_t *leaf_data, retro3d::Collider *collider, bool is_static = false, uint64_t filter_flags = ~uint64_t(0));
	bool Remove(const leafdata_t *leaf_data);
	bool Remove(const retro3d::Collider *collider);
	void Update( void );
	void Destroy( void );

	void SetMovementMargin(float margin);

	void Debug_PrintTree( void ) const;
	std::queue<Debug_AABB> Debug_GetTree( void ) const;

	leafdata_t *CastRay(const retro3d::Ray &world_ray, uint64_t filter_flags, retro3d::Ray::Contact *contact_info = nullptr) const;
	bool Contains(const mmlVector<3> &world_point, uint64_t filter_flags, mtlList<leafdata_t*> *contacts = nullptr) const;
	bool Contains(const retro3d::Frustum &world_frustum, uint64_t filter_flags, mtlList<leafdata_t*> *contacts = nullptr) const;

	uint32_t GetColliderCount( void ) const;

	const Contacts &GetPotentialContacts( void ) const;

	bool IsCheckingContacts( void ) const;
	void ToggleContactCheck( void );
};

}

// Optimizations:
// AVL balancing
// Nodes stored in linear array, not actually in tree

// Problem:
	// Sorted input leads to a linked list of AABBs, i.e. linear time search
	// Possible solutions:
		// At insertion, randomly swap child pointers at parent level?
		// AVL balancing?

template < typename leafdata_t >
retro3d::ColliderTree<leafdata_t>::Contact::Contact( void ) : a{ nullptr, nullptr }, b{ nullptr, nullptr }, a_is_static(false), b_is_static(false), a_filter_flags(0), b_filter_flags(0)
{}

template < typename leafdata_t >
retro3d::ColliderTree<leafdata_t>::Node::Node( void ) :
	parent(nullptr),
	child0(nullptr), child1(nullptr),
	container{ nullptr, nullptr},
	world_aabb(),
	filter_flags(0),
	is_static(false)
{}

template < typename leafdata_t >
retro3d::ColliderTree<leafdata_t>::Node::Node(leafdata_t *l, retro3d::Collider *c, float movement_margin, uint64_t filterFlags, bool isStatic) :
	Node()
{
	SetLeaf(l, c, movement_margin, filterFlags, isStatic);
}

template < typename leafdata_t >
retro3d::ColliderTree<leafdata_t>::Node::Node(retro3d::ColliderTree<leafdata_t>::Node *c0, retro3d::ColliderTree<leafdata_t>::Node *c1) :
	Node()
{
	SetBranch(c0, c1);
}

template < typename leafdata_t >
retro3d::ColliderTree<leafdata_t>::Node::~Node( void )
{
	delete child0;
	delete child1;
}

template < typename leafdata_t >
void retro3d::ColliderTree<leafdata_t>::Node::Destroy( void )
{
	delete child0;
	child0 = nullptr;
	delete child1;
	child1 = nullptr;
	container = { nullptr, nullptr };
}

template < typename leafdata_t >
bool retro3d::ColliderTree<leafdata_t>::Node::IsLeaf( void ) const
{
	return container.collider != nullptr;
}

template < typename leafdata_t >
void retro3d::ColliderTree<leafdata_t>::Node::SetLeaf(leafdata_t *l, retro3d::Collider *c, float movement_margin, uint64_t filterFlags, bool isStatic)
{
	Destroy();
	container = { l, c };
	world_aabb = GetColliderWorldAABB();
	world_aabb.SetHalfExtents(world_aabb.GetHalfExtents() + mmlVector<3>::Fill(movement_margin));
	filter_flags = filterFlags;
	is_static = isStatic;
	UpdateParentData();
}

template < typename leafdata_t >
void retro3d::ColliderTree<leafdata_t>::Node::SetBranch(retro3d::ColliderTree<leafdata_t>::Node *c0, retro3d::ColliderTree<leafdata_t>::Node *c1)
{
	Destroy();
	child0 = c0;
	child1 = c1;
	child0->parent = this;
	child1->parent = this;
	world_aabb = child0->world_aabb | child1->world_aabb;
	filter_flags = child0->filter_flags | child1->filter_flags;
	is_static = child0->is_static | child1->is_static;
	UpdateParentData();
}

template < typename leafdata_t >
void retro3d::ColliderTree<leafdata_t>::Node::UpdateParentData( void )
{
	Node *n = parent;
	while (n != nullptr) {
		n->world_aabb = n->child0->world_aabb |  n->child1->world_aabb;
		n->filter_flags = n->child0->filter_flags | n->child1->filter_flags;
		n->is_static = n->child0->is_static | n->child1->is_static;
		n = n->parent;
	}
}

template < typename leafdata_t >
retro3d::AABB retro3d::ColliderTree<leafdata_t>::Node::GetColliderWorldAABB( void ) const
{
	return container.collider->GetAABB();
}

template < typename leafdata_t >
typename retro3d::ColliderTree<leafdata_t>::Node *retro3d::ColliderTree<leafdata_t>::Node::GetRoot( void )
{
	Node *n = this;
	while (n->parent != nullptr) {
		n = n->parent;
	}
	return n;
}

template < typename leafdata_t >
bool retro3d::ColliderTree<leafdata_t>::Node::IsRoot( void ) const
{
	return parent == nullptr;
}

template < typename leafdata_t >
typename retro3d::ColliderTree<leafdata_t>::Node *retro3d::ColliderTree<leafdata_t>::Node::FindLeaf(const leafdata_t *l)
{
	// TODO: This can be sped up using a simple array for leaves
	if (IsLeaf() == false) {
		Node *S = child0->FindLeaf(l);
		return S != nullptr ? S : child1->FindLeaf(l);
	}
	return l == container.user_data ? this : nullptr;
}

template < typename leafdata_t >
typename retro3d::ColliderTree<leafdata_t>::Node *retro3d::ColliderTree<leafdata_t>::Node::FindLeaf(const retro3d::Collider *c)
{
	// TODO: This can be sped up using a simple array for leaves
	if (IsLeaf() == false) {
		Node *S = child0->FindLeaf(c);
		return S != nullptr ? S : child1->FindLeaf(c);
	}
	return c == container.collider ? this : nullptr;
}

template < typename leafdata_t >
float retro3d::ColliderTree<leafdata_t>::TestCostUp(const retro3d::ColliderTree<leafdata_t>::Node *parent, const retro3d::AABB &aabb) const
{
	if (parent == nullptr) { return 0.0f; }
	AABB union_aabb = parent->world_aabb | aabb;
	return ((parent->IsLeaf() == false) ? union_aabb.GetArea() : 0.0f) + TestCostUp(parent->parent, union_aabb);
}

template < typename leafdata_t >
float retro3d::ColliderTree<leafdata_t>::GetUpCost(const retro3d::ColliderTree<leafdata_t>::Node *n) const
{
	if (n == nullptr) { return 0.0f; }
	return (n->IsLeaf() == false ? n->world_aabb.GetArea() : 0.0f) + GetUpCost(n->parent);
}

template < typename leafdata_t >
float retro3d::ColliderTree<leafdata_t>::GetCost(const retro3d::ColliderTree<leafdata_t>::Node *n) const
{
	if (n == nullptr || n->IsLeaf()) { return 0.0f; }
	return n->world_aabb.GetArea() + GetCost(n->child0) + GetCost(n->child1);
}

template < typename leafdata_t >
float retro3d::ColliderTree<leafdata_t>::InheritedCost(const retro3d::ColliderTree<leafdata_t>::Node *parent, retro3d::AABB new_parent_aabb) const
{
	float inherited_cost = 0.0f;
	while (parent != nullptr) {
		inherited_cost += new_parent_aabb.GetArea() - parent->world_aabb.GetArea();
		new_parent_aabb = new_parent_aabb | parent->world_aabb;
		parent = parent->parent;
	}
	return inherited_cost;
}

template < typename leafdata_t >
typename retro3d::ColliderTree<leafdata_t>::Node *retro3d::ColliderTree<leafdata_t>::FindBestSibling(const retro3d::ColliderTree<leafdata_t>::Node *L)
{
	struct SearchNode
	{
		Node  *node;
		float  cost;

		bool operator<(const SearchNode &r) const { return cost < r.cost; }
	};

	Node *S_best = nullptr;
	float C_best = 0.0f;
	std::priority_queue<SearchNode> Q;
	Q.push({ m_root, 0.0f });

	while (Q.empty() == false) {

		SearchNode N = Q.top();
		Q.pop();

		if (N.node == nullptr) { continue; }

		retro3d::AABB new_parent_aabb = N.node->world_aabb | L->world_aabb;
		float direct_cost = new_parent_aabb.GetArea();
		float inherited_cost = InheritedCost(N.node->parent, new_parent_aabb);
		float C_N = direct_cost + inherited_cost;

		if (C_N < C_best || S_best == nullptr) {
			S_best = N.node;
			C_best = C_N;

			float C_low = L->world_aabb.GetArea() + InheritedCost(N.node, L->world_aabb);
			if (C_low < C_best) {
				Q.push({ N.node->child0, C_best });
				Q.push({ N.node->child1, C_best });
			}
		}
	}

	return S_best;
}

template < typename leafdata_t >
void retro3d::ColliderTree<leafdata_t>::AddContacts(const retro3d::ColliderTree<leafdata_t>::Node *n)
{
	if (m_check_contacts == false) { return; }

	std::stack<const Node*> stack;
	stack.push(m_root);

	while (stack.empty() == false) {

		const Node *top = stack.top();
		stack.pop();

		// Rules for collision:
		// Leaf can not collide with itself.
		// Leaf and potential collider leaf must chage a collider flag.
		// Leaf and potential collider leaf must not both be static.
		// Leaf and potential collider leaf must have overlapping AABBs.
		if (top == nullptr || top == n || (top->filter_flags & n->filter_flags) == 0 || (top->is_static && n->is_static) || top->world_aabb.Overlaps(n->world_aabb) == false) { continue; }

		if (top->IsLeaf() == true) {
			Contact c;
			c.a = n->container;
			c.b = top->container;
			c.a_is_static = n->is_static;
			c.b_is_static = top->is_static;
			c.a_filter_flags = n->filter_flags;
			c.b_filter_flags = top->filter_flags;
			m_contacts.AddLast(c);
		} else {
			stack.push(top->child0);
			stack.push(top->child1);
		}
	}
}

template < typename leafdata_t >
void retro3d::ColliderTree<leafdata_t>::RemoveContacts(const retro3d::ColliderTree<leafdata_t>::Node *n)
{
	if (m_check_contacts == false) { return; }

	mtlItem<Contact> *i = m_contacts.GetFirst();
	while (i != nullptr) {
		if ((i->GetItem().a.user_data == n->container.user_data && i->GetItem().a.collider == n->container.collider) || (i->GetItem().b.user_data == n->container.user_data && i->GetItem().b.collider == n->container.collider)) {
			i = i->Remove();
		} else {
			i = i->GetNext();
		}
	}
}

template < typename leafdata_t >
retro3d::ColliderTree<leafdata_t>::ColliderTree( void ) :
	m_root(nullptr), m_nodes(), m_colliders(), m_contacts(), m_removal_list(), m_collider_count(0), m_movement_margin(0.2f), m_memory_growth(512), m_check_contacts(true)
{
	m_nodes.poolMemory = true;
	m_nodes.SetCapacity(m_memory_growth);
	m_colliders.poolMemory = true;
	m_colliders.SetCapacity(m_memory_growth);
	m_removal_list.poolMemory = true;
	m_removal_list.SetCapacity(m_memory_growth);
}

template < typename leafdata_t >
retro3d::ColliderTree<leafdata_t>::~ColliderTree( void )
{
	delete m_root;
}

template < typename leafdata_t >
void retro3d::ColliderTree<leafdata_t>::Insert(leafdata_t *leaf_data, retro3d::Collider *collider, bool is_static, uint64_t filter_flags)
{
	// Process:
	// 1) Find best sibling.
	// 2) Create a new parent with sibling and n and children.
	// 3) Sibling's old parent now becomes parent to newly created parent (becomes sibling's grand-parent).
	// 4) Insert potential collision into potential collision list

	if (leaf_data == nullptr || collider == nullptr) { return; }

	++m_collider_count;
//	std::cout << "+" << m_collider_count << std::endl;

	Node *n = new Node(leaf_data, collider, m_movement_margin, filter_flags, is_static);

	if (m_root == nullptr) {
		m_root = n;
	} else {
		// 1) Find best sibling
		Node *sib = FindBestSibling(n);
		Node *old_parent = sib->parent;

		// 2) Create a new parent with sibling and n and children.
		Node *new_parent = new Node(n, sib);
		if (old_parent != nullptr) {
			if (sib == old_parent->child0) { old_parent->child0 = new_parent; }
			else                           { old_parent->child1 = new_parent; }
		}

		// 3) Sibling's old parent now becomes parent to newly created parent (becomes sibling's grand-parent).
		new_parent->parent = old_parent;
		m_root = m_root->GetRoot(); // In case the root shifted
		new_parent->UpdateParentData();

		// 4) Insert potential collision into potential collision list
		AddContacts(n);
	}

	if (m_removal_list.GetCapacity() < m_collider_count) {
		m_removal_list.SetCapacity(m_removal_list.GetCapacity() + m_memory_growth);
	}

	RETRO3D_ASSERT(Debug_VerifyIntegrity() == true);
}

template < typename leafdata_t >
void retro3d::ColliderTree<leafdata_t>::Remove(retro3d::ColliderTree<leafdata_t>::Node *n)
{
	// NOTE: Removes entire subtree

	// Process:
	// 1) Parent is deleted
	// 2) Sibling takes parent's place
	// 3) Sib->UpdateParentData();
	// 4) Remove potential collisions from potential collision list

	if (n == nullptr) { return; }

	--m_collider_count;
//	std::cout << "-" << m_collider_count << std::endl;

	if (n->IsRoot() == true) {
		delete n;
		m_root = nullptr;
		m_contacts.RemoveAll();
	} else {

		Node *parent  = n->parent;      // node parent
		Node *gparent = parent->parent; // node grand-parent
		Node *sib     = nullptr;        // node sibling (to be set below)

		// 1) Parent is deleted
		if (n == parent->child0) {
			sib = parent->child1; // 1.a) get n's sibling (to be new parent)
			parent->child1 = nullptr; // 1.b) sibling is child1, so child1 ptr is nulled so that deleting parent does not also delete sibling
		} else {
			sib = parent->child0; // 1.a) get n's sibling (to be new parent)
			parent->child0 = nullptr; // 1.b) sibling is child0, so child0 ptr is nulled so that deleting parent does not also delete sibling
		}

		// 2) Sibling takes parent's place
		sib->parent = gparent; // sibling parent pointer is set to grand-parent
		if (gparent != nullptr) { // grand-parent child pointer is set to sibling
			if (gparent->child0 == parent) {
				gparent->child0 = sib;
			} else {
				gparent->child1 = sib;
			}
		}

		// 3) Refit AABBs
		m_root = m_root->GetRoot();
		sib->UpdateParentData();

		// 4) Remove potential collisions from potential collision list
		RemoveContacts(n);

		delete parent; // this deletes parent and parent subtree, including n (since n ptr was never nulled at parent level)
	}

	RETRO3D_ASSERT(Debug_VerifyIntegrity() == true);
}

template < typename leafdata_t >
void retro3d::ColliderTree<leafdata_t>::Reinsert( void )
{	
	for (int i = 0; i < m_removal_list.GetSize(); ++i) { Remove(m_removal_list[i].node); }
	for (int i = 0; i < m_removal_list.GetSize(); ++i) { Insert(m_removal_list[i].container.user_data, m_removal_list[i].container.collider, m_removal_list[i].is_static, m_removal_list[i].filter_flags); }
	m_removal_list.Free();
}

template < typename leafdata_t >
bool retro3d::ColliderTree<leafdata_t>::Remove(const leafdata_t *leaf_data)
{
	if (m_root == nullptr) { return false; }
	Node *N = m_root->FindLeaf(leaf_data);
	Remove(N);
	return N != nullptr;
}

template < typename leafdata_t >
bool retro3d::ColliderTree<leafdata_t>::Remove(const retro3d::Collider *collider)
{
	if (m_root == nullptr) { return false; }
	Node *N = m_root->FindLeaf(collider);
	Remove(N);
	return N != nullptr;
}

template < typename leafdata_t >
void retro3d::ColliderTree<leafdata_t>::Update( void )
{
	// TODO: This can be sped up using one array for colliders and one array for nodes

	std::stack<Node*> s;
	s.push(m_root);

	while (s.empty() == false) {

		Node *N = s.top();
		s.pop();

		if (N == nullptr /*|| N->is_static == true*/) { continue; } // TODO: Skip all nodes/leaves that are static. However, due to some bug, nodes marked as static can contain dynamic leaves.

		if (N->IsLeaf() == true) {
			if (N->world_aabb.Contains(N->GetColliderWorldAABB()) <= retro3d::Contain_Partial) { // remove and re-insert if collider AABB has fully or partially slipped out of the bounds of the parent AABB

				// NOTE:
				// Ideally this node should be removed and data reinserted here.
				// However, removing changes the memory layout which invalidates pointers on the stack.

				m_removal_list.Add(NodeRemoval{ N, N->container, N->filter_flags, N->is_static });
			}
		} else {
			s.push(N->child0);
			s.push(N->child1);
		}
	}

	Reinsert();
}

template < typename leafdata_t >
void retro3d::ColliderTree<leafdata_t>::Destroy( void )
{
	delete m_root;
	m_root = nullptr;
	m_contacts.RemoveAll();
}

template < typename leafdata_t >
void retro3d::ColliderTree<leafdata_t>::SetMovementMargin(float margin)
{
	m_movement_margin = margin;
}

template < typename leafdata_t >
void retro3d::ColliderTree<leafdata_t>::Debug_PrintTree(const retro3d::ColliderTree<leafdata_t>::Node *n, int depth) const
{
	if (n == nullptr) {
		Debug_Tabs(depth);
		std::cout << "(null)" << std::endl;
		return;
	}

	Debug_Tabs(depth);
	std::cout << (n->IsLeaf() == false ? "(branch) " : "(leaf) ") << n << " {" << std::endl;
	++depth;


	mmlVector<3> c = n->world_aabb.GetCenter();
	mmlVector<3> e = n->world_aabb.GetHalfExtents();
	Debug_Tabs(depth);
	std::cout << "world_aabb : center{" << c[0] << "," << c[1] << "," << c[2] << "}, half_extents{" << e[0] << "," << e[1] << "," << e[2] << "}" << std::endl;
	if (n->IsLeaf() == false) {
		Debug_Tabs(depth);
		std::cout << "child0 : " << std::endl;
		Debug_PrintTree(n->child0, depth);
		Debug_Tabs(depth);
		std::cout << "child1 : " << std::endl;
		Debug_PrintTree(n->child1, depth);
	} else {
		Debug_Tabs(depth);
		std::cout << "data : " << n->container.user_data << ", "<< n->container.collider << std::endl;
		c = n->GetColliderWorldAABB().GetCenter();
		e = n->GetColliderWorldAABB().GetHalfExtents();
		Debug_Tabs(depth);
		std::cout << "collider_world_aabb : center{" << c[0] << "," << c[1] << "," << c[2] << "}, half_extents{" << e[0] << "," << e[1] << "," << e[2] << "}" << std::endl;
	}

	--depth;
	Debug_Tabs(depth);
	std::cout << "}" << std::endl;
}

template < typename leafdata_t >
void retro3d::ColliderTree<leafdata_t>::Debug_Tabs(int depth) const
{
	while (depth--) {
		std::cout << "  ";
	}
}

template < typename leafdata_t >
bool retro3d::ColliderTree<leafdata_t>::Debug_VerifyIntegrity( void ) const
{
	uint32_t collider_count = 0;

	std::stack<const Node*> stack;
	stack.push(m_root);

	while (stack.empty() == false) {

		const Node *top = stack.top();
		stack.pop();

		if (top == nullptr) { continue; }

		if (top->IsLeaf() == true) {
			++collider_count;
		} else {
			stack.push(top->child0);
			stack.push(top->child1);
		}
	}
	if (collider_count != m_collider_count) {
		std::cout << "ColliderTree corrpution: traversed leaf count = " << collider_count << " != stored leaf count = " << m_collider_count << std::endl;
	}
	return collider_count == m_collider_count;
}

template < typename leafdata_t >
void retro3d::ColliderTree<leafdata_t>::Debug_PrintTree( void ) const
{
	Debug_PrintTree(m_root, 0);
}

template < typename leafdata_t >
std::queue<typename retro3d::ColliderTree<leafdata_t>::Debug_AABB> retro3d::ColliderTree<leafdata_t>::Debug_GetTree( void ) const
{
	std::stack<const Node*> stack;
	std::queue<Debug_AABB> out;
	stack.push(m_root);

	while (stack.empty() == false) {

		const Node *top = stack.top();
		stack.pop();

		if (top == nullptr) {
			continue;
		}

		Debug_AABB aabb;
		aabb.SetCenter(top->world_aabb.GetCenter());
		aabb.SetHalfExtents(top->world_aabb.GetHalfExtents());
		aabb.type = (top->IsLeaf() == false) ? (top->is_static ? Debug_AABB::Node_Static : Debug_AABB::Node_Dynamic) : (top->is_static ? Debug_AABB::Leaf_Static : Debug_AABB::Leaf_Dynamic);
		out.push(aabb);
		if (top->IsLeaf() == false) {
			stack.push(top->child0);
			stack.push(top->child1);
		}
	}

	return out;
}

template < typename leafdata_t >
leafdata_t *retro3d::ColliderTree<leafdata_t>::CastRay(const retro3d::Ray &world_ray, uint64_t filter_flags, retro3d::Ray::Contact *contact_info) const
{
	retro3d::Ray::Contact temp_contact_info;
	leafdata_t *hit_detected = nullptr;
	std::stack<const Node*> stack;
	stack.push(m_root);

	while (stack.empty() == false) {

		const Node *top = stack.top();
		stack.pop();

		if (top == nullptr || (top->filter_flags & filter_flags) == 0) { continue; }

		if (top->IsLeaf() == true && top->container.collider->IsColliding(world_ray, &temp_contact_info) == true) {
			if (contact_info != nullptr && (hit_detected == nullptr || temp_contact_info.entry.distance < contact_info->entry.distance)) {
				*contact_info = temp_contact_info;
				hit_detected = top->container.user_data;
			}
		} else {
			mmlVector<3> max = top->world_aabb.GetMax();
			mmlVector<3> min = top->world_aabb.GetMin();
			if (mglCollision::Ray_AABB(world_ray.origin, world_ray.direction, min, max) == true) {
				// TODO: Sort the two children based on distance from the ray origin.
				// This ensures that the first true intersection be tested first.
				// Probably need to handle special case of AABBs contained inside other AABBs.
				stack.push(top->child0);
				stack.push(top->child1);
			}
		}
	}

	return hit_detected;
}

template < typename leafdata_t >
bool retro3d::ColliderTree<leafdata_t>::Contains(const mmlVector<3> &world_point, uint64_t filter_flags, mtlList<leafdata_t*> *out) const
{
	bool hit_detected = false;
	std::stack<const Node*> stack;
	stack.push(m_root);

	while (stack.empty() == false) {

		const Node *top = stack.top();
		stack.pop();

		if (top == nullptr || (top->filter_flags & filter_flags) == 0 || top->world_aabb.Contains(world_point) == retro3d::Contain_False) {
			continue;
		}

		if (top->IsLeaf() == true) {
			if (out != nullptr) {
				out->AddLast(top->container.user_data);
				hit_detected = true;
			} else {
				return true;
			}
		} else {
			stack.push(top->child0);
			stack.push(top->child1);
		}
	}

	return hit_detected;
}

template < typename leafdata_t >
bool retro3d::ColliderTree<leafdata_t>::Contains(const retro3d::Frustum &world_frustum, uint64_t filter_flags, mtlList<leafdata_t*> *out) const
{
	bool hit_detected = false;
	std::stack<const Node*> stack;
	stack.push(m_root);

	while (stack.empty() == false) {

		const Node *top = stack.top();
		stack.pop();

		if (top == nullptr || (top->filter_flags & filter_flags) == 0 || (world_frustum.Contains(top->world_aabb) == retro3d::Contain_False && top->world_aabb.Contains(world_frustum.GetOrigin()) == retro3d::Contain_False)) {
			continue;
		}

		if (top->IsLeaf() == true) {
			if (out != nullptr) {
				out->AddLast(top->container.user_data);
				hit_detected = true;
			} else {
				return true;
			}
		} else {
			stack.push(top->child0);
			stack.push(top->child1);
		}
	}

	return hit_detected;
}

template < typename leafdata_t >
uint32_t retro3d::ColliderTree<leafdata_t>::GetColliderCount( void ) const
{
	return m_collider_count;
}

template < typename leafdata_t >
const typename retro3d::ColliderTree<leafdata_t>::Contacts &retro3d::ColliderTree<leafdata_t>::GetPotentialContacts( void ) const
{
	return m_contacts;
}

template < typename leafdata_t >
bool retro3d::ColliderTree<leafdata_t>::IsCheckingContacts( void ) const
{
	return m_check_contacts;
}

template < typename leafdata_t >
void retro3d::ColliderTree<leafdata_t>::ToggleContactCheck( void )
{
	m_check_contacts = !m_check_contacts;
	if (m_check_contacts == false) {
		m_contacts.RemoveAll();
	}
}

#endif // RETRO_COLLIDER_TREE_H
