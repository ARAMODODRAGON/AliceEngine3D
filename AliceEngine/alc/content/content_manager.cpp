#include "content_manager.hpp"
#include "../core/alice_events.hpp"
#include "../core/debug.hpp"

namespace alc {

	//void content_manager::enable_removal_rate(uint32 rate) {
	//	if (s_removalRate == 0 && rate != 0)
	//		alice_events::onPostUpdate += content_manager::on_remove;
	//	else
	//		alice_events::onPostUpdate -= content_manager::on_remove;
	//	s_removalRate = rate;
	//}
	//
	//texture content_manager::load_texture(const std::string& file) {
	//	// check for exiting
	//	if (auto it = s_textures.find(file); it != s_textures.end())
	//		return it->second;
	//
	//	// load new
	//	texture tex = texture::load(file);
	//	s_textures.emplace(file, tex);
	//	return tex;
	//}
	//
	//shader content_manager::load_shader(const std::string& file) {
	//	// check for exiting
	//	if (auto it = s_shaders.find(file); it != s_shaders.end())
	//		return it->second;
	//
	//	// load new
	//	shader sha = shader::load(file);
	//	s_shaders.emplace(file, sha);
	//	return sha;
	//}
	//
	//shader content_manager::load_shader_source(const std::string& identifier, const std::string& source) {
	//	// check for exiting
	//	if (auto it = s_shaders.find(identifier); it != s_shaders.end())
	//		return it->second;
	//
	//	// load new
	//	shader sha = shader::loadsource(source);
	//	s_shaders.emplace(identifier, sha);
	//	return sha;
	//}
	//
	//mesh_ref content_manager::create_mesh(
	//	const std::string& identifier,
	//	const void* verticies, const size_t verticies_size,
	//	const void* indices, const size_t indices_size,
	//	const mesh_info& info
	//) {
	//	// already exists
	//	if (s_meshMap.find(identifier) != s_meshMap.end()) {
	//		ALC_DEBUG_ERROR("Could not load new mesh as the identifier \"" + identifier + "\" as it already exists");
	//		return nullptr;
	//	} 
	//
	//	// create and return
	//	mesh_ref meshPtr(new mesh(verticies, verticies_size, indices, indices_size, info));
	//	s_meshMap.emplace(identifier, meshPtr);
	//	return meshPtr;
	//}
	//
	//mesh_ref content_manager::get_mesh(const std::string& identifier) {
	//	if (auto it = s_meshMap.find(identifier); it != s_meshMap.end()) 
	//		return it->second;
	//	return nullptr;
	//}
	//
	//void content_manager::on_remove(timestep ts) {
	//	// check if we need to do anything this frame
	//	++s_framecounter;
	//	if ((s_framecounter % s_removalRate) != 0) return;
	//
	//	for (auto it = s_textures.begin(); it != s_textures.end(); it++) {
	//		if (it->second.get_shared_count() == 1) {
	//			texture::unload(it->second);
	//			it = s_textures.erase(it);
	//			// dunno why this is needed but it seems that the for loop doesnt like checking for this before iterating
	//			if (it == s_textures.end()) break;
	//		}
	//	}
	//
	//	for (auto it = s_shaders.begin(); it != s_shaders.end(); it++) {
	//		if (it->second.get_shared_count() == 1) {
	//			shader::unload(it->second);
	//			it = s_shaders.erase(it);
	//			// dunno why this is needed but it seems that the for loop doesnt like checking for this before iterating
	//			if (it == s_shaders.end()) break;
	//		}
	//	}
	//
	//	// delete unreferenced meshes
	//	for (auto it = s_meshMap.begin(); it != s_meshMap.end(); it++) {
	//		if (it->second.use_count() == 1) {
	//			it->second.reset();
	//			it = s_meshMap.erase(it);
	//		}
	//	}
	//	for (auto it = s_meshList.begin(); it != s_meshList.end(); it++) {
	//		if (it->use_count() == 1) {
	//			it->reset();
	//			it = s_meshList.erase(it);
	//		}
	//	}
	//
	//	// TODO: search for and remove content
	//}
	//
	//void content_manager::__init(uint32 rate) {
	//	enable_removal_rate(rate);
	//}
	//
	//void content_manager::__exit() {
	//	if (s_removalRate != 0) alice_events::onPostUpdate -= content_manager::on_remove;
	//
	//	for (auto [str, tex] : s_textures) {
	//		texture::unload(tex);
	//	}
	//	s_textures.clear();
	//
	//	for (auto [str, sha] : s_shaders) {
	//		shader::unload(sha);
	//	}
	//	s_shaders.clear();
	//
	//	// meshes
	//	for (auto [str, mesh] : s_meshMap) {
	//		mesh.force_delete();
	//	}
	//	s_meshMap.clear();
	//	for (auto mesh : s_meshList) {
	//		mesh.force_delete();
	//	}
	//	s_meshList.clear();
	//
	//	// TODO: remove all content
	//}
	//
	//void content_manager::__add(mesh_ref meshRef) { 
	//	s_meshList.push_back(meshRef);
	//}

	std::string content_manager::get_full_path(const std::string& contentPath) {
		return s_resourcePath + contentPath;
	}

	void content_manager::__init(uint32 rate, const std::string& resourcePath) {
		s_framecounter = 0;
		s_removalRate = rate;
		s_resourcePath = resourcePath;
	}

	void content_manager::__exit() {
		s_content.clear();
	}

	void content_manager::__step() {
		++s_framecounter;
		if ((s_framecounter % s_removalRate) != 0) return;

		for (auto& [type, con] : s_content) {
			con->remove_unreferenced();
		}
	}

}