#ifndef ALC_CONTENT_MESH_HPP
#define ALC_CONTENT_MESH_HPP
#include "../common.hpp"

namespace alc {


	enum class mesh_update_opt : uint8 {
		stream_draw,
		stream_read,
		stream_copy,
		static_draw,
		static_read,
		static_copy,
		dynamic_draw,
		dynamic_read,
		dynamic_copy
	};

	enum class mesh_attribute_opt : uint8 {

	};

	struct meshopts final {
		mesh_update_opt updateOpt;
		std::vector<mesh_attribute_opt> attributes;
	};

	// represents a set of 3d points in space
	class mesh final {
	public:
		struct vertex final {
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 uv;
		};

		// loads a mesh from a set of verticies and indicies
		static mesh create(const std::vector<vertex>& verticies,
						   const std::vector<uint32>& indicies = std::vector<uint32>(),
						   const mesh_update_opt opt = mesh_update_opt::static_draw);

		// loads a mesh from a set of verticies with custom attributes
		static mesh create(const void* verticies, std::vector<uint32>& indicies, const meshopts& opts);

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