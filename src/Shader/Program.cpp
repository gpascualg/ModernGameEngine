#include "Program.hpp"

#include "core.hpp"
#include "log.hpp"
#include "IO.hpp"


namespace Shader
{

    Program::Program()
    {
        _program = glCreateProgram();
    }

    GLuint Program::attach(GLenum type, std::string path)
    {
        GLuint shader = compile(type, path.c_str());
        if (shader != 0) {
            /* attach the shader to the program */
            glAttachShader(_program, shader);

            /* delete the shader - it won't actually be
             * destroyed until the program that it's attached
             * to has been destroyed */
            glDeleteShader(shader);
        }

        return shader;
    }

    GLuint Program::compile(GLenum type, std::string path)
    {
		IO::FileMapping* fileMapping = IO::mapFile(path.c_str());

        GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, (const char **)&fileMapping->addr, &fileMapping->length);
        glCompileShader(shader);
		
		IO::unmapFile(fileMapping);

        /* Make sure the compilation was successful */
        GLint result;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            /* get the shader info log */
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &fileMapping->length);
			char* log = new char[fileMapping->length];
			glGetShaderInfoLog(shader, fileMapping->length, &result, log);

            /* print an error message and the info log */
            LOGD("Unable to compile %s: %s", path.c_str(), log);
            delete[] log;

            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }

    bool Program::bindAttribute(const char* attrib, GLuint index)
    {
        _attribs.insert(std::make_pair(attrib, index));
        glBindAttribLocation(_program, index, attrib);
        return true;
    }

    bool Program::link()
    {
        GLint result;

        /* link the program and make sure that there were no errors */
        glLinkProgram(_program);
        glGetProgramiv(_program, GL_LINK_STATUS, &result);
        if (result == GL_FALSE) {
            GLint length;

            /* get the program info log */
            glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &length);
            char* log = new char[length];
            glGetProgramInfoLog(_program, length, &result, log);

            /* print an error message and the info log */
            LOGD("Program linking failed: %s", log);
            delete[] log;

            /* delete the program */
            glDeleteProgram(_program);
            _program = 0;
        }

        return _program != 0;
    }
}
