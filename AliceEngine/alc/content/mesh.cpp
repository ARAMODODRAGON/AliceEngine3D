#include "mesh.hpp"

namespace alc {

	mesh mesh::create(const std::vector<vertex>& verticies) {
		return create(verticies, std::vector<uint32>());
	}

	mesh mesh::create(const std::vector<vertex>& verticies, const std::vector<uint32>& indicies) {


		return mesh();
	}

	bool mesh::unload(mesh& m) {
		return false;
	}

	mesh::mesh() : m_data(nullptr) { }

	bool mesh::is_valid() const {
		return m_data.get();
	}

	mesh::operator bool() const {
		return is_valid();
	}

	long mesh::get_shared_count() const {
		return m_data.use_count();
	}

}