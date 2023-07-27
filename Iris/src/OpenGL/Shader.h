#ifndef GL_SHADER_H

#define GL_SHADER_H

#include <string>
#include <unordered_map>

namespace GL
{
	class Shader
	{
	public:
		Shader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc);
		Shader(const Shader&) = delete;
		~Shader();

		Shader& operator=(const Shader&) = delete;

		void bind() const;
		void unbind() const;

		unsigned getProgram();
		int getUniformLocation(const std::string& uniform) const;

		void setUniform1i(const std::string& uniform, int value) const;
		void setUniform1f(const std::string& uniform, float value) const;
		void setUniform3f(const std::string& uniform, float x, float y, float z) const;

	private:
		unsigned m_Program;
		mutable std::unordered_map<std::string, int> m_UniformLocationCache;

		void compileShader(unsigned shader);
		void loadShaderSource(unsigned shader, const std::string& src);
		void linkProgram(unsigned vertexShader, unsigned fragmentShader);
	};
}

#endif // !GL_SHADER_H
