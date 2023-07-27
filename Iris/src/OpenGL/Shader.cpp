#include "Shader.h"

#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

using namespace GL;

Shader::Shader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc)
{
    const unsigned vertexShader = glCreateShader(GL_VERTEX_SHADER);
    loadShaderSource(vertexShader, vertexShaderSrc);
    compileShader(vertexShader);

    const unsigned fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    loadShaderSource(fragmentShader, fragmentShaderSrc);
    compileShader(fragmentShader);

    linkProgram(vertexShader, fragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(m_Program);
}

void Shader::bind() const
{
    glUseProgram(m_Program);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

unsigned Shader::getProgram()
{
    return m_Program;
}

int Shader::getUniformLocation(const std::string& uniform) const
{
    if (m_UniformLocationCache.find(uniform) == m_UniformLocationCache.end())
        m_UniformLocationCache[uniform] = glGetUniformLocation(m_Program, uniform.c_str());

    return m_UniformLocationCache[uniform];
}

void Shader::setUniform1i(const std::string& uniform, int value) const
{
    const int location = getUniformLocation(uniform);
    glUniform1i(location, value);
}

void Shader::setUniform1f(const std::string& uniform, float value) const
{
    const int location = getUniformLocation(uniform);
    glUniform1f(location, value);
}

void Shader::setUniform3f(const std::string& uniform, float x, float y, float z) const
{
    const int location = getUniformLocation(uniform);
    glUniform3f(location, x, y, z);
}

void Shader::compileShader(unsigned shader)
{
    glCompileShader(shader);

    int compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (!compileStatus)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "OPEN_GL_ERROR::SHADER::COMPILE_ERROR: " << infoLog << std::endl;
        assert(false);
    }
}

void Shader::loadShaderSource(unsigned shader, const std::string& src)
{
    std::ifstream inputStream(src);

    if (!inputStream || !inputStream.is_open())
        assert(false);

    inputStream.exceptions(std::ios::failbit | std::ios::badbit);

    try 
    {
        std::stringstream sstream;
        sstream << inputStream.rdbuf();
        const std::string source = sstream.str();
        const char* sourcePtr = source.c_str();

        glShaderSource(shader, 1, &sourcePtr, nullptr);
    }
    catch (...)
    {
        std::cout << "OPEN_GL_ERROR::SHADER::READ_ERROR: Error reading shader source" << std::endl;
        assert(false);
    }

    inputStream.close();
}

void Shader::linkProgram(unsigned vertexShader, unsigned fragmentShader)
{
    m_Program = glCreateProgram();
    glUseProgram(m_Program);
    glAttachShader(m_Program, vertexShader);
    glAttachShader(m_Program, fragmentShader);
    glLinkProgram(m_Program);

    int linkStatus;
    glGetProgramiv(m_Program, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus)
    {
        char infoLog[512];
        glGetProgramInfoLog(m_Program, 512, nullptr, infoLog);
        std::cout << "OPEN_GL_ERROR::PROGRAM::LINK_ERROR: " << infoLog << std::endl;
        assert(false);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
