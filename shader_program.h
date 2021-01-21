#pragma once

#include <optional>


class ShaderProgram
{
public:
    ShaderProgram();

//    ShaderProgram(const ShaderProgram&) = delete;
//    ShaderProgram& operator=(const ShaderProgram&) = delete;

//    ShaderProgram(ShaderProgram&& other) = delete;
//    ShaderProgram& operator=(ShaderProgram&&) = delete;

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

