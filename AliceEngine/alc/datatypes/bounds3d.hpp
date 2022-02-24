#ifndef ALC_DATATYPES_ALIGNED_BOUNDS3D_HPP
#define ALC_DATATYPES_ALIGNED_BOUNDS3D_HPP
#include "../common.hpp"

namespace alc {

	// an axis aligned bounding box
	struct bounds3d final {
		glm::vec3 min, max;

		bounds3d() : min(0.0f), max(0.0f) { }

		bounds3d(glm::vec3 min_, glm::vec3 max_)
			: min(min_), max(max_) { }

		// checks if this bounds encapsulates the given bounds
		bool contains(const bounds3d& other) {
			if (min.x > other.min.x || max.x < other.max.x) return false;
			if (min.y > other.min.y || max.y < other.max.y) return false;
			if (min.z > other.min.z || max.z < other.max.z) return false;
			return true;
		}


		// checks if the two given bounds are intersecting
		static bool intersect(const bounds3d& a, const bounds3d& b) {
			if (a.min.x >= b.max.x || a.max.x <= b.min.x) return false;
			if (a.min.y >= b.max.y || a.max.y <= b.min.y) return false;
			if (a.min.z >= b.max.z || a.max.z <= b.min.z) return false;
			return true;
		}

	};

}

#endif // !ALC_DATATYPES_ALIGNED_BOUNDS3D_HPP