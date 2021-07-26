#ifndef ALC_CONTENT_CONTENTMANAGER_HPP
#define ALC_CONTENT_CONTENTMANAGER_HPP
#include "../common.hpp"
#include "texture.hpp"
#include <unordered_map>

namespace alc {

	//class shader;
	//class font;
	//class mesh;
	//class sound;

	class contentcontext final {
		ALC_NO_COPY(contentcontext);
	public:

		contentcontext();

		contentcontext(contentcontext&& other);

		contentcontext& operator=(contentcontext&& other);

		~contentcontext();

		// clears out and deletes the content stored in the internal storage
		void clear();

		// loads a texture and stores it in the internal storage
		texture loadtexture(const std::string& str);

		// adds a texture to the internal storage and manages it
		bool storetexture(const texture& tex, const std::string& str);

		// loads a shader file and stores it in the internal storage
		//shader loadshader(const std::string& str);

		// loads a shader source and stores it in the internal storage
		//shader loadshadersource(const std::string& identifier, const std::string& source);

		// loads a shader source and stores it in the internal storage using the source as the identifier
		//shader loadshadersource(const std::string& source);

		// loads a font file and stores it in the internal storage
		//font loadfont(const std::string& str);

	private:
		friend class contentmanager;
		// TODO: create all types of content
		std::unordered_map<std::string, texture> m_textures;
		//std::unordered_map<std::string, shader> m_shaders;
		//std::unordered_map<std::string, font> m_fonts;
		//std::unordered_map<std::string, mesh> m_meshes;
		//std::unordered_map<std::string, sound> m_sounds;
	};

	class contentmanager final {
		ALC_STATIC_CLASS(contentmanager);
	public:

		// default context in the contentmanager
		static contentcontext* def();

		// the current context being used in the content manager
		static contentcontext* cur();

		// sets the current context being used in the content manager
		static void set(contentcontext* context);

		// clears out and deletes the content stored in the current context
		static void clear();

		// loads a texture and stores it in the current context
		static texture loadtexture(const std::string& str);

		// adds a texture to the internal storage and manages it
		static bool storetexture(const texture& tex, const std::string& str);

		// loads a shader file and stores it in the current context
		//static shader loadshader(const std::string& str);

		// loads a shader source and stores it in the current context
		//static shader loadshadersource(const std::string& identifier, const std::string& source);

		// loads a shader source and stores it in the current context using the source as the identifier
		//static shader loadshadersource(const std::string& source);

		// loads a font file and stores it in the current context
		//static font loadfont(const std::string& str);

	private:
		static inline contentcontext* s_default;
		static inline contentcontext* s_current;
	public:
		static void __init();
		static void __exit();
	};

}

#endif // !ALC_CONTENT_CONTENTMANAGER_HPP