#include "mesh.hpp"
#include <glew.h>
#include "../core/debug.hpp"

namespace alc {

	static uint32 update_opt_to_gl(buffer_update_opt opt) {
		switch (opt) {
			case buffer_update_opt::stream_draw: return GL_STREAM_DRAW;
			case buffer_update_opt::stream_read: return GL_STREAM_READ;
			case buffer_update_opt::stream_copy: return GL_STREAM_COPY;
			case buffer_update_opt::static_draw: return GL_STATIC_DRAW;
			case buffer_update_opt::static_read: return GL_STATIC_READ;
			case buffer_update_opt::static_copy: return GL_STATIC_COPY;
			case buffer_update_opt::dynamic_draw: return GL_DYNAMIC_DRAW;
			case buffer_update_opt::dynamic_read: return GL_DYNAMIC_READ;
			case buffer_update_opt::dynamic_copy: return GL_DYNAMIC_COPY;
			default: return GL_STATIC_DRAW;
		}
	}

	static uint32 attribute_type_to_gl(attribute_type type) {
		switch (type) {
			case alc::attribute_type::type_byte: return GL_BYTE;
			case alc::attribute_type::type_unsigned_byte: return GL_UNSIGNED_BYTE;
			case alc::attribute_type::type_short: return GL_SHORT;
			case alc::attribute_type::type_unsigned_short: return GL_UNSIGNED_SHORT;
			case alc::attribute_type::type_int: return GL_INT;
			case alc::attribute_type::type_unsigned_int: return GL_UNSIGNED_INT;
			case alc::attribute_type::type_float: return GL_FLOAT;
			case alc::attribute_type::type_double: return GL_DOUBLE;
			default: return 0;
		}
	}

	mesh_info mesh::basic_vertex::basic_info() {
		static mesh_info info;
		if (info.vertexSize == 0) {
			info.vertexSize = sizeof(basic_vertex);
			info.attributes.emplace_back(attribute_type::type_float, 3, (void*)offsetof(basic_vertex, position));
			info.attributes.emplace_back(attribute_type::type_float, 3, (void*)offsetof(basic_vertex, normal));
			info.attributes.emplace_back(attribute_type::type_float, 2, (void*)offsetof(basic_vertex, uv));
		}
		return info;
	}

	mesh mesh::create(const void* verticies, const size_t verticies_size,
					  const void* indices, const size_t indices_size,
					  const mesh_info& info) {
		//// TODO: implement indicies as an option
		//// for now just print an error and exit
		//if (indicies_size > 0) {
		//	ALC_DEBUG_ERROR("Cannot use indicies");
		//	return mesh();
		//}

		// create variabls
		uint32 vao = -1, vbo = -1, ebo = -1;

		// create vao 
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// create vbo
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		size_t verticiesTotalSize = verticies_size * info.vertexSize;
		glBufferData(GL_ARRAY_BUFFER, verticiesTotalSize, verticies,
					 update_opt_to_gl(info.bufferUpdateOption));

		// define attributes
		for (size_t i = 0; i < info.attributes.size(); i++) {
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, info.attributes[i].components,
								  attribute_type_to_gl(info.attributes[i].type),
								  GL_FALSE, info.vertexSize, info.attributes[i].offset);
		}

		// create ebo if needed
		if (indices) {
			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size * sizeof(uint32), indices, GL_STATIC_DRAW);
		}

		// unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// set variables
		mesh m;
		m.m_data.reset(new mesh::data_t());
		m.m_data->vao = vao;
		m.m_data->vbo = vbo;
		m.m_data->ebo = ebo;
		m.m_data->vboSize = verticies_size;
		m.m_data->eboSize = indices_size;
		m.m_data->meshInfo = info; // copy this as we may need it later
		return m;
	}

	bool mesh::unload(mesh& m) {
		if (m.m_data->vao == -1) return false;
		// delete
		glDeleteVertexArrays(1, &m.m_data->vao);
		if (m.m_data->vbo != -1) glDeleteBuffers(1, &m.m_data->vbo);
		if (m.m_data->ebo != -1) glDeleteBuffers(1, &m.m_data->ebo);
		return true;
	}

	mesh::mesh(std::nullptr_t) : m_data(nullptr) { }

	bool mesh::is_valid() const {
		return m_data.get();
	}

	mesh::operator bool() const {
		return is_valid();
	}

	bool mesh::update_vertex_data(void* verticies, const size_t verticies_size) {
		const size_t newSize = m_data->meshInfo.vertexSize * verticies_size;
		uint32 vboFullSize = m_data->vboSize * m_data->meshInfo.vertexSize;

		// bind
		glBindVertexArray(m_data->vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_data->vbo);

		// resize if needed
		if (vboFullSize < newSize) {
			glBufferData(GL_ARRAY_BUFFER, newSize, nullptr, GL_STREAM_DRAW);
			m_data->vboSize = verticies_size;
		}

		// update data
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_data->vboSize * m_data->meshInfo.vertexSize, verticies);

		// unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		return true;
	}

	long mesh::get_shared_count() const {
		return m_data.use_count();
	}

	uint32 mesh::get_vao() const {
		if (m_data) return m_data->vao;
		return 0;
	}

	uint32 mesh::get_vbo() const {
		if (m_data) return m_data->vbo;
		return 0;
	}

	uint32 mesh::get_ebo() const {
		if (m_data) return m_data->ebo;
		return 0;
	}

	uint32 mesh::get_vertex_size() const {
		if (m_data) return m_data->vboSize;
		return 0;
	}

	uint32 mesh::get_indicies_size() const {
		if (m_data) return m_data->eboSize;
		return 0;
	}

}