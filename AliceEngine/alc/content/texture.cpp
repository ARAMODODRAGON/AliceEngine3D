#include "texture.hpp"
#include "../core/debug.hpp"
#include <glew.h>
#define STBI_NO_GIF // we dont want any gif loading
#define STB_IMAGE_IMPLEMENTATION
#include "..\dependencies\stb_image.h"

namespace alc {

	texture texture::load(const std::string& filepath) {
		// use default options
		textureopts options;
		return load(filepath, options);
	}

	texture texture::load(const std::string& filepath, const textureopts& options) {

		// set format type
		int tryformat = STBI_default;

		switch (options.format) {
			case alc::textureformat::r: tryformat = STBI_grey; break;
			case alc::textureformat::rg: tryformat = STBI_grey_alpha; break;
			case alc::textureformat::rgb: tryformat = STBI_rgb; break;
			case alc::textureformat::rgba: tryformat = STBI_rgb_alpha; break;
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
		textureformat realformat = options.format;
		if (realformat == textureformat::any) {
			switch (channels) {
				case STBI_grey: realformat = textureformat::r; break;
				case STBI_grey_alpha: realformat = textureformat::rg; break;
				case STBI_rgb: realformat = textureformat::rgb; break;
				case STBI_rgb_alpha: realformat = textureformat::rgba; break;
				default: break;
			}
		}

		// get mode
		int mode;
		switch (realformat) {
			case alc::textureformat::r: mode = GL_RED; break;
			case alc::textureformat::rg: mode = GL_RG; break;
			case alc::textureformat::rgb: mode = GL_RGB; break;
			case alc::textureformat::rgba: mode = GL_RGBA; break;
			default: break;
		}

		// create texture ID
		uint32 textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// load into opengl
		glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode, GL_UNSIGNED_BYTE, pixels);

		// wrapping and filtering options

		switch (options.wrap) {
			case texturewrap::clamp_edge:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				break;
			case texturewrap::clamp_border:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
				break;
			case texturewrap::repeat:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				break;
			case texturewrap::mirrored_repeat:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
				break;
			case texturewrap::mirror_clamp_edge:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRROR_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRROR_CLAMP_TO_EDGE);
				break;
			default: break;
		}

		switch (options.filter) {
			case texturefilter::linear:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			case texturefilter::nearest:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			default: break;
		}

		// unbind
		glBindTexture(GL_TEXTURE_2D, 0);

		// unload data 
		stbi_image_free(pixels);

		// create texture object and return
		texture tex;
		tex.m_data.reset(new data_t());
		tex.m_data->id = textureID;
		tex.m_data->size = glm::uvec2(width, height);
		tex.m_data->format = realformat;
		tex.m_data->wrap = options.wrap;
		tex.m_data->filter = options.filter;
		return tex;
	}

	bool texture::unload(texture& tex) {
		if (!tex) return false;

		// delete the texture
		tex.m_data.reset();
		return true;
	}

	texture::texture(std::nullptr_t)
		: m_data(nullptr) { }

	bool texture::is_valid() const {
		return m_data.get() != nullptr;
	}

	texture::operator bool() const { return is_valid(); }

	texture::operator uint32() const {
		if (!is_valid()) return -1;
		return m_data->id;
	}

	uint32 texture::get_id() const {
		if (!is_valid()) return -1;
		return m_data->id;
	}

	glm::uvec2 texture::get_size() const {
		if (!is_valid()) return glm::uvec2(0);
		return m_data->size;
	}

	textureformat texture::get_format() const {
		if (!is_valid()) return (textureformat)-1;
		return m_data->format;
	}

	texturewrap texture::get_wrap() const {
		if (!is_valid()) return (texturewrap)-1;
		return m_data->wrap;
	}

	texturefilter texture::get_filter() const {
		if (!is_valid()) return (texturefilter)-1;
		return m_data->filter;
	}

	bool texture::operator==(const texture& other) const {
		return m_data == other.m_data;
	}

	bool texture::operator!=(const texture& other) const {
		return m_data != other.m_data;
	}

	texture::data_t::~data_t() {
		glDeleteTextures(1, &id);
	}
}