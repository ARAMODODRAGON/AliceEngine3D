#ifndef ALC_PHYSICS3D_BODY3D_HPP
#define ALC_PHYSICS3D_BODY3D_HPP
#include "../objects/object3d.hpp"

namespace alc {

	class collider3d;

	// base class for all body types
	class body3d : public object3d {
	public:

		body3d();
		virtual ~body3d() = 0;

		// the number of colliders associated with this body
		size_t colliders_size() const;

		// access a collider
		collider3d* collider_get(size_t index);

		// access a collider
		const collider3d* collider_get(size_t index) const;

	private:
		std::vector<collider3d*> m_colliders;
	public:
		void __add_collider(collider3d* coll);
		void __remove_collider(const collider3d* coll);
	};

	// rigidbody represents any objects that has physics applied to it
	// has linear and rotational velocity, and mass
	class rigidbody3d : public body3d {
	public:

		rigidbody3d();
		~rigidbody3d();

		// adds a set amount of force to the rigidbody
		void apply_force(float force);

		// resets any forces to 0
		void reset_force();

		// returns the total force applied to this body this step
		float get_force() const;


		// gets the velocity
		glm::vec3 get_velocity() const;

		// sets the velocity
		void set_velocity(const glm::vec3& velocity);

		// gets the rotational velocity
		glm::quat get_rotational_velocity() const;

		// sets the rotational velocity
		void set_rotational_velocity(const glm::quat& rotationalVelocity);

		// gets the mass
		float get_mass() const;

		// gets the inverse mass
		float get_inverse_mass() const;

		// sets the mass
		// must be greater than 0
		void set_mass(float mass);

	private:
		bool m_isEnabled : 1;
		glm::vec3 m_velocity;
		glm::quat m_rotationalVelocity;
		float m_mass;
		float m_inverseMass;
		float m_force;
	};

	// any object that has a shape (colliders) but no physics applied to it
	class staticbody3d : public body3d {
	public:

		staticbody3d();
		~staticbody3d();

	private:
	};

	// an object with controlled movement (i.e. characters, vehicles)
	//class kinematicbody3d : public body3d {
	//public:
	//
	//	kinematicbody3d();
	//	~kinematicbody3d();
	//
	//private:
	//};

}

#endif // !ALC_PHYSICS3D_BODY3D_HPP