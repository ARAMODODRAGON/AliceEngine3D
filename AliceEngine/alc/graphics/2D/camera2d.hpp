#ifndef ALC_GRAPHICS_2D_CAMERA2D_HPP
#define ALC_GRAPHICS_2D_CAMERA2D_HPP
#include "../../objects/world.hpp"
//#include "../../datatypes/bounds2d.hpp"

namespace alc {

	class camera2d : public object {
	public:

		camera2d();
		~camera2d();

		// returns the position
		glm::vec2 get_position() const;

		// sets the position
		void set_position(const glm::vec2& position);

		// returns the extents of the cameras view
		glm::vec2 get_extents() const;

		// set the cameras extents
		void set_extents(const glm::vec2& extents);

		// returns the transform matrix
		glm::mat4 get_transform() const;

	private:
		glm::vec2 m_position;
		glm::vec2 m_extents;
		mutable bool m_transformDirty;
		mutable glm::mat4 m_transform;
	};

}

#endif // !ALC_GRAPHICS_2D_CAMERA2D_HPP