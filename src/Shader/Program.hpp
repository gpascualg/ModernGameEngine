#pragma once

#include "core.hpp"

namespace Shader
{

    class Program
    {
    public:
        Program();

        inline GLuint operator*()
        {
            return this->_program;
        }

        bool attach(GLenum type, std::string path);
        bool bindAttribute(const char* location, GLenum type);
        bool link();
        bool bind();

    private:
        GLuint compile(GLenum type, std::string path);

    private:
        GLuint _program;
        std::map<std::string, GLuint> _attribs;
    };

}
