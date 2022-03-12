#ifndef ALC_CONTENT_TEXTURE_HPP
#define ALC_CONTENT_TEXTURE_HPP
#include "../common.hpp"

namespace alc {

	enum class textureformat : uint8 {
		r, rg, rgb, rgba,

		// used to load the default format of a texture 
		any = static_cast<uint8>(-1)
	};

	enum class texturewrap : uint8 {
		clamp_edge,
		clamp_border,
		repeat,
		mirrored_repeat,
		mirror_clamp_edge
	};

	enum class texturefilter : uint8 {
		linear, nearest
	};

	struct textureopts final {
		textureformat format = textureformat::any;
		texturewrap wrap = texturewrap::clamp_edge;
		texturefilter filter = texturefilter::linear;
	};

	class texture final {
	public:

		// null texture object
		texture(std::nullptr_t = nullptr);

		// loads a texture 
		texture(const std::string& filepath, const textureopts& options = textureopts());

		// deletes a texture
		~texture();

		// checks if this is a valid texture
		bool is_valid() const;

		// checks if this is a valid texture
		operator bool() const;

		// returns the texture ID
		uint32 get_id() const;

		// returns the texture size
		glm::uvec2 get_size() const;

		// returns the format of the texture
		textureformat get_format() const;

		// returns the wrap of the texture
		texturewrap get_wrap() const;

		// returns the filter of the texture
		texturefilter get_filter() const;

	private:
		uint32 m_id;
		glm::uvec2 m_size;
		textureformat m_format;
		texturewrap m_wrap;
		texturefilter m_filter;
	};

}

#endif // !ALC_CONTENT_TEXTURE_HPP