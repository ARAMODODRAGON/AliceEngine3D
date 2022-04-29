#ifndef ALC_PHYSICS3D_COLLIDER3D_HPP
#define ALC_PHYSICS3D_COLLIDER3D_HPP
#include "body3d.hpp"

namespace alc {

	class collider3d : public object3d {
	public:

		collider3d();
		virtual ~collider3d() = 0;

		// returns the first rigidbody that is this collider's ancestor
		rigidbody3d* get_rigidbody() const;

		// returns the first staticbody that is this collider's ancestor
		staticbody3d* get_staticbody() const;

		// returns the first kinematicbody that is this collider's ancestor
		//kinematicbody3d* get_kinematicbody() const;

		// overridden events
		void on_create() override;
		void on_destroy() override;

	private:
		rigidbody3d* m_rigidbody;
		staticbody3d* m_staticbody;
		//kinematicbody3d* m_kinematicbody;
	};

}

#endif // !ALC_PHYSICS3D_COLLIDER3D_HPP