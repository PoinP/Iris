#ifndef GL_WINDOW_H

#define GL_WINDOW_H

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace GL
{
	struct WindowProperties
	{
		unsigned width, height;
		std::string title;
	};

	enum class MouseMode
	{
		DISABLED,
		HIDDEN,
		NORMAL
	};

	class Window
	{
	public:
		Window(const WindowProperties& windowProps);
		Window(const Window& other);
		~Window();

		Window& operator= (const Window& other);

		void setKeyCallback(void (*callback)(GLFWwindow*, int key, int scancode, int action, int mods));
		void setMousePosCallback(void (*callback)(GLFWwindow*, double xPos, double yPos));
		void setMouseMode(MouseMode mode);

		void update() const;
		void setContextCurrent() const;

		bool shouldClose() const;
		void setShouldClose();

		unsigned getWidth() const;
		unsigned getHeight() const;

		GLFWwindow* getWindow();

		static void terminate();

	private:
		GLFWwindow* m_Window;
		WindowProperties m_Props;
		static bool s_HasInitalizedGLFW;

		void init();
	};
}

#endif // !GL_WINDOW_H