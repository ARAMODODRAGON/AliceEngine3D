#ifndef ALC_DATATYPES_ALIGNED_BOUNDS3D_HPP
#define ALC_DATATYPES_ALIGNED_BOUNDS3D_HPP
#include "../common.hpp"

namespace alc {

	// an axis aligned bounding box
	struct alignedbounds3d final {
		glm::vec3 min, max;

		alignedbounds3d() : min(0.0f), max(0.0f) { }

		alignedbounds3d(glm::vec3 min_, glm::vec3 max_)
			: min(min_), max(max_) { }

		// checks if this bounds encapsulates the given bounds
		bool contains(const alignedbounds3d& other) {
			if (min.x > other.min.x || max.x < other.max.x) return false;
			if (min.y > other.min.y || max.y < other.max.y) return false;
			if (min.z > other.min.z || max.z < other.max.z) return false;
			return true;
		}


		// checks if the two given bounds are intersecting
		static bool intersect(const alignedbounds3d& a, const alignedbounds3d& b) {
			if (a.min.x >= b.max.x || a.max.x <= b.min.x) return false;
			if (a.min.y >= b.max.y || a.max.y <= b.min.y) return false;
			if (a.min.z >= b.max.z || a.max.z <= b.min.z) return false;
			return true;
		}

	};

	// an oriented bounding box
	struct orientedbounds3d final {
		glm::vec3 min, max;
		glm::quat rotation;

		orientedbounds3d() : min(0.0f), max(0.0f), rotation() { }

		orientedbounds3d(glm::vec3 min_, glm::vec3 max_, glm::quat rotation)
			: min(min_), max(max_), rotation(rotation) { }

		static bool intersects(const orientedbounds3d& a, const orientedbounds3d& b) {

		}

	};

}

#endif // !ALC_DATATYPES_ALIGNED_BOUNDS3D_HPP