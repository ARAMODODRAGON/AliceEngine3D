#include "entity.hpp"
#include <glm\gtc\matrix_transform.hpp>

namespace alc {

	entity::entity(const std::string& name)
		: object(name)
		, m_position(0.0f), m_rotation(), m_scale(1.0f)
		, m_transform(1.0f), m_transformIsDirty(true) { }

	entity::~entity() { }

	glm::vec3 entity::get_position() const {
		return m_position;
	}

	void entity::set_position(const glm::vec3& position) {
		m_position = position;
		m_transformIsDirty = true;
	}

	glm::quat entity::get_rotation() const {
		return m_rotation;
	}

	void entity::set_rotation(const glm::quat& rotation) {
		m_rotation = rotation;
		m_transformIsDirty = true;
	}

	glm::vec3 entity::get_scale() const {
		return m_scale;
	}

	void entity::set_scale(const glm::vec3& scale) {
		m_scale = scale;
		m_transformIsDirty = true;
	}

	glm::mat4 entity::get_transform() const {
		if (m_transformIsDirty) {
			m_transformIsDirty = false;

			m_transform = glm::mat4(1.0f);
			m_transform = glm::scale(m_transform, m_scale);
			m_transform = m_transform * glm::mat4_cast(m_rotation);
			m_transform = glm::translate(m_transform, get_position());
		}
		return m_transform;
	}

}