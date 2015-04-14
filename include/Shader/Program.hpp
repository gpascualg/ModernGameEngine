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
		LFS_INLINE GLuint uniformLocation(const char* unif);
        bool link();
        LFS_INLINE bool bind();

    private:
        GLuint compile(GLenum type, std::string path);

    private:
        GLuint _program;
		std::map<std::string, GLuint> _attribs;
		std::map<std::string, GLuint> _uniforms;
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

	GLuint Program::uniformLocation(const char* unif)
	{
		auto ret = _uniforms.find(unif);
		if (ret != _uniforms.end())
		{
			return ret->second;
		}

		GLuint unifLocation = glGetUniformLocation(this->_program, unif);
		_uniforms.insert(std::make_pair(unif, unifLocation));
		return unifLocation;
	}

    bool Program::bind()
    {
        glUseProgram(_program);

        return glGetError() == 0;
    }
}
