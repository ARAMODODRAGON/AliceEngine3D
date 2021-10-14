#ifndef ALC_CONTENT_MESH_HPP
#define ALC_CONTENT_MESH_HPP
#include "../common.hpp"
#include "shader.hpp"

namespace alc {

	class mesh {
	public:

		// loads a mesh
		static mesh load(const std::string& filepath);

		// deletes the mesh
		static bool unload(mesh& m);


		// default
		mesh(std::nullptr_t = nullptr);

		// checks if this is a valid mesh
		bool is_valid() const;

		// checks if this is a valid mesh
		operator bool() const;

		// compares this to another mesh
		bool operator==(const mesh& other) const;

		// compares this to another mesh
		bool operator!=(const mesh& other) const;

	private:
		uint32 m_id;
	};

}

#endif // !ALC_CONTENT_MESH_HPP