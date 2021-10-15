#ifndef ALC_GRAPHICS_CAMERA_HPP
#define ALC_GRAPHICS_CAMERA_HPP
#include "../common.hpp"
#include "../datatypes/layermask.hpp"

namespace alc {

	struct camera final {

		camera();

		// sets the view matrix to orthographic
		void create_orthographic(const glm::vec2& min, const glm::vec2& max);
		
		// sets the view matrix to orthographic with near and far planes
		void create_orthographic(const glm::vec2& min, const glm::vec2& max, const float near, const float far);

		// sets the view matrix to perspective
		void create_perspective();

		// returns the transformation matrix
		glm::mat4 get_transform() const;

		// returns the view matrix
		glm::mat4 get_view() const;

		// sets the position
		void set_position(const glm::vec3& position);

		// gets the position
		glm::vec3 get_position();
		
		// sets the position
		void set_rotation(const glm::quat& rotation);

		// gets the position
		glm::quat get_rotation();
		
		// sets the position in eular angles
		void set_eular_rotation(const glm::vec3& rotation);

		// gets the position
		glm::vec3 get_eular_rotation();

	private:
		
		glm::vec3 m_position;
		glm::quat m_rotation;

		void calculate_transform();

		glm::mat4 m_transform;
		glm::mat4 m_view;

	};

}

#endif // !ALC_GRAPHICS_CAMERA_HPP