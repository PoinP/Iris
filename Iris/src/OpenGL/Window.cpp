#include "Window.h"

#include <cassert>

using namespace GL;

bool Window::s_HasInitalizedGLFW = false;

Window::Window(const WindowProperties& windowProps)
    : m_Window(nullptr), m_Props(windowProps)
{
    init();
}

GL::Window::Window(const Window& other)
{
    m_Props = other.m_Props;
    init();
}

Window::~Window()
{
    glfwDestroyWindow(m_Window);
}

Window& GL::Window::operator=(const Window& other)
{
    if (this != &other)
    {
        glfwDestroyWindow(m_Window);
        m_Props = other.m_Props;
        init();
    }

    return *this;
}

void Window::setKeyCallback(void (*callback)(GLFWwindow*, int key, int scancode, int action, int mods))
{
    glfwSetKeyCallback(m_Window, callback);
}

void Window::setMousePosCallback(void (*callback)(GLFWwindow*, double xPos, double yPos))
{
    glfwSetCursorPosCallback(m_Window, callback);
}

void Window::setMouseMode(MouseMode mode)
{
    int cursorMode = 0;

    switch (mode)
    {
    case MouseMode::DISABLED:
        cursorMode = GLFW_CURSOR_DISABLED;
        break;

    case MouseMode::NORMAL:
        cursorMode = GLFW_CURSOR_NORMAL;
        break;

    case MouseMode::HIDDEN:
        cursorMode = GLFW_CURSOR_HIDDEN;
        break;
    }

    glfwSetInputMode(m_Window, GLFW_CURSOR, cursorMode);
}

void Window::update() const
{
    glfwPollEvents();
    glfwSwapBuffers(m_Window);
}

void Window::setContextCurrent() const
{
    glfwMakeContextCurrent(m_Window);
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(m_Window);
}

void Window::setShouldClose()
{
    glfwSetWindowShouldClose(m_Window, true);
}

unsigned Window::getWidth() const
{
    return m_Props.width;
}

unsigned Window::getHeight() const
{
    return m_Props.height;
}

GLFWwindow* Window::getWindow()
{
    return m_Window;
}

void Window::terminate()
{
    if (!s_HasInitalizedGLFW)
        return;

    glfwTerminate();
    s_HasInitalizedGLFW = false;
}

void Window::init()
{
    if (!s_HasInitalizedGLFW)
    {
        int status = glfwInit();
        assert(status);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

        s_HasInitalizedGLFW = true;
    }

    m_Window = glfwCreateWindow((int)m_Props.width, (int)m_Props.height, m_Props.title.c_str(), nullptr, nullptr);
    assert(m_Window);

    setContextCurrent();

    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    assert(status);

    glfwSetWindowUserPointer(m_Window, &m_Props);
    glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            WindowProperties& props = *(WindowProperties*)glfwGetWindowUserPointer(window);
            props.height = height;
            props.width = width;

            glViewport(0, 0, width, height);
        });
}
