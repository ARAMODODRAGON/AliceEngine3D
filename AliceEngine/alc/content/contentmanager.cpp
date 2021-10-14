#include "contentmanager.hpp"

namespace alc {

	contentcontext::contentcontext() { }

	contentcontext::contentcontext(contentcontext&& other) {
		// move content
		m_textures = std::move(other.m_textures);

		// TODO: add move lines

		// change the current to this if needed
		if (contentmanager::cur() == &other) contentmanager::set(this);
	}

	contentcontext& contentcontext::operator=(contentcontext&& other) {
		// move content
		m_textures = std::move(other.m_textures);

		// TODO: add move lines

		// change the current to this if needed
		if (contentmanager::cur() == &other) contentmanager::set(this);

		// return this
		return *this;
	}

	contentcontext::~contentcontext() {
		if (contentmanager::cur() == this) {
			contentmanager::set(nullptr);
			clear();
		}
	}

	void contentcontext::clear() {

		// clear textures
		for (auto [str, tex] : m_textures)
			alc::texture::unload(tex);
		m_textures.clear();

		/* TODO: delete content */
	}

	texture contentcontext::loadtexture(const std::string& str) {
		// return existing
		auto it = m_textures.find(str);
		if (it != m_textures.end()) return it->second;

		// create and emplace
		texture tex = texture::load(str);
		m_textures.emplace(str, tex);

		// return
		return tex;
	}

	bool contentcontext::storetexture(const texture& tex, const std::string& str) {
		// check if exists
		auto it = m_textures.find(str);
		if (it != m_textures.end()) return false;

		// emplace and return
		m_textures.emplace(str, tex);
		return true;
	}

	shader contentcontext::loadshader(const std::string& str) {
		// return existing
		auto it = m_shaders.find(str);
		if (it != m_shaders.end()) return it->second;

		// create and emplace
		shader shad = shader::load(str);
		m_shaders.emplace(str, shad);

		// return
		return shad;
	}

	contentcontext* contentmanager::def() {
		return s_default;
	}

	contentcontext* contentmanager::cur() {
		return s_current;
	}

	void contentmanager::set(contentcontext* context) {
		s_current = context;
	}

	void contentmanager::clear() {
		if (s_current) s_current->clear();
		else s_default->clear();
	}

	texture contentmanager::loadtexture(const std::string& str) {
		if (s_current) return s_current->loadtexture(str);
		else return s_default->loadtexture(str);
	}

	bool contentmanager::storetexture(const texture& tex, const std::string& str) {
		if (s_current) return s_current->storetexture(tex, str);
		else return s_default->storetexture(tex, str);
	}

	shader contentmanager::loadshader(const std::string& str) {
		if (s_current) return s_current->loadshader(str);
		else return s_default->loadshader(str);
	}

	void contentmanager::__init() {
		s_default = new contentcontext();
	}

	void contentmanager::__exit() {
		if (s_default) delete s_default;
	}

}