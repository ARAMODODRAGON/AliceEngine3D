#ifndef ALC_GRAPHICS3D_CAMERA_HPP
#define ALC_GRAPHICS3D_CAMERA_HPP
#include "../common.hpp"
#include "../objects/world_object.hpp"

namespace alc {

	class camera3d : public world_object {
	public:

		camera3d();
		~camera3d();

		// sets if enabled
		void set_enabled(bool enable);

		// gets if enabled
		bool get_enabled();

		// sets the cameras field of view
		void set_fov(float fov);

		// gets the cameras field of view
		float get_fov() const;

		// sets the ratio of the camera
		// ratio is determined by the width/height
		void set_ratio(float ratio);

		// returns the cameras ratio
		float get_ratio() const;

		// sets the near and far planes
		// both always positive
		//void set_near_far(const glm::vec2& nearFar);

		// gets the near and far planes
		//glm::vec2 get_near_far() const;

		// gets the projection matrix
		glm::mat4 get_projection() const;

		// makes the camera look at a particular position 
		// if facing equals the cameras position then nothing will change
		void look_at(
			const glm::vec3& facing, // the point to look at
			const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f)
		);

		// gets the view matrix 
		glm::mat4 get_view() const;

	private:
		float m_fov;
		float m_ratio;
		bool m_isEnabled : 1;
		mutable bool m_projIsDirty : 1;
		mutable glm::mat4 m_projection;
	};

}

#endif // !ALC_GRAPHICS3D_CAMERA_HPP