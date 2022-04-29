#include "mesh_renderer.hpp"
#include "scenegraph3d.hpp"

namespace alc {

	mesh_renderer::mesh_renderer()
		: m_mesh(nullptr), m_isStatic(false), m_isEnabled(true), m_lockedTransform(1.0f) { }

	mesh_renderer::~mesh_renderer() { }

	void mesh_renderer::on_create() {
		gfx3d::scenegraph3d::__add(this);
	}

	bool mesh_renderer::get_enabled() const {
		return m_isEnabled;
	}

	void mesh_renderer::set_enabled(bool enabled) {
		// this first check is here to prevent enabling if there is no object3d
		if (m_isEnabled != enabled) {
			m_isEnabled = enabled;
			if (m_isEnabled)
				gfx3d::scenegraph3d::__add(this);
			else
				gfx3d::scenegraph3d::__remove(this);
		}
	}

	bool mesh_renderer::get_static() const {
		return m_isStatic;
	}

	void mesh_renderer::set_static(bool stat) {
		if (m_isEnabled) gfx3d::scenegraph3d::__remove(this);
		m_isStatic = stat;
		if (m_isStatic && m_isEnabled) m_lockedTransform = object3d::get_transform();
		if (m_isEnabled) gfx3d::scenegraph3d::__add(this);
	}

	mesh_ref mesh_renderer::get_mesh() const {
		return m_mesh;
	}

	void mesh_renderer::set_mesh(mesh_ref m) {
		if (m_isEnabled) gfx3d::scenegraph3d::__remove(this);
		m_mesh = m;
		if (m_isEnabled) gfx3d::scenegraph3d::__add(this);
	}

	material mesh_renderer::get_material() const {
		return m_material;
	}

	void mesh_renderer::set_material(const material& m) {
		if (m_isEnabled) gfx3d::scenegraph3d::__remove(this);
		m_material = m;
		if (m_isEnabled) gfx3d::scenegraph3d::__add(this);
	}

	shader_ref mesh_renderer::get_shader() const {
		return m_shader;
	}

	void mesh_renderer::set_shader(shader_ref s) {
		if (m_isEnabled) gfx3d::scenegraph3d::__remove(this);
		m_shader = s;
		if (m_isEnabled) gfx3d::scenegraph3d::__add(this);
	}

	glm::mat4 mesh_renderer::get_transform() const {
		if (m_isStatic) return m_lockedTransform;
		return object3d::get_transform();
	}

}