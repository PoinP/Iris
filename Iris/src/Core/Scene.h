#ifndef SCENE_H

#define SCENE_H

#include <memory>

#include <Core/AABB.h>
#include <Core/Camera.h>
#include <Core/CubeMap.h>
#include <Objects/Light.h>
#include <Utilities/Image.h>
#include <Containers/Color.h>
#include <Objects/Surfaces/Mesh.h>

#include <rapidjson/document.h>

struct SceneFlags
{
	bool reflections = true;
	bool refractions = true;
	bool globalIllumination = false;
};

struct SceneSettings
{
	unsigned width{};
	unsigned height{};
	unsigned bucketSize{};
	CubeMap background{};
	SceneFlags flags{};
};

class Scene
{
public:
	Scene(const char* scenePath);
	Scene(const Camera& camera, const SceneSettings& settings);

	Scene(const Scene& other);
	Scene(Scene&& other) noexcept;

	Scene& operator=(const Scene& other);
	Scene& operator=(Scene&& other) noexcept;

	bool isHit(const Ray& ray, Surface::Context& context, float minT, float maxT) const;

	const SceneSettings& settings() const;
	SceneSettings& settings();
	void setResolution(unsigned width, unsigned height);
	void setBackground(const Color& color);
	void setBackground(const CubeMap& cubeMap);

	const Camera& camera() const;
	Camera& camera();
	void setCamera(const Camera& camera);

	void applyObjectTransformations();

	const std::vector<std::unique_ptr<Surface>>& objects() const;
	std::vector<std::unique_ptr<Surface>>& objects();

	const std::vector<Light>& lights() const;
	std::vector<Light>& lights();

	/// Access an object in the scene
	const std::unique_ptr<Surface>& operator[](size_t index) const;
	std::unique_ptr<Surface>& operator[](size_t index);

	void addObject(std::unique_ptr<Surface>&& object);
	void addObjects(std::vector<std::unique_ptr<Surface>>& objects);

private:
	AABB m_AABB;
	Camera m_Camera;
	SceneSettings m_Settings;
	std::vector<Light> m_Lights;
	std::vector<std::unique_ptr<Surface>> m_Objects;
	std::vector<std::shared_ptr<Material>> m_Materials;

	void constructAABB();
	void loadScene(const char* scenePath);
	bool loadSettings(const rapidjson::Document& json);
	bool loadCamera(const rapidjson::Document& json);
	bool loadLights(const rapidjson::Document& json);
	bool loadMaterials(const rapidjson::Document& json);
	bool loadObjects(const rapidjson::Document& json);

	rapidjson::Document parseJson(const char* jsonPath);
	Vector3i loadVector3i(const rapidjson::GenericArray<true, rapidjson::Value>& array);
	Vector3f loadVector3f(const rapidjson::GenericArray<true, rapidjson::Value>& array);
	Matrix3 loadMatrix3(const rapidjson::GenericArray<true, rapidjson::Value>& array);

	SceneFlags loadFlags(const rapidjson::Value& settings);
	bool loadFlag(const std::string& flagString, const rapidjson::Value& settings, bool defaultValue = true);

	std::unique_ptr<Mesh> loadMesh(const rapidjson::Value& object);
	std::shared_ptr<Material> loadMaterial(const rapidjson::Value& material);
	std::vector<Vector3f> loadVertexBuffer(const rapidjson::GenericArray<true, rapidjson::Value>& array);
	std::vector<Vector3i> loadIndexBuffer(const rapidjson::GenericArray<true, rapidjson::Value>& array);

	void copy(const Scene& other);
	void swap(Scene&& other);
};

#endif // !SCENE_H