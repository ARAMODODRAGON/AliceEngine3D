#include "texture.hpp"
#include "../core/debug.hpp"
#include <glew.h>
#define STBI_NO_GIF // we dont want any gif loading
#define STB_IMAGE_IMPLEMENTATION
#include "..\dependencies\stb_image.h"
#include "content_manager.hpp"

namespace alc {

	texture::texture(std::nullptr_t)
		: m_id(0), m_size(0u) { }

	texture::texture(const std::string& filepath, const textureopts& options) {
		std::string realFilepath = content_manager::get_full_path(filepath);

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
		stbi_uc* pixels = stbi_load(realFilepath.c_str(), &width, &height, &channels, tryformat);

		if (pixels == nullptr) {
			ALC_DEBUG_LOG("invalid filepath '" + filepath + "'");
			return;
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
		m_id = textureID;
		m_size = glm::uvec2(width, height);
		m_format = realformat;
		m_wrap = options.wrap;
		m_filter = options.filter;
	}

	texture::~texture() {
		if (is_valid())
			glDeleteTextures(1, &m_id);
	}

	bool texture::is_valid() const {
		return m_id != 0;
	}

	texture::operator bool() const { return is_valid(); }

	uint32 texture::get_id() const {
		return m_id;
	}

	glm::uvec2 texture::get_size() const {
		return m_size;
	}

	textureformat texture::get_format() const {
		return m_format;
	}

	texturewrap texture::get_wrap() const {
		return m_wrap;
	}

	texturefilter texture::get_filter() const {
		return m_filter;
	}

}