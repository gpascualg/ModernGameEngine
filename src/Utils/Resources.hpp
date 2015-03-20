#pragma once

#include "platform_detect.hpp"
#include "log.hpp"

#include <iostream>
#include <sstream>
#include <string>

#if defined(LFS_COMPILER_MSVC)
    #define SEP std::string("\\")
#else
    #define SEP std::string("/")
#endif

class Resources
{
public:
    static void setup(const char*& path, const char* folder)
    {
        basePath = path;

        size_t pos = basePath.find_last_of(SEP);
        if (pos == std::string::npos)
        {
            basePath = "";
        }
        else
        {
            basePath.erase(pos);
        }


        basePath += SEP + std::string(folder) + SEP;

        LOGD("Path set to %s", basePath.c_str());
    }

    template< typename ... Args >
    static std::string getPath(Args const& ... parts)
    {
        std::ostringstream stream;
        stream << basePath;
        using List= int[];
        (void)List{0, ( (void)(stream << parts << SEP), 0 ) ... };

        auto str = stream.str();
        str.erase(str.end() - 1);
        return str;
    }

private:
    static std::string basePath;
};
