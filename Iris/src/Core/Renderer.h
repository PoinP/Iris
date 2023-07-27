#ifndef RENDERER_H

#define RENDERER_H

#include <vector>
#include <memory>

#include <Core/Camera.h>
#include <Core/Scene.h>

#include <Containers/Vector2.h>
#include <Containers/Color.h>

#include <Objects/Surface.h>

#include <cassert>
#include <thread>
#include <mutex>

class Renderer
{
public:
	Renderer();
	void render(const Scene& scene, unsigned sampleCount, unsigned threadCount = 0);
	void saveRender(const char* filePath);

	const Image& image() const;
	
	static void setMaxDepth(unsigned maxDepth);
	static Color traceRay(const Ray& ray, const Scene& scene);
	static void shouldStop(bool stop);

private:
	Image m_Image;
	static unsigned s_MaxDepth;
	static bool s_ShouldStop;

private:
	class Worker
	{
	public:
		Worker();
		void operator()(const Scene& scene, Image& image, unsigned sampleCount);
		static void createBuckets(unsigned bucketSize, const Scene& scene);

	private:
		bool assignBucket();
		void fecthNextBucket(int& varX, int& varY);

	private:
		const unsigned m_Id;
		unsigned m_BlockX;
		unsigned m_BlockY;

		static std::vector<std::vector<bool>> s_Buckets;
		static std::atomic<unsigned> s_CompletedBuckets;

		static unsigned s_NextId;
		static unsigned s_BucketSize;
		static unsigned s_BucketWCap;
		static unsigned s_BucketHCap;
		static unsigned s_BucketCount;
		static unsigned s_ImageWidth;
		static unsigned s_ImageHeight;

		static bool s_RenderFlag; // < Tracks if a new render has started

		static std::mutex s_Mutex;

	};
};

#endif // !RENDERER_H