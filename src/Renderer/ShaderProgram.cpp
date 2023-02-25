#include "ShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#include <iostream>

namespace RendererEngine {
	ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
	{
        GLuint vertexShaderID;
        if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID))
        {
            std::cerr << "VERTEX SHADER compile-time error" << std::endl;
            glDeleteShader(vertexShaderID);
            return;
        }

        GLuint fragmentShaderID;
        if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID))
        {
            std::cerr << "FRAGMENT SHADER compile-time error" << std::endl;
            glDeleteShader(fragmentShaderID);
            return;
        }

        // Linking vertex and fragment shaders
        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertexShaderID);
        glAttachShader(m_ID, fragmentShaderID);
        glLinkProgram(m_ID);

        GLint success;
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            GLchar infoLog[1024];
            glGetShaderInfoLog(m_ID, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER: Link-time error:\n" << infoLog << std::endl;
        }
        else
        {
            m_isCompiled = true;
        }

        // Deleting VS and FS
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);

	}

    bool ShaderProgram::createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID)
    {
        // Shader creaating
        // Shader identifier
        shaderID = glCreateShader(shaderType);
        const char* code = source.c_str();
        glShaderSource(shaderID, 1, &code, nullptr);

        // Compiling shader
        glCompileShader(shaderID);

        GLint success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            // 1 Kb for log data
            GLchar infoLog[1024];
            glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER: Compile-time error:\n" << infoLog << std::endl;
            return false;
        }
        return true;
    }

    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(m_ID);
    }

    void ShaderProgram::use() const
    {
        glUseProgram(m_ID);
    }

    ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept
    {
        glDeleteProgram(m_ID);
        m_ID = shaderProgram.m_ID;
        m_isCompiled = shaderProgram.m_isCompiled;

        shaderProgram.m_ID = 0;
        shaderProgram.m_isCompiled = false;

        return *this;
    }

    ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept
    {
        m_ID = shaderProgram.m_ID;
        m_isCompiled = shaderProgram.m_isCompiled;

        shaderProgram.m_ID = 0;
        shaderProgram.m_isCompiled = false;
    }

    void ShaderProgram::setInt(const std::string& name, const GLuint value)
    {
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
    }

    void ShaderProgram::setMatrix4(const std::string& name, const glm::mat4& matrix)
    {
        glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
    }

}