#ifndef ALC_PHYSICS3D_BODY3D_HPP
#define ALC_PHYSICS3D_BODY3D_HPP
#include "../objects/object3d.hpp"

namespace alc {

	class shape3d;

	// body represents any object that has physics applied to it
	class body3d : public object3d {
		using shape_list = std::list<shape3d*>;
	public:
		using shape_iterator = shape_list::iterator;
		using shape_const_iterator = shape_list::const_iterator;


		body3d();
		virtual ~body3d() = 0;

		// gets the total mass
		// if there is no associated shapes then the mass is 1
		float get_mass() const;

		// gets the total inverse mass
		// if there is no associated shapes then the inverse mass is 1
		float get_inverse_mass() const;

		// gets the center of mass
		glm::vec3 get_center_of_mass() const;


		// the number of shapes associated with this body
		size_t shapes_size() const;

		// begin iterating through associated shapes
		shape_iterator begin();

		// begin iterating through associated shapes
		shape_const_iterator begin() const;

		// end iterating through associated shapes
		shape_iterator end();

		// end iterating through associated shapes
		shape_const_iterator end() const;

	private:
		shape_list m_collisionShapes;
		bool m_massIsDirty : 1;
		float m_mass;
		float m_inverseMass;
	public:
		void __add_shape(shape3d* coll);
		void __remove_shape(const shape3d* coll);
		void __set_mass_is_dirty();
	};

	// base class for any 3d shapes that are used either in static objects or bodies
	// has its own mass and friction values
	class shape3d : public object3d {
	public:

		shape3d();
		virtual ~shape3d() = 0;

		// returns the first body that is this shape's ancestor
		body3d* get_body() const;

		// gets the mass of this shape
		float get_mass() const;

		// gets the inverse mass of this shape
		float get_inverse_mass() const;

		// sets the mass of this shape
		// must be greater than 0
		void set_mass(float mass);


		// overridden events
		void on_create() override;
		void on_destroy() override;

	private:
		body3d* m_body;
		float m_mass;
		float m_inverseMass;
		float m_friction;
	};

	// rigidbody represents any objects that has 6 axis of motion
	// has linear and rotational velocity
	class rigidbody3d : public body3d {
	public:

		rigidbody3d();
		virtual ~rigidbody3d();

		// gets if this body is enabled
		bool get_enabled() const;

		// sets if this body is enabled
		void set_enabled(bool enabled);


		// checks if this body is asleep
		bool is_sleeping() const;

		// forces the body to sleep for at least one frame
		void sleep();

		// forces the body to wake up
		void wake_up();


		// adds a set amount of force to the rigidbody
		void apply_force(glm::vec3 force);

		// resets any forces to 0
		void reset_force();

		// returns the total force applied to this body this step
		float get_force() const;
		

		// adds a set amount of torque to the rigidbody
		void apply_torque(glm::quat torque);

		// resets any torque to 0
		void reset_torque();

		// returns the total torque applied to this body this step
		float get_torque() const;


		// gets the velocity
		glm::vec3 get_velocity() const;

		// sets the velocity
		void set_velocity(const glm::vec3& velocity);

		// gets the rotational velocity
		glm::quat get_rotational_velocity() const;

		// sets the rotational velocity
		void set_rotational_velocity(const glm::quat& rotationalVelocity);


	private:
		bool m_enabled : 1;
		bool m_isSleeping : 1;
		glm::vec3 m_velocity;
		glm::quat m_rotationalVelocity;
		glm::vec3 m_force;
		glm::quat m_torque;
	};

}


#endif // !ALC_PHYSICS3D_BODY3D_HPP