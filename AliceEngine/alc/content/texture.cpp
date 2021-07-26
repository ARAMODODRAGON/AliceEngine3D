#include "texture.hpp"
#include "../core/debug.hpp"
#include <glew.h>
#define STBI_NO_GIF // we dont want any gif loading
#define STB_IMAGE_IMPLEMENTATION
#include "..\dependencies\stb_image.h"

namespace alc {

	texture texture::load(const std::string& filepath) {
		return load(filepath, format::any);
	}

	texture texture::load(const std::string& filepath, format forceformat) {

		// set format type
		int tryformat = STBI_default;

		switch (forceformat) {
			case alc::texture::format::r: tryformat = STBI_grey; break;
			case alc::texture::format::rg: tryformat = STBI_grey_alpha; break;
			case alc::texture::format::rgb: tryformat = STBI_rgb; break;
			case alc::texture::format::rgba: tryformat = STBI_rgb_alpha; break;
			default: break;
		}

		// load the texture data
		int width, height, channels;
		stbi_uc* pixels = stbi_load(filepath.c_str(), &width, &height, &channels, tryformat);

		if (pixels == nullptr) {
			ALC_DEBUG_LOG("invalid filepath '" + filepath + "'");
			return texture();
		}

		// get format type
		format realformat = forceformat;
		if (forceformat == format::any) {
			switch (channels) {
				case STBI_grey: realformat = format::r; break;
				case STBI_grey_alpha: realformat = format::rg; break;
				case STBI_rgb: realformat = format::rgb; break;
				case STBI_rgb_alpha: realformat = format::rgba; break;
				default: break;
			}
		}

		// get mode
		int mode;
		switch (realformat) {
			case alc::texture::format::r: mode = GL_RED; break;
			case alc::texture::format::rg: mode = GL_RG; break;
			case alc::texture::format::rgb: mode = GL_RGB; break;
			case alc::texture::format::rgba: mode = GL_RGBA; break;
			default: break;
		}

		// create texture ID
		uint32 textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// load into opengl
		glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode, GL_UNSIGNED_BYTE, pixels);

		// wrapping and filtering options
		// TODO: rely on arguments to change these
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// unbind
		glBindTexture(GL_TEXTURE_2D, 0);

		// unload data 
		stbi_image_free(pixels);

		// create texture object and return
		texture tex;
		tex.m_id = textureID;
		tex.m_format = realformat;
		tex.m_size = glm::uvec2(width, height);
		return tex;
	}

	void texture::unload(texture& tex) {
		// delete the texture
		glDeleteTextures(1, &tex.m_id);
	}

	texture::texture(std::nullptr_t) : m_id(-1), m_size(0), m_format(format::any) { }

	bool texture::is_valid() const {
		return m_format != format::any;
	}

	texture::operator bool() const { return is_valid(); }

	texture::operator uint32() const { return m_id; }

	uint32 texture::get_id() const {
		return m_id;
	}

	glm::uvec2 texture::get_size() const {
		return m_size;
	}

	bool texture::operator==(const texture& other) const {
		return m_id == other.m_id;
	}

	bool texture::operator!=(const texture& other) const {
		return m_id != other.m_id;
	}

}