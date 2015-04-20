#pragma once

#include "core.hpp"

namespace Shader
{
	class Texture
	{
	public:
		Texture(const char* path);

		static GLuint readBMP(const char* path);
		static GLuint readDXT(const char* path);

		LFS_INLINE void bind();

	private:
		GLuint _textureID;
	};

	void Texture::bind()
	{
		glBindTexture(GL_TEXTURE_2D, _textureID);
	}
}
