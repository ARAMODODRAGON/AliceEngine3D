#ifndef ALC_DATATYPES_OBJECT_VERTEX_HPP
#define ALC_DATATYPES_OBJECT_VERTEX_HPP
#include "../common.hpp"

namespace alc {

	struct object_vertex final {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
		glm::vec4 color;
	};

}

#endif // !ALC_DATATYPES_OBJECT_VERTEX_HPP