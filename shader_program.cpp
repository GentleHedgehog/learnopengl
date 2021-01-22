#include "shader_program.h"
#include <glad/glad.h>
#include <iostream>
#include <cassert>

namespace {

auto checkShaderCompilation = [](unsigned int shaderId) -> bool {
    int success{};
    char infoLog[512];
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        std::cout << "compilation error for shader: id = " << shaderId << ", " << infoLog << std::endl;
    }
    else
    {
        std::cout << "shader compilation success, id = " << shaderId << std::endl;
    }
    return success == GL_TRUE;
};

}

ShaderProgram::ShaderProgram()
{
    shaderProgram = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    deleteShaders();
    if (shaderProgram.has_value())
    {
        glDeleteProgram(shaderProgram.value());
        shaderProgram.reset();
    }
}

void ShaderProgram::deleteShaders()
{
    if (vertexShader.has_value())
    {
        glDeleteShader(vertexShader.value());
        vertexShader.reset();
    }
    if (fragmentShader.has_value())
    {
        glDeleteShader(fragmentShader.value());
        fragmentShader.reset();
    }
}

bool ShaderProgram::createVertexShader(const char *sourceCode)
{
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* data = sourceCode;
    glShaderSource(vertexShader.value(), 1/*count of strings*/,
                   &data, NULL);
    glCompileShader(vertexShader.value());
    return checkShaderCompilation(vertexShader.value());
}

bool ShaderProgram::createFragmentShader(const char *sourceCode)
{
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* data = sourceCode;
    glShaderSource(fragmentShader.value(), 1, &data, NULL);
    glCompileShader(fragmentShader.value());
    return checkShaderCompilation(fragmentShader.value());
}

bool ShaderProgram::link()
{
    assert (vertexShader.has_value());
    assert (fragmentShader.has_value());

    glAttachShader(shaderProgram.value(), vertexShader.value());
    glAttachShader(shaderProgram.value(), fragmentShader.value());
    glLinkProgram(shaderProgram.value());

    {
        int success{};
        char infoLog[512];
        glGetProgramiv(shaderProgram.value(), GL_LINK_STATUS, &success);
        if (success == GL_FALSE)
        {
            glGetProgramInfoLog(shaderProgram.value(), 512, NULL, infoLog);
            std::cout << "link error for program: id = " << shaderProgram.value() << ", " << infoLog << std::endl;
            return false;
        }
        else
        {
            std::cout << "program link success" << std::endl;
        }
    }
    isLinked = true;
    return true;
}

bool ShaderProgram::createAndLink(const std::string &vertexShaderSource, const std::string &fragmentShaderSource)
{
    if (! createVertexShader(vertexShaderSource.data()))
        return false;

    if (! createFragmentShader(fragmentShaderSource.data()))
        return false;

    return link();
}

void ShaderProgram::setBool(const std::string &name, bool value) const
{
    assert(isLinked);
    glUniform1i(glGetUniformLocation(shaderProgram.value(), name.c_str()), static_cast<int>(value));
}

void ShaderProgram::setInt(const std::string &name, int value) const
{
    assert(isLinked);
    glUniform1i(glGetUniformLocation(shaderProgram.value(), name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string &name, float value) const
{
    assert(isLinked);
    glUniform1f(glGetUniformLocation(shaderProgram.value(), name.c_str()), value);
}

void ShaderProgram::use()
{
    assert(isLinked);
    glUseProgram(shaderProgram.value());
}


