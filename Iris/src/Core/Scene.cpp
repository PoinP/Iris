#include "Scene.h"

#include <fstream>
#include <cassert>
#include <stdexcept>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include <Objects/Materials/Diffuse.h>
#include <Objects/Materials/Constant.h>
#include <Objects/Materials/Reflective.h>
#include <Objects/Materials/Refractive.h>
#include <Objects/Materials/Emissive.h>

using namespace rapidjson;

Scene::Scene(const char* scenePath)
	: m_AABB(Vector3f(), Vector3f()), m_Camera(Point3f(), 1, 1)
{
	assert(scenePath);
	loadScene(scenePath);
	constructAABB();
}

Scene::Scene(const Camera& camera, const SceneSettings& settings)
	: m_AABB(Vector3f(), Vector3f()), m_Camera(camera), m_Settings(settings)
{
}

Scene::Scene(const Scene& other)
	: m_AABB(other.m_AABB), m_Camera(other.camera()), m_Settings(other.m_Settings)
{
	m_Objects.reserve(other.m_Objects.capacity());
	for (const std::unique_ptr<Surface>& object : other.m_Objects)
		m_Objects.emplace_back(object->clone());
}

Scene::Scene(Scene&& other) noexcept
	: m_AABB(Vector3f(), Vector3f()), m_Camera(Point3f(), 1, 1)
{
	swap(std::move(other));
}

Scene& Scene::operator=(const Scene& other)
{
	if (this != &other)
	{
		m_Objects.clear();
		copy(other);
	}

	return *this;
}

Scene& Scene::operator=(Scene&& other) noexcept
{
	swap(std::move(other));
	return *this;
}

bool Scene::isHit(const Ray& ray, Surface::Context& context, float minT, float maxT) const
{
	if (!m_AABB.isHit(ray, maxT, maxT))
		return false;

	bool isHit = false;
	Surface::Context tempContext;

	for (const std::unique_ptr<Surface>& surface : m_Objects) {
		if (surface->isHit(ray, tempContext, minT, maxT))
		{
			context = tempContext;
			maxT = context.distance;

			isHit = true;
		}
	}

	return isHit;
}

const SceneSettings& Scene::settings() const
{
	return m_Settings;
}

SceneSettings& Scene::settings()
{
	return m_Settings;
}

void Scene::setResolution(unsigned width, unsigned height)
{
	assert(width > 0 && height > 0);
	m_Settings.width = width;
	m_Settings.height = height;
	m_Camera.setResolution(width, height);
}

void Scene::setBackground(const Color& color)
{
	m_Settings.background = color;
}

void Scene::setBackground(const CubeMap& cubeMap)
{
	m_Settings.background = cubeMap;
}

const Camera& Scene::camera() const
{
	return m_Camera;
}

Camera& Scene::camera()
{
	return m_Camera;
}

void Scene::setCamera(const Camera& camera)
{
	m_Camera = camera;
}

void Scene::applyObjectTransformations()
{
	for (std::unique_ptr<Surface>& object : m_Objects)
		object->applyTransformations();
	constructAABB();
}


const std::vector<std::unique_ptr<Surface>>& Scene::objects() const
{
	return m_Objects;
}

std::vector<std::unique_ptr<Surface>>& Scene::objects()
{
	return m_Objects;
}

const std::vector<Light>& Scene::lights() const
{
	return m_Lights;
}

std::vector<Light>& Scene::lights()
{
	return m_Lights;
}

const std::unique_ptr<Surface>& Scene::operator[](size_t index) const
{
	assert(index < m_Objects.size());
	return m_Objects[index];
}

std::unique_ptr<Surface>& Scene::operator[](size_t index)
{
	assert(index < m_Objects.size());
	return m_Objects[index];
}

void Scene::addObject(std::unique_ptr<Surface>&& object)
{
	m_AABB.combine(object->getAABB());
	m_Objects.emplace_back(std::move(object));
}

void Scene::addObjects(std::vector<std::unique_ptr<Surface>>& objects)
{
	for (std::unique_ptr<Surface>& object : objects)
		addObject(std::move(object));
}

void Scene::constructAABB()
{
	Vector3f min = m_Objects[0]->getAABB().min();
	Vector3f max = m_Objects[0]->getAABB().max();

	for (std::unique_ptr<Surface>& surface : m_Objects)
	{
		const AABB& aabb = surface->getAABB();

		min.x() = std::min(min.x(), aabb.min().x());
		min.y() = std::min(min.y(), aabb.min().y());
		min.z() = std::min(min.z(), aabb.min().z());

		max.x() = std::max(max.x(), aabb.max().x());
		max.y() = std::max(max.y(), aabb.max().y());
		max.z() = std::max(max.z(), aabb.max().z());
	}

	m_AABB = AABB(min, max);
}

void Scene::loadScene(const char* scenePath)
{
	Document json = parseJson(scenePath);

	loadSettings(json);
	loadCamera(json);
	loadLights(json);
	loadMaterials(json);
	loadObjects(json);
}

bool Scene::loadSettings(const rapidjson::Document& json)
{
	const Value& settings = json.FindMember("settings")->value;
	if (!settings.IsNull() && settings.IsObject())
	{
		const Value& backgroundColor = settings.FindMember("background_color")->value;
		assert(!backgroundColor.IsNull() && backgroundColor.IsArray());
		m_Settings.background = static_cast<Color>(loadVector3f(backgroundColor.GetArray()));

		const Value& imageSettings = settings.FindMember("image_settings")->value;
		assert(!imageSettings.IsNull() && imageSettings.IsObject());

		const Value& width = imageSettings.FindMember("width")->value;
		const Value& height = imageSettings.FindMember("height")->value;
		assert(!width.IsNull() && !height.IsNull() && width.IsUint() && height.IsUint());

		m_Settings.bucketSize = 32;
		const Value::ConstMemberIterator& bucketSize = imageSettings.FindMember("bucket_size");
		if (bucketSize != imageSettings.MemberEnd())
		{
			assert(!bucketSize->value.IsNull() && bucketSize->value.IsInt());
			m_Settings.bucketSize = bucketSize->value.GetInt();
		}

		m_Settings.width = width.GetUint();
		m_Settings.height = height.GetUint();
		m_Settings.flags = loadFlags(settings);

		return true;
	}

	return false;
}

bool Scene::loadCamera(const rapidjson::Document& json)
{
	const Value& camera = json.FindMember("camera")->value;
	if (!camera.IsNull() && camera.IsObject())
	{
		const Value& matrix = camera.FindMember("matrix")->value;
		assert(!matrix.IsNull() && matrix.IsArray());
		Matrix3 rotationMatrix = loadMatrix3(matrix.GetArray());

		const Value& position = camera.FindMember("position")->value;
		assert(!position.IsNull() && position.IsArray());
		Vector3f cameraPosition = loadVector3f(position.GetArray());

		float FOV = 90.0f;
		const Value::ConstMemberIterator& cameraFOV = camera.FindMember("fov");
		if (cameraFOV != camera.MemberEnd())
		{
			assert(!cameraFOV->value.IsNull() && cameraFOV->value.IsFloat());
			FOV = cameraFOV->value.GetFloat();
		}

		m_Camera = Camera(cameraPosition, m_Settings.width, m_Settings.height, FOV);
		m_Camera.transform(rotationMatrix);

		return true;
	}

	return false;
}

bool Scene::loadLights(const rapidjson::Document& json)
{
	const Value& lights = json.FindMember("lights")->value;
	if (!lights.IsNull() && lights.IsArray())
	{
		const GenericArray<true, Value>& lightArray = lights.GetArray();
		for (const Value& light : lightArray)
		{
			const Value& intensity = light.FindMember("intensity")->value;
			assert(!intensity.IsNull() && intensity.IsFloat());
			const Value& position = light.FindMember("position")->value;
			assert(!position.IsNull() && position.IsArray());

			m_Lights.emplace_back(loadVector3f(position.GetArray()), intensity.GetFloat());
		}

		return true;
	}

	return false;
}

bool Scene::loadMaterials(const rapidjson::Document& json)
{
	const Value& materials = json.FindMember("materials")->value;
	if (!materials.IsNull() && materials.IsArray())
	{
		const GenericArray<true, Value>& materialArray = materials.GetArray();
		for (const Value& material : materialArray)
			m_Materials.emplace_back(loadMaterial(material));

		return true;
	}

	return false;
}

bool Scene::loadObjects(const rapidjson::Document& json)
{
	const Value& objects = json.FindMember("objects")->value;
	if (!objects.IsNull() && objects.IsArray())
	{
		const GenericArray<true, Value>& objectBuffer = objects.GetArray();
		for (const Value& object : objectBuffer)
			m_Objects.emplace_back(loadMesh(object));

		return true;
	}

	return false;
}

rapidjson::Document Scene::parseJson(const char* jsonPath)
{
	std::ifstream inputStream(jsonPath);

	if (!inputStream || !inputStream.is_open())
		throw std::invalid_argument("Error opening file! It may not exist!");

	IStreamWrapper streamWrapper(inputStream);
	Document json;

	json.ParseStream(streamWrapper);

	inputStream.close();

	if (json.HasParseError())
		throw std::invalid_argument("Error parsing the json scene file");

	return json;
}

bool Scene::loadFlag(const std::string& flagString, const rapidjson::Value& settings, bool defaultValue)
{
	const Value::ConstMemberIterator flag = settings.FindMember(flagString.c_str());

	if (flag != settings.MemberEnd())
	{
		assert(!flag->value.IsNull() && flag->value.IsBool());
		return flag->value.GetBool();
	}

	return defaultValue;
}

SceneFlags Scene::loadFlags(const rapidjson::Value& settings)
{
	SceneFlags flags;
	flags.reflections = loadFlag("reflections_on", settings);
	flags.refractions = loadFlag("refractions_on", settings);
	flags.globalIllumination = loadFlag("gi_on", settings, false);

	return flags;
}

Vector3i Scene::loadVector3i(const rapidjson::GenericArray<true, rapidjson::Value>& array)
{
	assert(array.Size() == 3);
	return Vector3i(
		array[0].GetInt(),
		array[1].GetInt(),
		array[2].GetInt()
	);
}

Vector3f Scene::loadVector3f(const rapidjson::GenericArray<true, rapidjson::Value>& array)
{
	assert(array.Size() == 3);
	return Vector3f(
		array[0].GetFloat(),
		array[1].GetFloat(),
		array[2].GetFloat()
	);
}

Matrix3 Scene::loadMatrix3(const rapidjson::GenericArray<true, rapidjson::Value>& array)
{
	assert(array.Size() == 9);
	return Matrix3(
		Vector3f(array[0].GetFloat(), array[1].GetFloat(), array[2].GetFloat()),
		Vector3f(array[3].GetFloat(), array[4].GetFloat(), array[5].GetFloat()),
		Vector3f(array[6].GetFloat(), array[7].GetFloat(), array[8].GetFloat())
	);
}

std::unique_ptr<Mesh> Scene::loadMesh(const rapidjson::Value& object)
{
	const Value& vertices = object.FindMember("vertices")->value;
	assert(!vertices.IsNull() && vertices.IsArray());
	std::vector<Vector3f> vertexBuffer = loadVertexBuffer(vertices.GetArray());

	const Value& indexes = object.FindMember("triangles")->value;
	assert(!indexes.IsNull() && indexes.IsArray());
	std::vector<Vector3i> indexBuffer = loadIndexBuffer(indexes.GetArray());

	const Value& materialIndex = object.FindMember("material_index")->value;
	assert(!materialIndex.IsNull() && materialIndex.IsInt());
	int materialIdx = materialIndex.GetInt();
	assert(materialIdx >= 0);

	return std::make_unique<Mesh>(std::move(vertexBuffer), std::move(indexBuffer), m_Materials[materialIdx]);
}

std::shared_ptr<Material> Scene::loadMaterial(const rapidjson::Value& material)
{
	const Value& type = material.FindMember("type")->value;
	assert(!type.IsNull() && type.IsString());
	std::string materialType = type.GetString();

	const Value& shading = material.FindMember("smooth_shading")->value;
	assert(!shading.IsNull() && shading.IsBool());
	bool smoothShading = shading.GetBool();

	const Value::ConstMemberIterator& culling = material.FindMember("back_face_culling");
	bool backfaceCulling = false;
	if (culling != material.MemberEnd())
	{
		assert(!culling->value.IsNull() && culling->value.IsBool());
		backfaceCulling = culling->value.GetBool();
	}

	const Value& albedo = material.FindMember("albedo")->value;
	assert(!albedo.IsNull() && albedo.IsArray());
	const Color color = static_cast<Color>(loadVector3f(albedo.GetArray()));

	if (materialType == "diffuse") return std::make_shared<Diffuse>(color, smoothShading, backfaceCulling);
	if (materialType == "constant") return std::make_shared<Constant>(color, smoothShading, backfaceCulling);

	if (materialType == "reflective")
	{
		float roughnessValue = 0.0f;
		const Value::ConstMemberIterator& roughness = material.FindMember("roughness");
		if (roughness != material.MemberEnd())
		{
			assert(!roughness->value.IsNull() && roughness->value.IsFloat());
			roughnessValue = roughness->value.GetFloat();
		}

		return std::make_shared<Reflective>(color, 0.95f, roughnessValue, smoothShading, backfaceCulling);
	}

	if (materialType == "emissive")
	{
		const Value& strength = material.FindMember("strength")->value;
		assert(!strength.IsNull() && albedo.IsFloat());
		const float strengthValue = strength.GetFloat();
		return std::make_shared<Emissive>(color, strengthValue, smoothShading, backfaceCulling);
	}

	if (materialType == "refractive")
	{
		const Value& ior = material.FindMember("ior")->value;
		assert(!ior.IsNull() && ior.IsFloat());
		const float indexOfRefraction = ior.GetFloat();

		return std::make_shared<Refractive>(Color(1.0f, 1.0f, 1.0f), indexOfRefraction, smoothShading, backfaceCulling);
	}

	assert(false);
	return nullptr;
}

std::vector<Vector3f> Scene::loadVertexBuffer(const rapidjson::GenericArray<true, rapidjson::Value>& array)
{
	assert(array.Size() % 3 == 0);

	std::vector<Vector3f> vertexBuffer;
	vertexBuffer.reserve(array.Size() / 3);

	for (Value::ConstValueIterator itr = array.Begin(); itr != array.End(); itr += 3)
	{
		vertexBuffer.emplace_back(itr->GetFloat(), (itr + 1)->GetFloat(), (itr + 2)->GetFloat());
	}

	return vertexBuffer;
}

std::vector<Vector3i> Scene::loadIndexBuffer(const rapidjson::GenericArray<true, rapidjson::Value>& array)
{
	assert(array.Size() % 3 == 0);

	std::vector<Vector3i> indexBuffer;
	indexBuffer.reserve(array.Size() / 3);

	for (Value::ConstValueIterator itr = array.Begin(); itr != array.End(); itr += 3)
	{
		indexBuffer.emplace_back(itr->GetFloat(), (itr + 1)->GetFloat(), (itr + 2)->GetFloat());
	}

	return indexBuffer;
}

void Scene::copy(const Scene& other)
{
	m_Settings = other.m_Settings;
	m_Camera = other.m_Camera;

	m_Objects.reserve(other.m_Objects.capacity());
	for (const std::unique_ptr<Surface>& object : other.m_Objects)
		m_Objects.emplace_back(object->clone());
}

void Scene::swap(Scene&& other)
{
	std::swap(m_Settings, other.m_Settings);
	std::swap(m_Camera, other.m_Camera);
	std::swap(m_Objects, other.m_Objects);
}