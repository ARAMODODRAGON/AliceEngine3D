#include "camera2d.hpp"
#include <glm\gtc\matrix_transform.hpp>

namespace alc {

	camera2d::camera2d()
		: m_position(0.0f)
		, m_extents(0.0f)
		, m_transformDirty(true)
		, m_transform(0.0f) { }

	camera2d::~camera2d() { }

	glm::vec2 camera2d::get_position() const {
		return m_position;
	}

	void camera2d::set_position(const glm::vec2& position) {
		m_position = position;
		m_transformDirty = true;
	}

	glm::vec2 camera2d::get_extents() const {
		return m_extents;
	}

	void camera2d::set_extents(const glm::vec2& extents) {
		m_extents = extents;
		m_transformDirty = true;
	}

	glm::mat4 camera2d::get_transform() const {
		if (m_transformDirty) {
			m_transformDirty = false;

			// calculate transform
			m_transform = glm::ortho(
				m_position.x - m_extents.x,
				m_position.x + m_extents.x,
				m_position.y - m_extents.y,
				m_position.y + m_extents.y
			);

		}
		return m_transform;
	}

}