#include "mesh.hpp"
#include "../core/debug.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "../dependencies/tiny_obj_loader.h"

namespace alc {

	mesh mesh::load(const std::string& filepath) {

		// create static vectors and reserve some memory
		static tinyobj::attrib_t attrib = {
			std::vector<tinyobj::real_t>(1000),
			std::vector<tinyobj::real_t>(1000),
			std::vector<tinyobj::real_t>(1000)
		};
		static std::vector<tinyobj::shape_t> shapes(1000);
		static std::vector<tinyobj::material_t> material(1000);

		// error message
		static std::string err = std::string(500, ' ');
		err.clear();

		// load object
		bool success = tinyobj::LoadObj(&attrib, &shapes, &material, &err, filepath.c_str());

		// check for failure
		if (!success) {
			ALC_DEBUG_ERROR("object load error: \"" + err + "\"");
			return mesh();
		}

		// create mesh object
		mesh m;

		ALC_DEBUG_WARNING("Function incomplete. Do not use");

		return mesh();
	}

	bool mesh::unload(mesh& m) {
		ALC_DEBUG_WARNING("Function incomplete. Do not use");
		return false;
	}


	mesh::mesh(std::nullptr_t) {
		ALC_DEBUG_WARNING("Function incomplete. Do not use");
	}

	bool mesh::is_valid() const {
		ALC_DEBUG_WARNING("Function incomplete. Do not use");
		return false;
	}

	mesh::operator bool() const {
		ALC_DEBUG_WARNING("Function incomplete. Do not use");
		return is_valid();
	}

	bool mesh::operator==(const mesh& other) const {
		ALC_DEBUG_WARNING("Function incomplete. Do not use");
		return false;
	}

	bool mesh::operator!=(const mesh& other) const {
		ALC_DEBUG_WARNING("Function incomplete. Do not use");
		return false;
	}

}