#include "Program.hpp"

#include "core.hpp"
#include "log.hpp"

#if defined(LFS_COMPILER_GCC)
    #include <fcntl.h>
    #include <sys/mman.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
#elif defined(LFS_COMPILER_MSVC)
    #include <Windows.h>
#endif

Program::Program()
{
    _program = glCreateProgram();
}

bool Program::attach(GLenum type, std::string path)
{
    GLuint shader = compile(type, path.c_str());
    if(shader != 0) {
        /* attach the shader to the program */
        glAttachShader(_program, shader);

        /* delete the shader - it won't actually be
         * destroyed until the program that it's attached
         * to has been destroyed */
        glDeleteShader(shader);
    }

    return shader != 0;
}

GLuint Program::compile(GLenum type, std::string path)
{
#if defined(LFS_COMPILER_GCC)
    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1)
    {
        return 0;
    }

    // obtain file size
    struct stat sb;
    if (fstat(fd, &sb) == -1)
    {
        return 0;
    }

    int length = sb.st_size;

    const char* addr = static_cast<const char*>(mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0u));
    if (addr == MAP_FAILED)
    {
        return 0;
    }
#elif defined(LFS_COMPILER_MSVC)
    HANDLE hfile = CreateFile(path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if(hfile == INVALID_HANDLE_VALUE)
    {
        return 0;
    }

    int length = 0;
    DWORD res = GetFileSize(hfile, (LPDWORD)&length);
    if (res == INVALID_FILE_SIZE)
    {
        CloseHandle(hfile);
        return 0;
    }

    HANDLE map_handle = CreateFileMapping(hfile, NULL, PAGE_READWRITE | SEC_RESERVE, 0, 0, 0);
    if(map_handle == NULL)
    {
        CloseHandle(hfile);
        return 0;
    }

    const char* addr = static_cast<const char*>(MapViewOfFile( map_handle, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0 ));
    if(addr == NULL )
    {
        CloseHandle(map_handle);
    	CloseHandle(hfile);
        return 0;
    }
#endif


    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const char **)&addr, &length);
    glCompileShader(shader);

#if defined(LFS_COMPILER_GCC)
    munmap((void*)addr, length);
    close(fd);
#elif defined(LFS_COMPILER_MSVC)
    UnmapViewOfFile(addr);
    CloseHandle(map_handle);
    CloseHandle(hfile);
#endif

    /* Make sure the compilation was successful */
    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE) {
        /* get the shader info log */
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char* log = new char[length];
        glGetShaderInfoLog(shader, length, &result, log);

        /* print an error message and the info log */
        LOGD("shaderCompileFromFile(): Unable to compile %s: %s", path.c_str(), log);
        delete [] log;

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

bool Program::link()
{
    GLint result;

    /* link the program and make sure that there were no errors */
    glLinkProgram(_program);
    glGetProgramiv(_program, GL_LINK_STATUS, &result);
    if(result == GL_FALSE) {
        GLint length;

        /* get the program info log */
        glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &length);
        char* log = new char[length];
        glGetProgramInfoLog(_program, length, &result, log);

        /* print an error message and the info log */
        LOGD("sceneInit(): Program linking failed: %s", log);
        delete [] log;

        /* delete the program */
        glDeleteProgram(_program);
        _program = 0;
    }

    return _program != 0;
}
