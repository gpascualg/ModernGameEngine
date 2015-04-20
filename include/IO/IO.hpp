#pragma once 

#include "core.hpp"

namespace IO
{
	struct FileMapping
	{
#if defined(LFS_COMPILER_GCC)
		int fd;
#elif defined(LFS_COMPILER_MSVC)
		HANDLE hfile;
		HANDLE map_handle;
#endif
		const char* addr;
		int length;
	};

	FileMapping* mapFile(const char* filepath);
	void unmapFile(FileMapping* fp);
}
