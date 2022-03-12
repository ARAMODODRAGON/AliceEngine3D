#ifndef ALC_CONTENT_MESH_HPP
#define ALC_CONTENT_MESH_HPP
#include "../common.hpp"

namespace alc {

	// determines how a mesh's data is updated
	enum class buffer_update_opt : uint8 {
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

	// used to determine what types each attribute are when generating a mesh
	enum class attribute_type : uint8 {
		type_none,
		type_byte,
		type_unsigned_byte,
		type_short,
		type_unsigned_short,
		type_int,
		type_unsigned_int,
		type_float,
		type_double
	};

	// used to generate a mesh 
	struct mesh_info final {
		struct attribute_info final {
			attribute_type type;
			size_t components;
			void* offset;
			attribute_info(attribute_type type_, size_t components_, void* offset_)
				: type(type_), components(components_), offset(offset_) { }
		};

		buffer_update_opt bufferUpdateOption = buffer_update_opt::static_draw;
		size_t vertexSize = 0;
		std::vector<attribute_info> attributes;
	};

	// represents a mesh
	class mesh final {
		ALC_NO_COPY(mesh) ALC_NO_MOVE(mesh)
	public:
		struct basic_vertex final {
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 uv;

			static mesh_info basic_info();
		};

		// loads a mesh from a set of verticies and indicies
		mesh(const void* verticies, const size_t verticies_size,
			 const void* indices, const size_t indices_size,
			 const mesh_info& info);

		// default constructs an invalid mesh
		mesh();

		// deletes a mesh
		~mesh();

		// checks if this is a valid mesh
		bool is_valid() const;

		// checks if this is a valid mesh
		operator bool() const;

		// updates the vertex data stored within this mesh
		bool update_vertex_data(void* verticies, const size_t verticies_size);

		// updates the indices data stored within this mesh
		//bool update_indices_data(void* indices, const size_t indices_size);

		// gets the vao
		uint32 get_vao() const;

		// gets the vbo
		uint32 get_vbo() const;

		// gets the ebo
		uint32 get_ebo() const;

		// gets the number of stored verticies
		uint32 get_vertex_size() const;

		// gets the number of stored verticies
		uint32 get_indicies_size() const;

	private:
		uint32 m_vao, m_vbo, m_ebo;
		size_t m_vboSize, m_eboSize;
		mesh_info m_meshInfo;
	public:
	};

}

#endif // !ALC_CONTENT_MESH_HPP