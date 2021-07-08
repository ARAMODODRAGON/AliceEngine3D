#include "SpriteBatch.hpp"
#include "detail\SpriteShaderSource.hpp"
#include <glew.h>
#include "../Core/SceneManager.hpp"

namespace ALC {

	namespace {

		struct vertex {
			vec2 position;
			vec2 uvcoords;
			vec4 color;
			int32 textureIndex = -1;
		};

		vector<vertex> m_verticies;
		vector<uint32> m_textures;
		uint32 m_vao = -1;
		uint32 m_vbo = -1;
		uint32 m_TextureCountLoc = -1;
		uint32 m_bufferSize = 0;
		Shader m_shader;
		uint32 m_transformLoc = -1;
		//Shader m_currentShader;

		uint32 TryAddTexture(const Texture& texture);
		void DrawCurrent();
	}

	void SpriteBatch::__Init() {
		// make sure our shader is loaded
		// set the max texture count
		m_shader = detail::GetSpriteShader();
		m_transformLoc = m_shader.GetUniform("u_transform");

		// allocate the texture vector to match the max number
		m_textures.reserve(detail::GetMaxTextureCount());

		// allocate some memory for the vertex vector
		m_verticies.reserve(100);

		// create our VAO
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		// create our VBO
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STREAM_DRAW);

		// set the attributes

		// set the position to location 0
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, position));

		// set the uvcoords to location 1
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, uvcoords));

		// set the color to location 2
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, color));

		// set the textureIndex to location 3
		glEnableVertexAttribArray(3);
		glVertexAttribIPointer(3, 1, GL_INT, sizeof(vertex), (GLvoid*)offsetof(vertex, textureIndex));

		// unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void SpriteBatch::__Exit() {
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		m_verticies.clear();
		m_textures.clear();
	}


	void SpriteBatch::Begin() {
		Begin(Bounds2D(vec2(0.0f), SceneManager::GetWindow()->GetScreenSize()));
	}

	void SpriteBatch::Begin(const Bounds2D& bounds) {
		mat4 transform = glm::ortho(bounds.left, bounds.right, bounds.top, bounds.bottom);
		Begin(transform);
	}

	void SpriteBatch::Begin(const Camera2D& camera) {
		Begin(camera.GetTransform());
	}

	void SpriteBatch::Begin(const mat4& transform) {
		m_textures.clear();
		m_verticies.clear();

		// set the shader
		glUseProgram(m_shader);

		// bind vertex array and buffer
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		// set uniform data
		glUniformMatrix4fv(m_transformLoc, 1, GL_FALSE, &(transform[0].x));
	}

	void SpriteBatch::End() {
		// draw any remaining verticies
		DrawCurrent();

		// unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	void SpriteBatch::Draw(const Bounds2D& quad, const vec4& color) {
		// dont draw
		if (NearlyEqual(color.a, 0.0f)) return;

		// create verticies
		vertex verts[4];

		// set texture index
		verts[0].textureIndex = verts[1].textureIndex
			= verts[2].textureIndex = verts[3].textureIndex = -1;

		// set the positions
		/* bottom left  */ verts[0].position = quad.min;
		/* top left     */ verts[1].position = vec2(quad.left, quad.top);
		/* top right    */ verts[2].position = quad.max;
		/* bottom right */ verts[3].position = vec2(quad.right, quad.bottom);

		// set color
		verts[0].color = verts[1].color
			= verts[2].color = verts[3].color = color;

		// push into vector
		m_verticies.push_back(verts[0]);
		m_verticies.push_back(verts[1]);
		m_verticies.push_back(verts[2]);
		m_verticies.push_back(verts[0]);
		m_verticies.push_back(verts[2]);
		m_verticies.push_back(verts[3]);

		// finish
	}

	void SpriteBatch::Draw(const Bounds2D& quad, const Texture& texture, const vec4& color) {
		// dont draw
		if (NearlyEqual(color.a, 0.0f)) return;

		// create verticies
		vertex verts[4];

		// set texture index
		verts[0].textureIndex = verts[1].textureIndex
			= verts[2].textureIndex = verts[3].textureIndex = TryAddTexture(texture);

		// set the positions
		/* bottom left  */ verts[0].position = quad.min;
		/* top left     */ verts[1].position = vec2(quad.left, quad.top);
		/* top right    */ verts[2].position = quad.max;
		/* bottom right */ verts[3].position = vec2(quad.right, quad.bottom);

		// set color
		verts[0].color = verts[1].color
			= verts[2].color = verts[3].color = color;

		// default uvs
		/* bottom left  */ verts[0].uvcoords = vec2(0.0f, 0.0f);
		/* top left     */ verts[1].uvcoords = vec2(0.0f, 1.0f);
		/* top right    */ verts[2].uvcoords = vec2(1.0f, 1.0f);
		/* bottom right */ verts[3].uvcoords = vec2(1.0f, 0.0f);

		// push into vector
		m_verticies.push_back(verts[0]);
		m_verticies.push_back(verts[1]);
		m_verticies.push_back(verts[2]);
		m_verticies.push_back(verts[0]);
		m_verticies.push_back(verts[2]);
		m_verticies.push_back(verts[3]);

		// finish
	}

	void SpriteBatch::Draw(const Bounds2D& quad, const Texture& texture, const Bounds2D& target, const vec4& color) {
		// dont draw
		if (NearlyEqual(color.a, 0.0f)) return;

		// create verticies
		vertex verts[4];

		// set texture index
		verts[0].textureIndex = verts[1].textureIndex
			= verts[2].textureIndex = verts[3].textureIndex = TryAddTexture(texture);

		// set the positions		
		/* bottom left  */ verts[0].position = quad.min;
		/* top left     */ verts[1].position = vec2(quad.left, quad.top);
		/* top right    */ verts[2].position = quad.max;
		/* bottom right */ verts[3].position = vec2(quad.right, quad.bottom);

		// set color
		verts[0].color = verts[1].color
			= verts[2].color = verts[3].color = color;

		// given uvs
		vec2 size(texture.GetSize());
		if (!NearlyZero(size)) size = vec2(1.0f) / size;
		/* bottom left  */ verts[0].uvcoords = target.min * size;
		/* top left     */ verts[1].uvcoords = vec2(target.left, target.top) * size;
		/* top right    */ verts[2].uvcoords = target.max * size;
		/* bottom right */ verts[3].uvcoords = vec2(target.right, target.bottom) * size;

		// push into vector
		m_verticies.push_back(verts[0]);
		m_verticies.push_back(verts[1]);
		m_verticies.push_back(verts[2]);
		m_verticies.push_back(verts[0]);
		m_verticies.push_back(verts[2]);
		m_verticies.push_back(verts[3]);

		// finish
	}

	void SpriteBatch::DrawTriangle(const SpriteVertex& sv0, const SpriteVertex& sv1, const SpriteVertex& sv2, const Texture& texture) {
		if (NearlyZero(sv0.color.a) && NearlyZero(sv1.color.a) && NearlyZero(sv2.color.a)) return;

		// create verticies
		vertex verts[3];

		// set texture index
		verts[0].textureIndex = verts[1].textureIndex
			= verts[2].textureIndex = TryAddTexture(texture);

		// set the positions
		verts[0].position = sv0.position;
		verts[1].position = sv1.position;
		verts[2].position = sv2.position;

		// set color
		verts[0].color = sv0.color;
		verts[1].color = sv1.color;
		verts[2].color = sv2.color;

		// set uvs
		verts[0].uvcoords = sv0.uvcoords;
		verts[1].uvcoords = sv1.uvcoords;
		verts[2].uvcoords = sv2.uvcoords;

		// push into vector
		m_verticies.push_back(verts[0]);
		m_verticies.push_back(verts[1]);
		m_verticies.push_back(verts[2]);

		// finish
	}


	namespace {

		uint32 TryAddTexture(const Texture& texture) {
			// solid color
			if (texture == nullptr)
				return -1;

			// check if its already in the vector
			for (size_t i = 0; i < m_textures.size(); i++) {
				if (m_textures[i] == texture) {
					return i;
				}
			}

			// try add
			if (m_textures.size() < m_textures.capacity()) {
				m_textures.push_back(texture);
				return m_textures.size() - 1;
			}

			// batch break
			DrawCurrent();
			m_textures.push_back(texture);
			return 0;
		}

		void DrawCurrent() {
			if (m_verticies.size() == 0)
				return;
			const uint32 bytes = (sizeof(vertex) * m_verticies.size());

			// resize the buffer if needed
			if (m_bufferSize < bytes) {
				if (m_bufferSize == 0) m_bufferSize = bytes;
				else m_bufferSize *= 2;
				glBufferData(GL_ARRAY_BUFFER, m_bufferSize, nullptr, GL_STREAM_DRAW);
			}

			// update the vertex data
			glBufferSubData(GL_ARRAY_BUFFER, 0, bytes, m_verticies.data());

			// load in the textures
			for (size_t i = 0; i < m_textures.size(); i++) {
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, m_textures[i]);
			}

			// draw
			glDrawArrays(GL_TRIANGLES, 0, m_verticies.size());

			// clear out vectors
			m_textures.clear();
			m_verticies.clear();
		}

	}
}