#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/mat4x4.hpp>

namespace RendererEngine {
	class ShaderProgram {
	public:
		ShaderProgram() = delete;
		ShaderProgram(ShaderProgram& shaderProgram) = delete;

		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		
		// Move constructor
		ShaderProgram(ShaderProgram&& shaderProgram) noexcept;


		~ShaderProgram();
		
		// return flag of shader compile????
		bool isCompiled() const { return m_isCompiled; }

		// Shader enamle function
		void use() const;

		//
		void setInt(const std::string& nmae, const GLuint value);

		// set transformation matrix in shader
		void setMatrix4(const std::string& name, const glm::mat4& matrix);

		// Copy assignment is forbidden
		ShaderProgram& operator=(const ShaderProgram& shaderProgram) = delete;

		// Move assignment
		ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
		

	private:
		// Create Shader
		bool createShader(const std::string& source, const GLenum shaderTpe, GLuint& shaderID);

		// Did shader compile successfully? flag
		bool m_isCompiled = false;
		
		// ID of shader program
		GLuint m_ID = 0;

	};
}