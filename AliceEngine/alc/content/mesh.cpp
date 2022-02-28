#include "mesh.hpp"
#include <glew.h>
#include "../core/debug.hpp"

namespace alc {

	static uint32 UpdateOptToGL(mesh_update_opt opt) {
		switch (opt) {
			case mesh_update_opt::stream_draw: return GL_STREAM_DRAW;
			case mesh_update_opt::stream_read: return GL_STREAM_READ;
			case mesh_update_opt::stream_copy: return GL_STREAM_COPY;
			case mesh_update_opt::static_draw: return GL_STATIC_DRAW;
			case mesh_update_opt::static_read: return GL_STATIC_READ;
			case mesh_update_opt::static_copy: return GL_STATIC_COPY;
			case mesh_update_opt::dynamic_draw: return GL_DYNAMIC_DRAW;
			case mesh_update_opt::dynamic_read: return GL_DYNAMIC_READ;
			case mesh_update_opt::dynamic_copy: return GL_DYNAMIC_COPY;
			default: return GL_STATIC_DRAW;
		}
	}

	mesh mesh::create(const std::vector<vertex>& verticies,
					  const std::vector<uint32>& indicies,
					  const mesh_update_opt opt) {
		// TODO: implement indicies as an option
		// for now just print an error and exit
		if (indicies.size() > 0) {
			ALC_DEBUG_ERROR("Cannot use indicies");
			return mesh();
		}

		// create variabls
		uint32 vao = -1, vbo = -1, ebo = -1;

		// create vao 
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// create vbo
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(vertex), verticies.data(), UpdateOptToGL(opt));

		// upload buffer data 

		// attribute 0
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, position));

		// attribute 1
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, normal));

		// attribute 1
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, uv));

		// unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// set variables
		mesh m;
		m.m_data.reset(new mesh::data_t());
		m.m_data->vao = vao;
		m.m_data->vbo = vbo;
		m.m_data->ebo = ebo;
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