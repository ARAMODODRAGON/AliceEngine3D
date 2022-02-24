#ifndef ALC_OBJECTS_ENTITY_HPP
#define ALC_OBJECTS_ENTITY_HPP
#include "world.hpp"

namespace alc {

	// type of object that can exist physically in the world,
	// with a position, rotation, scale, and transform
	// some components require the parent type to be an entity
	class entity : public object {
	public:

		entity(const std::string& name = "");
		~entity();


		// returns the position
		glm::vec3 get_position() const;
		
		// sets the position
		void set_position(const glm::vec3& position);
		
		// returns the rotation
		glm::quat get_rotation() const;
		
		// sets the rotation
		void set_rotation(const glm::quat& rotation);
		
		// returns the scale
		glm::vec3 get_scale() const;
		
		// sets the scale
		void set_scale(const glm::vec3& scale);
		
		// returns the transformation matrix
		glm::mat4 get_transform() const;

	private:

		glm::vec3 m_position;
		glm::quat m_rotation;
		glm::vec3 m_scale;

		mutable glm::mat4 m_transform;
		mutable bool m_transformIsDirty : 1;

	};

}

#endif // !ALC_OBJECTS_ENTITY_HPP