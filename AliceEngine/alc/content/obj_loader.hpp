#ifndef ALC_CONTENT_OBJ_LOADER
#define ALC_CONTENT_OBJ_LOADER
#include "../common.hpp"
#include "content_manager.hpp"

namespace alc {

	namespace tools {

		// loads a series of meshes from a .obj file
		size_t load_obj(const std::string& filepath, std::vector<std::pair<mesh_ref, material>>& out);

	}
}

#endif // !ALC_CONTENT_OBJ_LOADER