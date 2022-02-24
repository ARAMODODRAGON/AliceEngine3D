#include "mesh.hpp"
#include <glew.h>
#include "../core/debug.hpp"

namespace alc {

	mesh mesh::create(const std::vector<vertex>& verticies) {
		return create(verticies, std::vector<uint32>());
	}

	mesh mesh::create(const std::vector<vertex>& verticies, const std::vector<uint32>& indicies) {
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
		glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(vertex), verticies.data(), GL_STATIC_DRAW);

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
		m.m_data->ebo = -1;
		return m;
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