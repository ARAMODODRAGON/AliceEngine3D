#include "camera3d.hpp"
#include "scenegraph3d.hpp"

namespace alc {

	camera3d::camera3d()
		: m_fov(60.0f), m_ratio(1920.0f / 1080.0f), m_nearFar(0.0f, 10000.0f)
		, m_isEnabled(true), m_projIsDirty(true), m_projection(1.0f) {
		gfx3d::scenegraph3d::__add(this);
	}

	camera3d::~camera3d() {
		if (m_isEnabled) gfx3d::scenegraph3d::__remove(this);
	}

	void camera3d::set_enabled(bool enable) {
		if (m_isEnabled != enable) {
			if (enable) gfx3d::scenegraph3d::__add(this);
			else gfx3d::scenegraph3d::__remove(this);
		}
		m_isEnabled = enable;
	}

	bool camera3d::get_enabled() {
		return m_isEnabled;
	}

	void camera3d::set_fov(float fov) {
		m_projIsDirty = true;
		m_fov = fov;
	}

	float camera3d::get_fov() const {
		return m_fov;
	}

	void camera3d::set_ratio(float ratio) {
		m_projIsDirty = true;
		m_ratio = ratio;
	}

	float camera3d::get_ratio() const {
		return m_ratio;
	}

	void camera3d::set_near_far(const glm::vec2& nearFar) {
		m_projIsDirty = true;
		m_nearFar = nearFar;
	}

	glm::vec2 camera3d::get_near_far() const {
		return m_nearFar;
	}

	glm::mat4 camera3d::get_projection() const {
		if (m_projIsDirty) {
			m_projIsDirty = false;
			m_projection = glm::perspective(glm::radians(m_fov), m_ratio, m_nearFar[0], m_nearFar[1]);
		}
		return m_projection;
	}

	void camera3d::look_at(const glm::vec3& facing, const glm::vec3& up) { 
		if (glm::distance2(facing, get_position()) < 0.01f) return;
		glm::mat4 view = glm::lookAt(get_position(), facing, up);
		set_rotation(glm::conjugate(glm::toQuat(view)));
	}

	glm::mat4 camera3d::get_view() const {
		const auto pos = get_position();
		const auto dir = get_rotation() * glm::vec3(0.0f, 0.0f, -1.0f);
		return glm::lookAt(pos, dir + pos, glm::vec3(0.0f, 1.0f, 0.0f));
	}

}