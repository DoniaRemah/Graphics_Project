#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our
{

    class ShaderProgram
    {

    private:
        // Shader Program Handle (OpenGL object name)
        GLuint program;

    public:
        ShaderProgram()
        {
            // TODO: (Req 1) Create A shader program
            program = glCreateProgram(); // create the program that will contain vertex & frag shaders & link them
        }
        ~ShaderProgram()
        {
            // TODO: (Req 1) Delete a shader program
            if (program != 0) // check that there is a program to destroy
            {
                glDeleteProgram(program); // destroy the program
            }
        }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use()
        {
            glUseProgram(program);
        }

        GLuint getUniformLocation(const std::string &name)
        {
            // TODO: (Req 1) Return the location of the uniform with the given name
            return glGetUniformLocation(program, name.c_str());
            // return unsigned integer (pointer) represent the location of the uniform
        }

        void set(const std::string &uniform, GLfloat value)
        {
            // TODO: (Req 1) Send the given float value to the given uniform
            glUniform1f(getUniformLocation(uniform), value);
            // set the value of the given uniform name with the given float value
            // use function getUniformLocation to get the pointer to this uniform
        }

        void set(const std::string &uniform, GLuint value)
        {
            // TODO: (Req 1) Send the given unsigned integer value to the given uniform
            glUniform1ui(getUniformLocation(uniform), value);
            // set the value of the given uniform name with the given unsigned integer value
            // use function getUniformLocation to get the pointer to this uniform
        }

        void set(const std::string &uniform, GLint value)
        {
            // TODO: (Req 1) Send the given integer value to the given uniform
            glUniform1i(getUniformLocation(uniform), value);
            // set the value of the given uniform name with the given integer value
            // use function getUniformLocation to get the pointer to this uniform
        }

        void set(const std::string &uniform, glm::vec2 value)
        {
            // TODO: (Req 1) Send the given 2D vector value to the given uniform
            glUniform2f(getUniformLocation(uniform), value.x, value.y);
            // set the value of the given uniform name with the given vector values
            // use 2f as vec2 contains 2 floats x , y
            // use function getUniformLocation to get the pointer to this uniform
        }

        void set(const std::string &uniform, glm::vec3 value)
        {
            // TODO: (Req 1) Send the given 3D vector value to the given uniform
            glUniform3f(getUniformLocation(uniform), value.x, value.y, value.z);
            // set the value of the given uniform name with the given vector values
            // use 3f as vec3 contains 3 floats x , y , z
            // use function getUniformLocation to get the pointer to this uniform
        }

        void set(const std::string &uniform, glm::vec4 value)
        {
            // TODO: (Req 1) Send the given 4D vector value to the given uniform
            glUniform4f(getUniformLocation(uniform), value.x, value.y, value.z, value.w);
            // set the value of the given uniform name with the given vector values
            // use 4f as vec3 contains 4 floats x , y , z , w
            // use function getUniformLocation to get the pointer to this uniform
        }

        void set(const std::string &uniform, glm::mat4 matrix)
        {
            // TODO: (Req 1) Send the given matrix 4x4 value to the given uniform
        }

        // TODO: (Req 1) Delete the copy constructor and assignment operator.
        // Question: Why do we delete the copy constructor and assignment operator?

        ShaderProgram(ShaderProgram const &) = delete;
        ShaderProgram &operator=(ShaderProgram const &) = delete;
    };

}

#endif