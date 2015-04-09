#pragma once

#include "core.hpp"

namespace Shader
{

    class Program
    {
    public:
        Program();

        LFS_INLINE GLuint operator*();

        GLuint attach(GLenum type, std::string path);
        bool bindAttribute(const char* attrib, GLuint index);
        LFS_INLINE GLuint attributeLocation(const char* attrib);
        bool link();
        LFS_INLINE bool bind();

    private:
        GLuint compile(GLenum type, std::string path);

    private:
        GLuint _program;
        std::map<std::string, GLuint> _attribs;
    };


    // INLINE METHODS

    GLuint Program::operator*()
    {
        return this->_program;
    }

    GLuint Program::attributeLocation(const char* attrib)
    {
        return _attribs[attrib];
    }

    bool Program::bind()
    {
        glUseProgram(_program);

        return glGetError() == 0;
    }
}
