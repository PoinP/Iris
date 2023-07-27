#include <iostream>

#include <Core/Scene.h>
#include <Core/Renderer.h>

#include <OpenGL/Window.h>
#include <OpenGL/Shader.h>
#include <OpenGL/Texture.h>
#include <OpenGL/GLRenderer.h>
#include <OpenGL/IndexBuffer.h>
#include <OpenGL/VertexArray.h>
#include <OpenGL/VertexBuffer.h>

void render(const Scene& scene, unsigned sampleCount, const std::string& outputPath);
void renderWithUI(const Scene& scene, unsigned sampleCount, const std::string& outputPath);

int main()
{
	const int cornellSampleCount = 1200;
	const int bigScenesampleCount = 10;

	// Note: This scene is in the repo bit it is zipped inside the folder!
	//Scene scene("scenes\\custom\\big_scene.crtscene");
	//std::vector<std::string> paths = {
	//	"textures\\Yokahoma3\\posx.jpg", "textures\\Yokahoma3\\posy.jpg", "textures\\Yokahoma3\\posz.jpg",
	//	"textures\\Yokahoma3\\negx.jpg", "textures\\Yokahoma3\\negy.jpg", "textures\\Yokahoma3\\negz.jpg"
	//};

	//scene.setBackground(CubeMap(paths));

	Scene scene("scenes\\custom\\cornell_box.crtscene");

	renderWithUI(scene, cornellSampleCount, "out\\cornell-box.ppm");
}

void render(const Scene& scene, unsigned sampleCount, const std::string& outputPath)
{
	Renderer renderer;
	renderer.render(scene, sampleCount);
	renderer.saveRender(outputPath.c_str());
}

void renderWithUI(const Scene& scene, unsigned sampleCount, const std::string& outputPath)
{
	const SceneSettings& settings = scene.settings();
	GL::Window window({ settings.width - 100, settings.height - 100, "Iris" });

	float verticies[] =
	{
		-1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 0.0f
	};

	unsigned indexes[] =
	{
		0, 1, 2,
		3, 2, 1
	};

	GL::VertexArray vertexArray;

	GL::VertexBuffer vertexBuffer(verticies, sizeof(verticies));
	vertexBuffer.addAttribute(0, 3, GL_FLOAT);
	vertexBuffer.addAttribute(1, 2, GL_FLOAT);
	vertexArray.addVertexAttributes(vertexBuffer);

	GL::IndexBuffer indexBuffer(indexes, sizeof(indexes));

	GL::Shader shader("src\\OpenGL\\Shaders\\vertex.glsl", "src\\OpenGL\\Shaders\\fragment.glsl");
	shader.setUniform1i("u_TextureSampler", 0);

	Renderer renderer;
	std::thread thread([&renderer, &scene, sampleCount]() { renderer.render(scene, sampleCount); });

	GL::Texture texture(Image(scene.settings().width, scene.settings().height));

	while (!window.shouldClose())
	{
		GL::GLRenderer::clear();

		texture.update(renderer.image());
		texture.bind();

		GL::GLRenderer::draw(vertexArray, indexBuffer, shader);

		window.update();
	}

	Renderer::shouldStop(true);
	GL::Window::terminate();

	thread.join();
	renderer.saveRender(outputPath.c_str());
}