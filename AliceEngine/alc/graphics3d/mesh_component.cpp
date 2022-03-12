#include "mesh_component.hpp"
#include "scenegraph3d.hpp"

namespace alc {

	/// mesh m_mesh;
	/// material m_material;
	/// shader m_shader;
	/// 
	/// bool m_isStatic : 1;
	/// bool m_isEnabled : 1;
	/// glm::mat4 m_lockedTransform;

	mesh_component::mesh_component()
		: m_isStatic(false), m_isEnabled(false), m_lockedTransform(1.0f) { }

	mesh_component::~mesh_component() { }

	void mesh_component::on_create() {
		if (get_world_object()) {
			m_isEnabled = true;
			if (m_isEnabled) gfx3d::scenegraph3d::__add(this);
		}
	}

	bool mesh_component::get_enabled() const {
		return m_isEnabled;
	}

	void mesh_component::set_enabled(bool enabled) {
		// this first check is here to prevent enabling if there is no world_object
		if (get_world_object() && (m_isEnabled != enabled)) {
			m_isEnabled = enabled;
			if (m_isEnabled)
				gfx3d::scenegraph3d::__add(this);
			else
				gfx3d::scenegraph3d::__remove(this);
		}
	}

	bool mesh_component::get_static() const {
		return m_isStatic;
	}

	void mesh_component::set_static(bool stat) {
		if (m_isEnabled) gfx3d::scenegraph3d::__remove(this);
		m_isStatic = stat;
		if (m_isStatic && m_isEnabled) m_lockedTransform = get_world_object()->get_transform();
		if (m_isEnabled) gfx3d::scenegraph3d::__add(this);
	}

	mesh mesh_component::get_mesh() const {
		return m_mesh;
	}

	void mesh_component::set_mesh(const mesh& m) {
		if (m_isEnabled) gfx3d::scenegraph3d::__remove(this);
		m_mesh = m;
		if (m_isEnabled) gfx3d::scenegraph3d::__add(this);
	}

	material mesh_component::get_material() const {
		return m_material;
	}

	void mesh_component::set_material(const material& m) {
		if (m_isEnabled) gfx3d::scenegraph3d::__remove(this);
		m_material = m;
		if (m_isEnabled) gfx3d::scenegraph3d::__add(this);
	}

	shader mesh_component::get_shader() const {
		return m_shader;
	}

	void mesh_component::set_shader(const shader& s) {
		if (m_isEnabled) gfx3d::scenegraph3d::__remove(this);
		m_shader = s;
		if (m_isEnabled) gfx3d::scenegraph3d::__add(this);
	}

	glm::mat4 mesh_component::get_transform() const {
		if (m_isStatic) return m_lockedTransform;
		return get_world_object()->get_transform();
	}

}