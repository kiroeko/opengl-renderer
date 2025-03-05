#ifndef __SHADER_H__
#define __SHADER_H__

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "common/FileUtils.h"

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>

namespace OGLRenderer::Graphics
{
    class Shader
    {
    public:
        Shader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
        {
            GLuint vs = 0;
            if (!CreateAndComplieShader(vertexShaderFilePath, GL_VERTEX_SHADER, vs))
                return;

            GLuint fs = 0;
            if (!CreateAndComplieShader(vertexShaderFilePath, GL_FRAGMENT_SHADER, fs))
                return;

            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vs);
            glAttachShader(shaderProgram, fs);
            glLinkProgram(shaderProgram);
            int success = 0;
            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
            if (!success)
            {
                constexpr int errlogLen = 4096;
                char errLog[errlogLen];
                glGetProgramInfoLog(shaderProgram, 4096, NULL, errLog);
                std::cerr <<
                    "ERROR: shader program (vs: \"" <<vertexShaderFilePath <<
                    "\",fs: \"" << fragmentShaderFilePath << "\") linking failed!\n" <<
                    errLog << std::endl;
            }

            glDeleteShader(vs);
            glDeleteShader(fs);
        }

        ~Shader()
        {
            glDeleteProgram(shaderProgram);
        }

        void Use()
        {
            glUseProgram(shaderProgram);
        }

        bool SetUniformWithFloat(const std::string& uniformName, const std::vector<float>& floats)
        {
            GLint loc = GetUniformLocation(uniformName);
            if (loc == -1)
                return false;

            const auto& size = floats.size();
            if (size == 1)
            {
                glUniform1f(loc, floats[0]);
                return true;
            }
            if (size == 2)
            {
                glUniform2f(loc, floats[0], floats[1]);
                return true;
            }
            if (size == 3)
            {
                glUniform3f(loc, floats[0], floats[1], floats[2]);
                return true;
            }
            if (size == 4)
            {
                glUniform4f(loc, floats[0], floats[1], floats[2], floats[3]);
                return true;
            }
            return false;
        }

        template<typename... Args>
        std::enable_if_t<(std::is_same_v<Args, float> && ...), bool>
            SetUniformWithFloat(const std::string& uniformName, Args... args)
        {
            GLint loc = GetUniformLocation(uniformName);
            if (loc == -1)
                return false;

            constexpr std::size_t numArgs = sizeof...(Args);
            static_assert(numArgs >= 1 && numArgs <= 4, "Error: Unsupported number of arguments in SetUniformWithFloat.");

            if constexpr (numArgs == 1)
            {
                glUniform1f(loc, args...);
            }
            else if constexpr (numArgs == 2)
            {
                glUniform2f(loc, args...);
            }
            else if constexpr (numArgs == 3)
            {
                glUniform3f(loc, args...);
            }
            else if constexpr (numArgs == 4)
            {
                glUniform4f(loc, args...);
            }
            return true;
        }
    private:
        bool CreateAndComplieShader(
            const std::string& shaderFilePath,
            GLenum shaderType,
            GLuint& outShader)
        {
            outShader = 0;

            const auto& str =
                OGLRenderer::Common::FileUtils::GetInstance().ReadFile(shaderFilePath);
            if (str.empty())
                return false;
            const char* strC = str.c_str();

            outShader = glCreateShader(shaderType);
            glShaderSource(outShader, 1, &strC, NULL);
            glCompileShader(outShader);

            int success = 0;
            glGetShaderiv(outShader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                constexpr int errlogLen = 4096;
                char errLog[errlogLen];
                glGetShaderInfoLog(outShader, errlogLen, NULL, errLog);
                std::cerr << "ERROR: shader file \"" << shaderFilePath << "\" compilation failed!\n" << errLog << std::endl;
                return false;
            };
            return true;
        }

        GLint GetUniformLocation(const std::string& uniformName)
        {
            const auto& it = uniformLocations.find(uniformName);
            if (it != uniformLocations.end())
                return it->second;

            GLint loc = glGetUniformLocation(shaderProgram, uniformName.c_str());
            if (loc != -1)
            {
                uniformLocations[uniformName] = loc;
            }
            return loc;
        }

        GLuint shaderProgram = 0;
        std::unordered_map<std::string, GLint> uniformLocations;
    };
}

#endif