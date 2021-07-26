#ifndef ALC_CONTENT_TEXTURE_HPP
#define ALC_CONTENT_TEXTURE_HPP
#include "../common.hpp"

namespace alc {

	class texture {
	public:

		enum class format : uint8 {
			r, rg, rgb, rgba,

			// used to load the default format of a texture 
			any = static_cast<uint8>(-1)
		};

		// loads a texture 
		static texture load(const std::string& filepath);

		// loads a texture 
		static texture load(const std::string& filepath, format forceformat);

		// deletes a texture
		static void unload(texture& tex);


		// null texture object
		texture(std::nullptr_t = nullptr);

		// checks if this is a valid texture
		bool is_valid() const;

		// checks if this is a valid texture
		operator bool() const;

		// returns the texture ID
		operator uint32() const;

		// returns the texture ID
		uint32 get_id() const;

		// returns the texture size
		glm::uvec2 get_size() const;

		// compare the textures
		bool operator==(const texture& other) const;

		// compare the textures
		bool operator!=(const texture& other) const;

	private:
		uint32 m_id;
		glm::uvec2 m_size;
		format m_format;
	};

}

#endif // !ALC_CONTENT_TEXTURE_HPP