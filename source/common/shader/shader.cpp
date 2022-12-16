#include "shader.hpp"

#include <cassert>
#include <iostream>
#include <fstream>
#include <string>

// Forward definition for error checking functions
std::string checkForShaderCompilationErrors(GLuint shader);
std::string checkForLinkingErrors(GLuint program);

bool our::ShaderProgram::attach(const std::string &filename, GLenum type) const
{
    // Here, we open the file and read a string from it containing the GLSL code of our shader
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "ERROR: Couldn't open shader file: " << filename << std::endl;
        return false;
    }
    std::string sourceString = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const char *sourceCStr = sourceString.c_str();
    file.close();

    // TODO: Complete this function
    // Note: The function "checkForShaderCompilationErrors" checks if there is
    //  an error in the given shader. You should use it to check if there is a
    //  compilation error and print it so that you can know what is wrong with
    //  the shader. The returned string will be empty if there is no errors.

    GLuint shader; // unsigned integer used to create shader

    shader = glCreateShader(type);
    // create the shader program of the type is sent from ShaderTestState class and return unsigned int saved in shader variable
    // type = GL_VERTEX_SHADER ... or ... type = GL_FRAGMENT_SHADER

    glShaderSource(shader, 1, &sourceCStr, nullptr); // send the source code read from file as c_str to create the shader

    glCompileShader(shader); // compile the shader

    std::string compileError = checkForShaderCompilationErrors(shader); // call the function to check if there is compilation error

    if (compileError != "")
    {
        // there is an error as it returns empty string if no error
        std::cout << "ERROR::SHADER COMPILATION_FAILED :: \n"
                  << compileError << "\n"
                  << std::endl;
        return false;
    }

    glAttachShader(program, shader); // attach the created shader with the program

    // We return true if the compilation succeeded
    return true;
}

bool our::ShaderProgram::link() const
{
    // TODO: Complete this function
    // Note: The function "checkForLinkingErrors" checks if there is
    //  an error in the given program. You should use it to check if there is a
    //  linking error and print it so that you can know what is wrong with the
    //  program. The returned string will be empty if there is no errors.

    glLinkProgram(program); // link 2 shaders attached to the program

    std::string linkError = checkForLinkingErrors(program); // call the function to check if there is compilation error

    if (linkError != "")
    {
        // there is an error as it returns empty string if no error
        std::cout << "ERROR::SHADER LINKING_FAILED :: \n"
                  << linkError << "\n"
                  << std::endl;
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////
// Function to check for compilation and linking error in shaders //
////////////////////////////////////////////////////////////////////

std::string checkForShaderCompilationErrors(GLuint shader)
{
    // Check and return any error in the compilation process
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetShaderInfoLog(shader, length, nullptr, logStr);
        std::string errorLog(logStr);
        delete[] logStr;
        return errorLog;
    }
    return std::string();
}

std::string checkForLinkingErrors(GLuint program)
{
    // Check and return any error in the linking process
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status)
    {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetProgramInfoLog(program, length, nullptr, logStr);
        std::string error(logStr);
        delete[] logStr;
        return error;
    }
    return std::string();
}