#ifndef BOUNDARY_TREE_H

#define BOUNDARY_TREE_H

#include <Core/AABB.h>
#include <Core/PoolAllocator.h>
#include <memory>

template <typename TValue>
struct Node
{
	Node(const AABB& aabb)
		: Node(nullptr, nullptr, aabb) {}

	Node(Node* left, Node* right, const AABB& aabb)
		: Node(left, right, aabb, TValue()) {}

	Node(Node* left, Node* right, const AABB& aabb, const TValue& value)
		: left(left), right(right), AABB(aabb), value(value) {}

	~Node() = default;

	Node* left;
	Node* right;
	AABB AABB;
	TValue value;
};

template <typename TValue>
class BoundaryTree
{
public:
	BoundaryTree() : BoundaryTree(nullptr) {}
	BoundaryTree(Node<TValue>* root) : m_Root(root), m_Allocator(512) {}
	~BoundaryTree() { deallocateNode(m_Root); }

	Node<TValue>* root()			 { return m_Root; }
	const Node<TValue>* root() const { return m_Root; };

	void clear();
	Node<TValue>* createNode(const AABB& aabb);
	void transferAllocatedBlocks(BoundaryTree& other);

private:
	Node<TValue>* m_Root;
	PoolAllocator m_Allocator;

	void deallocateNode(Node<TValue>* node);
};

#endif // !BOUNDARY_TREE_H

template<typename TValue>
inline void BoundaryTree<TValue>::clear()
{
	deallocateNode(m_Root);
	m_Allocator.freeAllocatedMemory();
	m_Root = nullptr;
}

template<typename TValue>
inline Node<TValue>* BoundaryTree<TValue>::createNode(const AABB& aabb)
{
	void* nodeMemory = m_Allocator.allocate(sizeof(Node<TValue>));
	Node<TValue>* node = reinterpret_cast<Node<TValue>*>(nodeMemory);
	new (node) Node<TValue>(aabb);

	if (m_Root == nullptr)
		m_Root = node;

	return node;
}

template<typename TValue>
inline void BoundaryTree<TValue>::transferAllocatedBlocks(BoundaryTree& other)
{
	m_Root = nullptr;
	m_Allocator.transferOwnership(other.m_Allocator);
}

template<typename TValue>
inline void BoundaryTree<TValue>::deallocateNode(Node<TValue>* node)
{
	if (node == nullptr)
		return;

	deallocateNode(node->left);
	deallocateNode(node->right);

	node->~Node();
}
