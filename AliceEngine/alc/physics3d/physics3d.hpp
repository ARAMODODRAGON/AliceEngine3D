#ifndef ALC_PHYSICS3D_PHYSICS3D_HPP
#define ALC_PHYSICS3D_PHYSICS3D_HPP
#include "../common.hpp"
#include "body3d.hpp"

namespace alc {

	// the system that handles doing motion, kinematics, and collisions
	class physics3d final {
		ALC_STATIC_CLASS(physics3d);
	public:


	private:
		static inline std::vector<staticbody3d*> s_staticbodies;
		static inline std::vector<rigidbody3d*> s_rigidbodies;
	private:
		static void __init();
		static void __exit();
		static void __step(timestep ts);
		static void __add_body(staticbody3d* sb);
		static void __remove_body(const staticbody3d* sb);
		static void __add_body(rigidbody3d* rb);
		static void __remove_body(const rigidbody3d* rb);
	};

}

#endif // !ALC_PHYSICS3D_PHYSICS3D_HPP