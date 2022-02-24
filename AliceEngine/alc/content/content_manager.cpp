#include "content_manager.hpp"
#include "../core/alice_events.hpp"

namespace alc {

	void content_manager::enable_removal_rate(uint32 rate) {
		if (s_removalRate == 0 && rate != 0)
			alice_events::onPostUpdate += content_manager::on_remove;
		else
			alice_events::onPostUpdate -= content_manager::on_remove;
		s_removalRate = rate;
	}

	texture content_manager::load_texture(const std::string& file) {
		// check for exiting
		if (auto it = s_textures.find(file); it != s_textures.end())
			return it->second;

		// load new
		texture tex = texture::load(file);
		s_textures.emplace(file, tex);
		return tex;
	}

	shader content_manager::load_shader(const std::string& file) {
		// check for exiting
		if (auto it = s_shaders.find(file); it != s_shaders.end())
			return it->second;

		// load new
		shader sha = shader::load(file);
		s_shaders.emplace(file, sha);
		return sha;
	}

	shader content_manager::load_shader_source(const std::string& source) {
		// check for exiting
		if (auto it = s_shaders.find(source); it != s_shaders.end())
			return it->second;

		// load new
		shader sha = shader::loadsource(source);
		s_shaders.emplace(source, sha);
		return sha;
	}

	model content_manager::load_model(const std::string& file) {
		// check for exiting
		if (auto it = s_models.find(file); it != s_models.end())
			return it->second;

		// load new
		model mo = model::load(file);
		s_models.emplace(file, mo);
		return mo;
	}

	void content_manager::on_remove(timestep ts) {
		// check if we need to do anything this frame
		++s_framecounter;
		if ((s_framecounter % s_removalRate) != 0) return;

		for (auto it = s_textures.begin(); it != s_textures.end(); it++) {
			if (it->second.get_shared_count() == 1) {
				texture::unload(it->second);
				it = s_textures.erase(it);
				// dunno why this is needed but it seems that the for loop doesnt like checking for this before iterating
				if (it == s_textures.end()) break; 
			}
		}
		
		for (auto it = s_shaders.begin(); it != s_shaders.end(); it++) {
			if (it->second.get_shared_count() == 1) {
				shader::unload(it->second);
				it = s_shaders.erase(it);
				// dunno why this is needed but it seems that the for loop doesnt like checking for this before iterating
				if (it == s_shaders.end()) break; 
			}
		}

		for (auto it = s_models.begin(); it != s_models.end(); it++) {
			if (it->second.get_shared_count() == 1) {
				model::unload(it->second);
				it = s_models.erase(it);
				// dunno why this is needed but it seems that the for loop doesnt like checking for this before iterating
				if (it == s_models.end()) break; 
			}
		}

		// TODO: search for and remove content
	}

	void content_manager::__init(uint32 rate) {
		enable_removal_rate(rate);
	}

	void content_manager::__exit() {
		if (s_removalRate != 0) alice_events::onPostUpdate -= content_manager::on_remove;

		for (auto [str, tex] : s_textures) {
			texture::unload(tex);
		}
		s_textures.clear();

		for (auto [str, sha] : s_shaders) {
			shader::unload(sha);
		}
		s_shaders.clear();

		for (auto [str, mo] : s_models) {
			model::unload(mo);
		}
		s_models.clear();

		// TODO: remove all content
	}

}