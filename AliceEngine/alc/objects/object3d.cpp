#include "object3d.hpp"
#include <glm\gtc\matrix_transform.hpp>

namespace alc {

	object3d::object3d()
		: m_position(0.0f), m_rotation(), m_scale(1.0f)
		, m_checkedParent(false), m_parentWorldObject(nullptr)
		, m_transform(1.0f), m_transformIsDirty(true) { }

	object3d::~object3d() { }

	object3d* object3d::get_parent_object3d() const {
		if (!m_checkedParent) {
			m_parentWorldObject = get_ancestor<object3d>();
			m_checkedParent = true;
		}
		return m_parentWorldObject;
	}

	glm::vec3 object3d::get_position() const {
		auto* parent = get_parent_object3d();
		if (parent) return parent->get_position() 
			+ (parent->get_rotation() * m_position);
		return m_position;
	}

	glm::vec3 object3d::get_relative_position() const {
		return m_position;
	}

	void object3d::set_position(const glm::vec3& position) {
		auto* parent = get_parent_object3d();
		m_transformIsDirty = true;
		if (parent)
			m_position = position * parent->get_rotation() - parent->get_position();
		else
			m_position = position;
	}

	void object3d::set_relative_position(const glm::vec3& position) {
		m_transformIsDirty = true;
		m_position = position;
	}

	glm::quat object3d::get_rotation() const {
		auto* parent = get_parent_object3d();
		if (parent) return parent->get_rotation() * m_rotation;
		return m_rotation;
	}

	glm::quat object3d::get_relative_rotation() const {
		return m_rotation;
	}

	void object3d::set_rotation(const glm::quat& rotation) {
		auto* parent = get_parent_object3d();
		m_transformIsDirty = true;
		if (parent)
			// this should *reverse* the rotation to effectively subtract the rotation
			// **untested**
			m_rotation = parent->get_rotation() * rotation;
		else
			m_rotation = rotation;
	}

	void object3d::set_relative_rotation(const glm::quat& rotation) {
		m_transformIsDirty = true;
		m_rotation = rotation;
	}

	glm::vec3 object3d::get_scale() const {
		auto* parent = get_parent_object3d();
		if (parent) return parent->get_scale() * m_scale;
		return m_scale;
	}

	glm::vec3 object3d::get_relative_scale() const {
		return m_scale;
	}

	void object3d::set_scale(const glm::vec3& scale) {
		auto* parent = get_parent_object3d();
		m_transformIsDirty = true;
		if (parent)
			m_scale = scale / parent->get_scale();
		else
			m_scale = scale;
	}

	void object3d::set_relative_scale(const glm::vec3& scale) {
		m_transformIsDirty = true;
		m_scale = scale;
	}

	glm::mat4 object3d::get_transform() const {
		auto* parent = get_parent_object3d();
		if (m_transformIsDirty) {
			m_transformIsDirty = false;

			m_transform = glm::mat4(1.0f);
			m_transform = glm::translate(m_transform, m_position);
			m_transform = m_transform * glm::mat4_cast(m_rotation);
			m_transform = glm::scale(m_transform, m_scale);
		}
		if (get_parent_object3d()) return get_parent_object3d()->get_transform() * m_transform;
		return m_transform;
	}

}