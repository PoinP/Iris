#include "Mesh.h"

#include <stack>
#include <thread>
#include <fstream>
#include <Utilities/Timer.h>
#include <Utilities/Utility.h>

#include <Objects/Materials/Diffuse.h>

unsigned Mesh::s_MaxTreeDepth = 30;

Mesh::Mesh(const char* objFile, const std::shared_ptr<Material>& material)
	: Surface(material, AABB(Vector3f(), Vector3f())), m_MaxTrianglesPerLeaf(15)
{
	assert(objFile);
	loadObj(objFile);
	calculateVertexNormals();
	constructAABB();
	constructBVH();
}

Mesh::Mesh(std::vector<Vector3f>&& vertexBuffer, std::vector<Vector3i>&& indexBuffer, const std::shared_ptr<Material>& material)
	: Surface(material, AABB(Vector3f(), Vector3f())), m_MaxTrianglesPerLeaf(15)
{
	std::swap(m_VertexBuffer, vertexBuffer);
	std::swap(m_IndexBuffer, indexBuffer);
	calculateVertexNormals();
	constructAABB();
	constructBVH();
}

bool Mesh::isHit(const Ray& ray, Context& context, float minT, float maxT) const
{
	if (!m_AABB.isHit(ray, maxT, maxT))
		return false;

	std::stack<const BVHNode*> nodes;
	const BVHNode* root = m_BVH.root();
	nodes.push(root);

	bool isHit = false;

	float innerMaxT = std::numeric_limits<float>::max();
	while (!nodes.empty())
	{
		root = nodes.top();
		nodes.pop();

		float temp;
		if (root->AABB.isHit(ray, maxT, temp))
		{
			if (!root->value.empty())
			{
				for (int triangleIndex : root->value)
				{
					if (rayTriangleIntersection(m_IndexBuffer[triangleIndex], ray, context, minT, innerMaxT, m_Material->hasBackfaceCulling()))
					{
						isHit = true;
						innerMaxT = context.distance;
					}
				}
			}
			else
			{
				if (root->left != nullptr)
					nodes.push(root->left);

				if (root->right != nullptr)
					nodes.push(root->right);
			}
		}
	}

	return isHit;
}

void Mesh::applyTransformations()
{
	Matrix4 transfromationMatrix4(m_TransformationMatrix);
	transfromationMatrix4.col3() = m_Position;

	for (Vector3f& vertex : m_VertexBuffer)
		vertex = transfromationMatrix4 * vertex;

	Matrix4 transposedInvertedMatrix = transpose(invertAffine(transfromationMatrix4));
	for (Vector3f& normal : m_VertexNormals)
		normal = transposedInvertedMatrix * normal;

	constructAABB();
	constructBVH();
}

std::unique_ptr<Surface> Mesh::clone() const
{
	return std::make_unique<Mesh>(std::vector<Vector3f>(m_VertexBuffer), std::vector<Vector3i>(m_IndexBuffer), m_Material);
}

void Mesh::setMaxTrianglesPerLeaf(unsigned maxTriangles)
{
	m_MaxTrianglesPerLeaf = maxTriangles;
}

void Mesh::setMaxTreeDepth(unsigned depth)
{
	s_MaxTreeDepth = depth;
}

void Mesh::calculateVertexNormals()
{
	m_VertexNormals.resize(m_VertexBuffer.size());

	for (Vector3i indexes : m_IndexBuffer)
	{
		const Vector3f edge0 = m_VertexBuffer[indexes[1]] - m_VertexBuffer[indexes[0]];
		const Vector3f edge1 = m_VertexBuffer[indexes[2]] - m_VertexBuffer[indexes[0]];
		const Vector3f faceNormal = crossProduct(edge0, edge1);

		m_VertexNormals[indexes[0]] += faceNormal;
		m_VertexNormals[indexes[1]] += faceNormal;
		m_VertexNormals[indexes[2]] += faceNormal;
	}

	for (Vector3f& normal : m_VertexNormals)
		normal.normalize();
}

bool Mesh::rayTriangleIntersection(const Vector3i& triangle, const Ray& ray, Context& context, float minT, float maxT, bool culling) const
{
	const Point3f& v0 = m_VertexBuffer[triangle[0]];
	const Point3f& v1 = m_VertexBuffer[triangle[1]];
	const Point3f& v2 = m_VertexBuffer[triangle[2]];

	const Vector3f edge0 = v1 - v0;
	const Vector3f edge1 = v2 - v0;

	const Vector3f normal = toUnitVector(crossProduct(edge0, edge1));

	// Read up on the Möller-Trumbore algorithm for a better understanding
	const Vector3f dVec = crossProduct(ray.direction(), edge1);
	const float det = dotProduct(dVec, edge0);
	constexpr float deltaEpsilon = 1.0e-7f; // 0.0000001;

	if (culling && dotProduct(ray.direction(), normal) > 0.0f)
		return false;

	if (fabsf(det) < deltaEpsilon)
		return false;

	const float invDet = 1.0f / det;

	const Vector3f oVec = ray.origin() - v0;
	const float u = dotProduct(dVec, oVec) * invDet;

	constexpr float lowerBound = 0.0f - deltaEpsilon;
	constexpr float upperBound = 1.0f + deltaEpsilon;

	// 0 >= u, v <= 1; u + v <= 1
	if (u < lowerBound || u > upperBound)
		return false;

	const Vector3f eVec = crossProduct(oVec, edge0);
	const float v = dotProduct(eVec, ray.direction()) * invDet;

	if (v < lowerBound || v + u > upperBound)
		return false;

	const float t = dotProduct(eVec, edge1) * invDet;

	if (t < minT || t > maxT)
		return false;

	context.distance = t;
	context.uv = Point2f(u, v);
	context.material = m_Material.get();

	const Vector3f& vN0 = m_VertexNormals[triangle[0]];
	const Vector3f& vN1 = m_VertexNormals[triangle[1]];
	const Vector3f& vN2 = m_VertexNormals[triangle[2]];

	// Elegant ray terminator fix by
	// Johannes Hanika
	const Point3f hitPoint = ray.at(t);
	Vector3f tmpu = hitPoint - v1;
	Vector3f tmpv = hitPoint - v2;
	Vector3f tmpw = hitPoint - v0;

	const float dotu = std::min(0.0f, dotProduct(tmpu, vN1));
	const float dotv = std::min(0.0f, dotProduct(tmpv, vN2));
	const float dotw = std::min(0.0f, dotProduct(tmpw, vN0));

	tmpu -= dotu * vN1;
	tmpv -= dotv * vN2;
	tmpw -= dotw * vN0;

	const float w = std::max(0.0f, (1 - u - v));

	context.faceNormal = normal;
	context.hitPoint = hitPoint;
	context.normal = toUnitVector(vN1 * u + vN2 * v + vN0 * w); // Smooth normal
	context.smoothHitPoint = hitPoint + u * tmpu + v * tmpv + w * tmpw;

	return true;
}

void Mesh::constructAABB()
{
	Vector3f min = m_VertexBuffer[0];
	Vector3f max = m_VertexBuffer[0];

	for (Vector3f vertex : m_VertexBuffer)
	{
		for (int axis = 0; axis < 3; axis++)
		{
			min[axis] = std::min(min[axis], vertex[axis]);
			max[axis] = std::max(max[axis], vertex[axis]);
		}
	}

	m_AABB = AABB(min, max);
}

void Mesh::constructBVH()
{
	std::vector<AABB> triangleAABBs;
	triangleAABBs.reserve(m_IndexBuffer.size());
	std::vector<int> triangleIndexBuffer(m_IndexBuffer.size());

	for (int i = 0; i < m_IndexBuffer.size(); i++)
	{
		triangleIndexBuffer[i] = i;
		triangleAABBs.push_back(constructTriangleAABB(m_IndexBuffer[i]));
	}

	constexpr unsigned concurrencyDepth = 6;
	constexpr unsigned powerOfTwo = constexprPow(2, 6);

	WorkerInfo workerInfo;
	workerInfo.depth = concurrencyDepth;
	workerInfo.workerNodes.reserve(powerOfTwo);
	SplitInfo boxInfo{ m_AABB, triangleIndexBuffer };

	m_BVH.clear();

	TreeBuilder builder(this, &workerInfo.workerNodes, &triangleAABBs, m_MaxTrianglesPerLeaf, concurrencyDepth);
	builder.buildTree(boxInfo, m_BVH, &workerInfo);

	if (workerInfo.workerNodes.empty())
		return;

	builder.startConcurrency();

	for (WorkerNode& node : workerInfo.workerNodes)
		node.bvh->transferAllocatedBlocks(m_BVH);
}

AABB Mesh::constructTriangleAABB(const Vector3i& triangle)
{
	Vector3f min = m_VertexBuffer[triangle[0]];
	Vector3f max = m_VertexBuffer[triangle[0]];

	for (int vertexIndex = 1; vertexIndex < 3; vertexIndex++)
	{
		const Vector3f& vertex = m_VertexBuffer[triangle[vertexIndex]];
		for (int axis = 0; axis < 3; axis++)
		{
			min[axis] = std::min(min[axis], vertex[axis]);
			max[axis] = std::max(max[axis], vertex[axis]);
		}
	}

	return AABB(min, max);
}

Mesh::SplitPair Mesh::splitBoundingVolume(const AABB& aabb, const std::vector<int>& triangleIndexBuffer, const std::vector<AABB>& triangleAABBs, unsigned depth)
{
	std::vector<int> leftNodeIndexes;
	leftNodeIndexes.reserve(triangleIndexBuffer.size());
	std::vector<int> rightNodeIndexes;
	rightNodeIndexes.reserve(triangleIndexBuffer.size());

	BoxPair pair = aabb.split(depth % 3);

	for (int index : triangleIndexBuffer)
	{
		const AABB& triangleAABB = triangleAABBs[index];

		if (pair.leftBox.intersects(triangleAABB))
			leftNodeIndexes.push_back(index);
		if (pair.rightBox.intersects(triangleAABB))
			rightNodeIndexes.push_back(index);
	}

	SplitInfo leftSplit = { pair.leftBox, std::move(leftNodeIndexes) };
	SplitInfo rightSplit = { pair.rightBox, std::move(rightNodeIndexes) };

	return SplitPair{ leftSplit, rightSplit };
}

void Mesh::loadObj(const char* filePath)
{
	std::ifstream inputStream(filePath);

	if (!inputStream || !inputStream.is_open())
		throw std::invalid_argument("File does not exist!");

	std::string buffer;

	int v0, v1, v2;
	float x, y, z;
	char c;

	while (inputStream)
	{
		c = inputStream.get();
		if (c == 'v')
		{
			inputStream >> x >> y >> z;
			m_VertexBuffer.emplace_back(x, y, z);
			inputStream.get();
			continue;
		}

		if (c == 'f')
		{
			inputStream >> v0 >> v1 >> v2;

			m_IndexBuffer.emplace_back(v0 - 1, v1 - 1, v2 - 1);
			inputStream.get();
			continue;
		}

		std::getline(inputStream, buffer);
	}

	if (!inputStream && !inputStream.eof())
		throw std::invalid_argument("Invalid file structure!");

	inputStream.close();
}

Mesh::TreeBuilder::TreeBuilder(const Mesh* meshData, std::vector<WorkerNode>* workerNodes, const std::vector<AABB>* triangleAABBs, unsigned maxTrianglesPerLeaf, unsigned startingDepth)
	: m_NextNode(0), m_WorkerNodes(workerNodes), m_TriangleAABBs(triangleAABBs), m_MaxTrianglesPerLeaf(maxTrianglesPerLeaf), m_StartingDepth(startingDepth)
{
}

void Mesh::TreeBuilder::startConcurrency(unsigned threadCount)
{
	std::vector<std::thread> threadVector;
	int threads = threadCount != 0 ? threadCount : std::thread::hardware_concurrency();

	for (int i = 0; i < threads; i++)
		threadVector.emplace_back(&Mesh::TreeBuilder::assignWorker, this);

	for (std::thread& thread : threadVector)
		thread.join();
}

BVHNode* Mesh::TreeBuilder::buildTree(SplitInfo& boxInfo, BVHTree& bvh, WorkerInfo* workerInfo, unsigned depth)
{
	const AABB& aabb = boxInfo.aabb;
	std::vector<int>& triangleIndexBuffer = boxInfo.triangleIndexBuffer;

	if (boxInfo.triangleIndexBuffer.size() == 0)
		return nullptr;

	BVHNode* root = bvh.createNode(boxInfo.aabb);

	if (boxInfo.triangleIndexBuffer.size() <= m_MaxTrianglesPerLeaf || depth >= s_MaxTreeDepth)
	{
		boxInfo.triangleIndexBuffer.shrink_to_fit();
		root->value = std::move(boxInfo.triangleIndexBuffer);
		return root;
	}

	if (workerInfo != nullptr && depth >= workerInfo->depth)
	{
		workerInfo->workerNodes.emplace_back(aabb, root, std::move(triangleIndexBuffer));
		return root;
	}

	SplitPair pair = Mesh::splitBoundingVolume(boxInfo.aabb, boxInfo.triangleIndexBuffer, *m_TriangleAABBs, depth);

	root->left = buildTree(pair.left, bvh, workerInfo, depth + 1);
	root->right = buildTree(pair.right, bvh, workerInfo, depth + 1);

	return root;
}

void Mesh::TreeBuilder::assignWorker()
{
	size_t workIndex = 0;

	while (assignWork(workIndex))
		constructTree(m_WorkerNodes->operator[](workIndex), *m_TriangleAABBs);
}

bool Mesh::TreeBuilder::assignWork(size_t& workIndex)
{
	std::lock_guard<std::mutex> lock(s_Mutex);
	workIndex = m_NextNode++;
	return workIndex < m_WorkerNodes->size();
}

void Mesh::TreeBuilder::constructTree(WorkerNode& node, const std::vector<AABB>& triangleAABBs)
{
	SplitPair pair = Mesh::splitBoundingVolume(node.aabb, node.triangleIndexBuffer, *m_TriangleAABBs, m_StartingDepth);

	node.root->left = buildTree(pair.left, *node.bvh, nullptr, m_StartingDepth);
	node.root->right = buildTree(pair.right, *node.bvh, nullptr, m_StartingDepth);
}

std::mutex Mesh::TreeBuilder::s_Mutex{};
