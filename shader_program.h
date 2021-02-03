#pragma once

#include <optional>
#include <glm/mat4x4.hpp>


class ShaderProgram
{
public:
    ShaderProgram();

//    ShaderProgram(const ShaderProgram&) = delete;
//    ShaderProgram& operator=(const ShaderProgram&) = delete;

//    ShaderProgram(ShaderProgram&& other) = delete;
//    ShaderProgram& operator=(ShaderProgram&&) = delete;

    // to set uniform vars:
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;

    std::optional<unsigned int> getId()
    {
        return shaderProgram;
    }

    bool createAndLink(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);

    bool createVertexShader(const char* sourceCode);
    bool createFragmentShader(const char* sourceCode);
    bool link();
    void use();

    ~ShaderProgram();
    void deleteShaders();

private:
    bool isLinked = false;
    std::optional<unsigned int> shaderProgram{};
    std::optional<unsigned int> vertexShader{};
    std::optional<unsigned int> fragmentShader{};
};

