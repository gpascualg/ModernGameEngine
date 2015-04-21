#include "IO.hpp"

#if defined(LFS_COMPILER_GCC)
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#elif defined(LFS_COMPILER_MSVC)
#include <Windows.h>
#endif

namespace IO
{
	FileMapping* mapFile(const char* filepath)
	{
		FileMapping* fileMapping = new FileMapping;

#if defined(LFS_COMPILER_GCC)
		fileMapping->fd = open(filepath, O_RDONLY);
		if (fileMapping->fd == -1)
		{
			return 0;
		}

		// obtain file size
		struct stat sb;
		if (fstat(fileMapping->fd, &sb) == -1)
		{
			return 0;
		}

		fileMapping->length = sb.st_size;

		fileMapping->addr = static_cast<const char*>(mmap(NULL, fileMapping->length, PROT_READ, MAP_PRIVATE, fileMapping->fd, 0u));
		if (fileMapping->addr == MAP_FAILED)
		{
			return 0;
		}
#elif defined(LFS_COMPILER_MSVC)
		fileMapping->hfile = CreateFile(filepath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		if (fileMapping->hfile == INVALID_HANDLE_VALUE)
		{
			return 0;
		}

		fileMapping->length = GetFileSize(fileMapping->hfile, NULL);
		if (fileMapping->length == INVALID_FILE_SIZE)
		{
			CloseHandle(fileMapping->hfile);
			return 0;
		}

		fileMapping->map_handle = CreateFileMapping(fileMapping->hfile, NULL, PAGE_READWRITE | SEC_RESERVE, 0, 0, 0);
		if (fileMapping->map_handle == NULL)
		{
			CloseHandle(fileMapping->hfile);
			return 0;
		}

		fileMapping->addr = static_cast<const char*>(MapViewOfFile(fileMapping->map_handle, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0));
		if (fileMapping->addr == NULL)
		{
			CloseHandle(fileMapping->map_handle);
			CloseHandle(fileMapping->hfile);
			return 0;
		}
#endif

		return fileMapping;
	}

	void unmapFile(FileMapping* fp)
	{
#if defined(LFS_COMPILER_GCC)
		munmap((void*)fp->addr, fp->length);
		close(fp->fd);
#elif defined(LFS_COMPILER_MSVC)
		UnmapViewOfFile(fp->addr);
		CloseHandle(fp->map_handle);
		CloseHandle(fp->hfile);
#endif
	}
}
