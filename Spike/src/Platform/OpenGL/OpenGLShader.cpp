//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "OpenGLShader.h"
#include "Spike/Core/Vault.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Spike
{
    static GLenum ShaderTypeFromString(const String& type)
    {
        if (type == "vertex") return GL_VERTEX_SHADER;
        if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

        SPK_INTERNAL_ASSERT("Unknown shader type!");
        return 0;
    }

    OpenGLShader::OpenGLShader(const String& filepath)
        :m_Filepath(filepath), m_Name(Vault::GetNameWithExtension(filepath))
    {
        String source = Vault::ReadFile(m_Filepath);
        m_ShaderSource = PreProcess(source);
        Compile();
    }

    OpenGLShader::OpenGLShader(const String& source, const char* name)
        :m_Name(name), m_Filepath("[Spike] Shader is Built-In [Spike]")
    {
        m_ShaderSource = PreProcess(source);
        Compile();
    }

    OpenGLShader::~OpenGLShader()
    {
        Uint rendererID = reinterpret_cast<Uint>(m_RendererID);
        glDeleteProgram(rendererID);
    }

    std::unordered_map<GLenum, String> OpenGLShader::PreProcess(const String& source)
    {
        std::unordered_map<GLenum, String> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
        while (pos != String::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
            SPK_CORE_ASSERT(eol != String::npos, "Syntax error");
            size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
            String type = source.substr(begin, eol - begin);
            SPK_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

            size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
            SPK_CORE_ASSERT(nextLinePos != String::npos, "Syntax error");
            pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

            shaderSources[ShaderTypeFromString(type)] = (pos == String::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
        }

        return shaderSources;
    }

    void OpenGLShader::Compile()
    {
        GLuint program = glCreateProgram();
        SPK_CORE_ASSERT(m_ShaderSource.size() <= 2, "We only support two shaders for now.");
        std::array<GLenum, 2> glShaderIDs;
        int glShaderIDIndex = 0;
        for (auto& keyvalue : m_ShaderSource)
        {
            GLenum type = keyvalue.first;
            const String& source = keyvalue.second;

            GLuint shader = glCreateShader(type);

            const GLchar* sourceCstr = source.c_str();
            glShaderSource(shader, 1, &sourceCstr, 0);
            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
                glDeleteShader(shader);

                SPK_CORE_LOG_CRITICAL("%s", infoLog.data());
                SPK_CORE_LOG_CRITICAL("Shader compilation failure!");
                break;
            }
            glAttachShader(program, shader);
            glShaderIDs[glShaderIDIndex++] = shader;
        }

        glLinkProgram(program);
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(program);

            for (auto id : glShaderIDs)
                glDeleteShader(id);
            SPK_CORE_LOG_CRITICAL("%s", infoLog.data());
            SPK_CORE_LOG_CRITICAL("Shader link failure!");
        }
        for (auto id : glShaderIDs)
        {
            glDetachShader(program, id);
            glDeleteShader(id);
        }

        m_RendererID = (RendererID)program;
    }

    void OpenGLShader::Bind() const
    {
        glUseProgram((GLuint)m_RendererID);
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }

    void* OpenGLShader::GetNativeClass()
    {
        return (void*)this;
    }

    void OpenGLShader::SetInt(const String& name, int value) { UploadUniformInt(name, value); }
    void OpenGLShader::SetIntArray  (const String& name, int* value, Uint count) { UploadUniformIntArray(name, value, count); }
    void OpenGLShader::SetFloat     (const String& name, float value)                { UploadUniformFloat   (name, value);        }
    void OpenGLShader::SetFloat2    (const String& name, const glm::vec2& value)     { UploadUniformFloat2  (name, value);        }
    void OpenGLShader::SetFloat3    (const String& name, const glm::vec3& value)     { UploadUniformFloat3  (name, value);        }
    void OpenGLShader::SetFloat4    (const String& name, const glm::vec4& value)     { UploadUniformFloat4  (name, value);        }
    void OpenGLShader::SetMat4      (const String& name, const glm::mat4& value)     { UploadUniformMat4    (name, value);        }

    void OpenGLShader::UploadUniformInt(const String& name, int value)
    {
        GLint location = glGetUniformLocation((GLuint)m_RendererID, name.c_str());
        if (location != -1)
            glUniform1i(location, value);
    }

    void OpenGLShader::UploadUniformIntArray(const String& name, int* value, Uint count)
    {
        GLint location = glGetUniformLocation((GLuint)m_RendererID, name.c_str());
        if (location != -1)
            glUniform1iv(location, count, value);
    }

    void OpenGLShader::UploadUniformFloat(const String& name, float value)
    {
        GLint location = glGetUniformLocation((GLuint)m_RendererID, name.c_str());
        if (location != -1)
            glUniform1f(location, value);
    }

    void OpenGLShader::UploadUniformFloat2(const String& name, const glm::vec2& values)
    {
        GLint location = glGetUniformLocation((GLuint)m_RendererID, name.c_str());
        if (location != -1)
            glUniform2f(location, values.x, values.y);
    }

    void OpenGLShader::UploadUniformFloat3(const String& name, const glm::vec3& values)
    {
        GLint location = glGetUniformLocation((GLuint)m_RendererID, name.c_str());
        if (location != -1)
            glUniform3f(location, values.x, values.y, values.z);
    }

    void OpenGLShader::UploadUniformFloat4(const String& name, const glm::vec4& values)
    {
        GLint location = glGetUniformLocation((GLuint)m_RendererID, name.c_str());
        if (location != -1)
            glUniform4f(location, values.x, values.y, values.z, values.w);
    }

    void OpenGLShader::UploadUniformMat3(const String& name, const glm::mat3& matrix)
    {
        GLint location = glGetUniformLocation((GLuint)m_RendererID, name.c_str());
        if (location != -1)
            glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::UploadUniformMat4(const String& name, const glm::mat4& matrix)
    {
        GLint location = glGetUniformLocation((GLuint)m_RendererID, name.c_str());
        if (location != -1)
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}