#include "Renderer.h"

#include <Utilities/Image.h>
#include <Utilities/Timer.h>
#include <Utilities/Utility.h>
#include <Objects/Material.h>

#include <algorithm>

#define MULTI_THREADING

unsigned Renderer::s_MaxDepth = 10;
bool Renderer::s_ShouldStop = false;

Renderer::Renderer()
	: m_Image(Image(1, 1))
{};

void Renderer::render(const Scene& scene, unsigned sampleCount, unsigned threadCount)
{
	s_ShouldStop = false;
	const SceneSettings& settings = scene.settings();
	m_Image = Image(settings.width, settings.height);

#ifdef MULTI_THREADING

	const unsigned bucketSize = scene.settings().bucketSize;
	Worker::createBuckets(bucketSize, scene);

	threadCount = threadCount == 0 ? std::thread::hardware_concurrency() : threadCount;

	std::vector<std::thread> threads;
	threads.reserve(threadCount);

	std::cout << "Rendering..." << std::endl;

	{
		Timer t;
		for (size_t i = 0; i < threadCount; i++)
			threads.emplace_back(Worker{}, std::ref(scene), std::ref(m_Image), sampleCount);

		for (std::thread& thread : threads)
			thread.join();

		std::cout << "Render complete!\n";
	}

#else
	const Camera& camera = scene.camera();

	const Color backgroundColor = settings.background;
	const unsigned height = settings.height;
	const unsigned width = settings.width;

	std::vector<Ray> rays;
	rays.reserve(sampleCount);

	std::cout << "Rendering..." << std::endl;
	{
		Timer t;
		for (unsigned row = 0; row < height; row++)
		{
			std::cout << "Progress: " << (int)(((row + 1) / (float)height) * 100) << "%\r";
			for (unsigned col = 0; col < width; col++)
			{
				Color color;
				camera.generateRays(col, row, rays, sampleCount);

				for (unsigned i = 0; i < sampleCount; i++)
					color += traceRay(rays[i], scene);

				rays.clear();
				color /= sampleCount;
				color = clamp(color, 0.0f, 1.0f);
				m_Image.setPixel(col, row, color);
			}
		}
	}

#endif // MULTI-THREADING

	std::cout << "\n" << std::endl;
}

void Renderer::saveRender(const char* filePath)
{
	assert(filePath);
	m_Image.writeToPPM(filePath);
}

void Renderer::setMaxDepth(unsigned maxDepth)
{
	s_MaxDepth = maxDepth;
}

const Image& Renderer::image() const
{
	return m_Image;
}

Color Renderer::traceRay(const Ray& ray, const Scene& scene)
{
	if (ray.depth() >= s_MaxDepth)
		return Color();

	Surface::Context context;

	if (!scene.isHit(ray, context, 0.000001f, std::numeric_limits<float>::max()))
		return scene.settings().background.getColor(ray);

	return context.material->shadeScene(ray, scene, context);
}

void Renderer::shouldStop(bool stop)
{
	s_ShouldStop = stop;
}

Renderer::Worker::Worker()
	: m_Id(s_NextId++), m_BlockX(0), m_BlockY(0)
{
}

void Renderer::Worker::operator()(const Scene& scene, Image& image, unsigned sampleCount)
{
	const Camera& camera = scene.camera();

	std::vector<Ray> rays;
	rays.reserve(sampleCount);

	assert(s_Buckets.size() > 0);

	while (assignBucket())
	{
		if (s_ShouldStop)
			return;

		if (m_Id == 0)
			std::cout << "Progress: " << (int)((s_CompletedBuckets) / (float)s_BucketCount * 100) << "%\r";

		for (unsigned row = m_BlockY; row < (m_BlockY + s_BucketSize); row++)
		{
			for (unsigned col = m_BlockX; col < (m_BlockX + s_BucketSize); col++)
			{
				if (row >= s_ImageHeight || col >= s_ImageWidth)
					break;

				Color color;
				camera.generateRays(col, row, rays, sampleCount);

				for (unsigned i = 0; i < sampleCount; i++)
					color += traceRay(rays[i], scene);

				rays.clear();
				color *= 1.0f / sampleCount;
				color = clamp(color, 0.0f, 1.0f);
				image.setPixel(col, row, color);
			}
		}

		s_CompletedBuckets++;
	}
}

void Renderer::Worker::createBuckets(unsigned bucketSize, const Scene& scene)
{
	s_RenderFlag = false;

	s_NextId = 0;
	s_CompletedBuckets = 0;
	s_BucketSize = bucketSize;
	s_ImageWidth = scene.settings().width;
	s_ImageHeight = scene.settings().height;

	s_BucketWCap = (s_ImageWidth + s_BucketSize - 1) / s_BucketSize;
	s_BucketHCap = (s_ImageHeight + s_BucketSize - 1) / s_BucketSize;
	s_BucketCount = s_BucketHCap * s_BucketWCap;

	s_Buckets.resize(s_BucketHCap);
	for (std::vector<bool>& bucket : s_Buckets)
	{
		bucket.clear();
		bucket.resize(s_BucketWCap);
	}
}

bool Renderer::Worker::assignBucket()
{
	bool hasAssigned = false;

	std::lock_guard<std::mutex> lock(s_Mutex);
	{
		int x, y;
		while (!hasAssigned)
		{
			fecthNextBucket(x, y);
			if (y < 0 || y >= s_BucketHCap || x < 0 || x >= s_BucketWCap)
				break;

			if (!s_Buckets[y][x])
			{
				m_BlockX = x * s_BucketSize;
				m_BlockY = y * s_BucketSize;
				s_Buckets[y][x] = true;

				hasAssigned = true;
			}
		}

		return hasAssigned;
	}
}

// Spiral pattern
void Renderer::Worker::fecthNextBucket(int& varX, int& varY)
{
	static int x = -1;
	static int y = 0;

	static int dx = 1;
	static int dy = 0;
	static int t = 0;
	static int cap = 0;

	if (!s_RenderFlag)
	{
		x = -1;
		y = 0;

		dx = 1;
		dy = 0;
		t = 0;
		cap = 0;
		s_RenderFlag = true;
	}

	if ((dx == 1 && x - s_BucketWCap + cap + 1 == 0) || (dx == -1 && x == cap)
		|| (dy == 1 && y - s_BucketHCap + cap + 1 == 0) || (dy == -1 && y == cap))
	{
		t = dx;
		dx = -dy;
		dy = t;

		if (x == cap && y == s_BucketHCap - cap - 1)
			cap++;
	}

	x += dx;
	y += dy;

	varX = x;
	varY = y;
}

std::vector<std::vector<bool>> Renderer::Worker::s_Buckets;
std::atomic<unsigned> Renderer::Worker::s_CompletedBuckets{ 0 };

unsigned Renderer::Worker::s_NextId{};
unsigned Renderer::Worker::s_BucketSize{};
unsigned Renderer::Worker::s_BucketWCap{};
unsigned Renderer::Worker::s_BucketHCap{};
unsigned Renderer::Worker::s_BucketCount{};
unsigned Renderer::Worker::s_ImageWidth{};
unsigned Renderer::Worker::s_ImageHeight{};
bool Renderer::Worker::s_RenderFlag{};

std::mutex Renderer::Worker::s_Mutex{};