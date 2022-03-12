#include "world_object.hpp"
#include <glm\gtc\matrix_transform.hpp>

namespace alc {

	world_object::world_object()
		: m_position(0.0f), m_rotation(), m_scale(1.0f)
		, m_checkedParent(false), m_parentWorldObject(nullptr)
		, m_transform(1.0f), m_transformIsDirty(true) { }

	world_object::~world_object() { }

	world_object* world_object::get_parent_world_object() const {
		if (!m_checkedParent) {
			m_parentWorldObject = dynamic_cast<world_object*>(get_parent());
			m_checkedParent = true;
		}
		return m_parentWorldObject;
	}

	glm::vec3 world_object::get_position() const {
		auto* parent = get_parent_world_object();
		if (parent) return parent->get_position() 
			+ parent->get_rotation() * m_position;
		return m_position;
	}

	glm::vec3 world_object::get_relative_position() const {
		return m_position;
	}

	void world_object::set_position(const glm::vec3& position) {
		auto* parent = get_parent_world_object();
		m_transformIsDirty = true;
		if (parent)
			m_position = position * parent->get_rotation() - parent->get_position();
		else
			m_position = position;
	}

	void world_object::set_relative_position(const glm::vec3& position) {
		m_transformIsDirty = true;
		m_position = position;
	}

	glm::quat world_object::get_rotation() const {
		auto* parent = get_parent_world_object();
		if (parent) return parent->get_rotation() * m_rotation;
		return m_rotation;
	}

	glm::quat world_object::get_relative_rotation() const {
		return m_rotation;
	}

	void world_object::set_rotation(const glm::quat& rotation) {
		auto* parent = get_parent_world_object();
		m_transformIsDirty = true;
		if (parent)
			// this should *reverse* the rotation to effectively subtract the rotation
			// **untested**
			m_rotation = parent->get_rotation() * rotation;
		else
			m_rotation = rotation;
	}

	void world_object::set_relative_rotation(const glm::quat& rotation) {
		m_transformIsDirty = true;
		m_rotation = rotation;
	}

	glm::vec3 world_object::get_scale() const {
		auto* parent = get_parent_world_object();
		if (parent) return parent->get_scale() * m_scale;
		return m_scale;
	}

	glm::vec3 world_object::get_relative_scale() const {
		return m_scale;
	}

	void world_object::set_scale(const glm::vec3& scale) {
		auto* parent = get_parent_world_object();
		m_transformIsDirty = true;
		if (parent)
			m_scale = scale / parent->get_scale();
		else
			m_scale = scale;
	}

	void world_object::set_relative_scale(const glm::vec3& scale) {
		m_transformIsDirty = true;
		m_scale = scale;
	}

	glm::mat4 world_object::get_transform() const {
		auto* parent = get_parent_world_object();
		if (m_transformIsDirty) {
			m_transformIsDirty = false;

			m_transform = glm::mat4(1.0f);
			m_transform = glm::scale(m_transform, m_scale);
			m_transform = m_transform * glm::mat4_cast(m_rotation);
			m_transform = glm::translate(m_transform, get_position());
		}
		// this should combine the parents transform with this one,
		// **untested**
		if (parent) return parent->get_transform() * m_transform;
		return m_transform;
	}

	///////////////////////////////////////////////////////////////////////////////////////////

	world_component::world_component()
		: m_checkedParent(false), m_parentWorldObject(nullptr) { }

	world_component::~world_component() { }

	world_object* world_component::get_world_object() const {
		if (!m_checkedParent) {
			m_parentWorldObject = dynamic_cast<world_object*>(get_object());
			m_checkedParent = true;
		}
		return m_parentWorldObject;
	}

}