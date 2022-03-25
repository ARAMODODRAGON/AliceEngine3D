#ifndef ALC_OBJECTS_ENTITY_HPP
#define ALC_OBJECTS_ENTITY_HPP
#include "world.hpp"

namespace alc {

	// type of object that can exist physically in the world,
	// with a position, rotation, scale, and transform
	// some components require the parent type to be an world_object
	class world_object : public object {
	public:

		world_object();
		~world_object();

		// returns the parent object if it's a world_object
		world_object* get_parent_world_object() const;

		// returns the position
		glm::vec3 get_position() const;
		
		// returns the relative position
		glm::vec3 get_relative_position() const;

		// sets the position
		void set_position(const glm::vec3& position);
		
		// sets the relative position
		void set_relative_position(const glm::vec3& position);

		// returns the rotation
		glm::quat get_rotation() const;
		
		// returns the relative rotation
		glm::quat get_relative_rotation() const;

		// sets the rotation
		void set_rotation(const glm::quat& rotation);
		
		// sets the relative rotation
		void set_relative_rotation(const glm::quat& rotation);

		// returns the scale
		glm::vec3 get_scale() const;
		
		// returns the relative scale
		glm::vec3 get_relative_scale() const;

		// sets the scale
		void set_scale(const glm::vec3& scale);
		
		// sets the relative scale
		void set_relative_scale(const glm::vec3& scale);

		// returns the transformation matrix
		glm::mat4 get_transform() const;

	private:

		glm::vec3 m_position;
		glm::quat m_rotation;
		glm::vec3 m_scale;

		mutable glm::mat4 m_transform;
		mutable bool m_transformIsDirty : 1;

		mutable bool m_checkedParent : 1;
		mutable world_object* m_parentWorldObject;

	};

}

#endif // !ALC_OBJECTS_ENTITY_HPP