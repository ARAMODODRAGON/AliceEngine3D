#ifndef ALC_CONTENT_MESH_HPP
#define ALC_CONTENT_MESH_HPP
#include "../common.hpp"

namespace alc {

	// TODO: implement a settings struct for creating a mesh
	//enum class mesh_update_setting : uint8 {
	//
	//};
	//
	//struct mesh_settings final {
	//
	//
	//};

	// represents a set of 3d points in space
	class mesh final {
	public:
		struct vertex final {
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 uv;
		};

		// loads a mesh from a set of verticies
		static mesh create(const std::vector<vertex>& verticies);

		// loads a mesh from a set of verticies and indicies
		static mesh create(const std::vector<vertex>& verticies, const std::vector<uint32>& indicies);

		// deletes a mesh
		static bool unload(mesh& m);


		mesh();

		// checks if this is a valid mesh
		bool is_valid() const;

		// checks if this is a valid mesh
		operator bool() const;

		// gets the number of shared instances of this mesh
		long get_shared_count() const;

	private:
		struct data_t {
			uint32 vao = -1, vbo = -1, ebo = -1;
		};
		std::shared_ptr<data_t> m_data;
	};

}

#endif // !ALC_CONTENT_MESH_HPP