#ifndef MESH_H

#define MESH_H

#include <atomic>
#include <mutex>
#include <vector>
#include <Core/BoundaryTree.h>
#include <Objects/Surface.h>
#include <Containers/Matrix4.h>

using BVHNode = Node<std::vector<int>>;
using BVHTree = BoundaryTree<std::vector<int>>;

class Mesh : public Surface
{
	struct SplitInfo;
	struct SplitPair;

public:
	Mesh(const char* objFile, const std::shared_ptr<Material>& material);
	Mesh(std::vector<Vector3f>&& vertexBuffer, std::vector<Vector3i>&& indexBuffer, const std::shared_ptr<Material>& material);

	virtual bool isHit(const Ray& ray, Context& context, float minT, float maxT) const override;
	virtual void applyTransformations() override;
	virtual std::unique_ptr<Surface> clone() const override;

	void setMaxTrianglesPerLeaf(unsigned maxTriangles);
	static void setMaxTreeDepth(unsigned depth);

private:
	std::vector<Point3f> m_VertexBuffer;
	std::vector<Vector3i> m_IndexBuffer;
	std::vector<Vector3f> m_VertexNormals;
	BVHTree m_BVH;

	static unsigned s_MaxTreeDepth;
	unsigned m_MaxTrianglesPerLeaf;

	void calculateVertexNormals();
	bool rayTriangleIntersection(const Vector3i& triangle, const Ray& ray, Context& context, float minT, float maxT, bool culling) const;

	void constructAABB();
	void constructBVH();
	AABB constructTriangleAABB(const Vector3i& triangle);

	static SplitPair splitBoundingVolume(const AABB& aabb, const std::vector<int>& triangleIndexBuffer, const std::vector<AABB>& triangleAABBs, unsigned depth = 0);
	// Objects' geometry must be triangular
	// Only vertecies and face elements are supported at this time!
	void loadObj(const char* filePath);

private:
	struct WorkerNode;
	struct SplitInfo { AABB aabb; std::vector<int> triangleIndexBuffer; };
	struct SplitPair { SplitInfo left; SplitInfo right; };
	struct WorkerInfo { std::vector<WorkerNode> workerNodes; unsigned depth = 0; };

	struct WorkerNode {
		WorkerNode()
			: aabb(Vector3f(), Vector3f()), root(nullptr)
		{}

		WorkerNode(const AABB& aabb, BVHNode* root, const std::vector<int>& triangleIndexBuffer)
			: aabb(aabb), root(root), triangleIndexBuffer(triangleIndexBuffer), bvh(std::make_unique<BVHTree>())
		{}

		AABB aabb;
		BVHNode* root;
		std::vector<int> triangleIndexBuffer;
		std::unique_ptr<BVHTree> bvh;
	};

	class TreeBuilder
	{
	public:
		TreeBuilder(const Mesh* meshData, std::vector<WorkerNode>* workerNodes, const std::vector<AABB>* triangleAABBs, unsigned maxTrianglesPerLeaf, unsigned startingDepth);

		void startConcurrency(unsigned threadCount = 0);
		BVHNode* buildTree(SplitInfo& boxInfo, BVHTree& bvh, WorkerInfo* workerInfo = nullptr, unsigned depth = 0);

	private:
		int m_NextNode;
		unsigned m_StartingDepth;
		unsigned m_MaxTrianglesPerLeaf;

		const Mesh* m_MeshData;
		std::vector<WorkerNode>* m_WorkerNodes;
		const std::vector<AABB>* m_TriangleAABBs;

		static std::mutex s_Mutex;

		void assignWorker();
		bool assignWork(size_t& workIndex);
		void constructTree(WorkerNode& node, const std::vector<AABB>& triangleAABBs);
	};
};

#endif // !MESH_H