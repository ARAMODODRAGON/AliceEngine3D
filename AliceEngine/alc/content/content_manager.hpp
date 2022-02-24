#ifndef ALC_CONTENT_CONTENT_MANAGER_HPP
#define ALC_CONTENT_CONTENT_MANAGER_HPP
#include "../common.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "model.hpp"
#include "../datatypes/timestep.hpp"

namespace alc {

	class content_manager final {
		ALC_STATIC_CLASS(content_manager);
	public:

		// enables the check to remove unreferenced content at the set rate
		static void enable_removal_rate(uint32 rate);


		// loads a texture into memory
		// will return existing textures if any match
		static texture load_texture(const std::string& file);

		// loads a shader into memory
		// will return existing shaders if any match
		static shader load_shader(const std::string& file);

		// loads a shader source into memory
		// will return existing shaders if any match
		static shader load_shader_source(const std::string& source);

		// loads an obj file as a model
		static model load_model(const std::string& file);

	private:
		static void on_remove(timestep ts);
		static inline int32 s_removalRate = 0;
		static inline int32 s_framecounter = 0;
		//static inline std::unordered_map<std::string, mesh> s_meshes;
		static inline std::unordered_map<std::string, texture> s_textures;
		static inline std::unordered_map<std::string, shader> s_shaders;
		static inline std::unordered_map<std::string, model> s_models;
	public:
		static void __init(uint32 rate);
		static void __exit();
	};

}

#endif // !ALC_CONTENT_CONTENT_MANAGER_HPP