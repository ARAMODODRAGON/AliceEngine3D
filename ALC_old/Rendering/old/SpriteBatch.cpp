/*
* MIT License
*
* Copyright (c) 2021 Domara Shlimon
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include "SpriteBatch.hpp"
#include <glew.h>
#include "../Content/ContentManager.hpp"
#include "detail\SpriteShaderSource.hpp"

namespace ALC {

	SpriteBatch::SpriteBatch()
		: m_vao(-1), m_vbo(-1), m_TextureCountLoc(-1), m_bufferSize(0), m_camera(nullptr) {

		// get the max number of textures per shader
		GLint maxtextures = -1;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxtextures);

		// make sure our shader is loaded
		// set the max texture count
		m_defaultShader = detail::GetSpriteShader();

		// allocate the texture vector to match the max number
		m_textures.reserve(maxtextures);

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

	SpriteBatch::~SpriteBatch() {
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
	}

	void SpriteBatch::Begin(Camera& camera, Shader shader) {
		m_textures.clear();
		m_verticies.clear();

		m_camera = &camera;

		// set the shader
		Shader currentShader = shader;
		if (currentShader == nullptr) currentShader = m_defaultShader;
		glUseProgram(currentShader);

		// bind vertex array and buffer
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		// set uniform data
		mat4 transform = m_camera->GetTransform();
		glUniformMatrix4fv(currentShader.GetUniform("u_transform"), 1, GL_FALSE, &(transform[0].x));

	}

	void SpriteBatch::Draw(const Transform2D& transform, const SpriteComponent& sprite) {
		// quit if should not draw
		if (!sprite.shouldDraw) return;

		// draw the sprite
		Draw(transform.position + sprite.offset, sprite.bounds, sprite.textureBounds, sprite.texture, sprite.color);
	}

	void SpriteBatch::Draw(const vec2& position, const rect& bounds,
						   const rect& textureBounds, const Texture& texture, const vec4& color) {

		// check if should batch break
		uint32 textureindex = TryAddTexture(texture);
		if (textureindex == -2) {
			DrawCurrent();
			m_textures.push_back(texture);
			textureindex = 0;
		}

		const vec2 min = position + bounds.min;
		const vec2 max = position + bounds.max;
		vec2 size = texture.GetSize();
		if (NearlyZero(size)) size = vec2(1.0f);
		const vec2 texmin = textureBounds.min / size;
		const vec2 texmax = textureBounds.max / size;

		// create verticies
		vertex verts[4];

		/* bottom left  */ verts[0].position = min;
		/* top left     */ verts[1].position = vec2(min.x, max.y);
		/* top right    */ verts[2].position = max;
		/* bottom right */ verts[3].position = vec2(max.x, min.y);

		// set color
		verts[0].color = verts[1].color
			= verts[2].color = verts[3].color = color;

		// set texture index
		verts[0].textureIndex = verts[1].textureIndex
			= verts[2].textureIndex = verts[3].textureIndex = textureindex;

		// set uvcoords
		/* bottom left  */ verts[0].uvcoords = vec2(texmin.x, texmax.y);
		/* top left     */ verts[1].uvcoords = texmin;
		/* top right    */ verts[2].uvcoords = vec2(texmax.x, texmin.y);
		/* bottom right */ verts[3].uvcoords = texmax;

		// push into vector
		m_verticies.push_back(verts[0]);
		m_verticies.push_back(verts[1]);
		m_verticies.push_back(verts[2]);
		m_verticies.push_back(verts[0]);
		m_verticies.push_back(verts[2]);
		m_verticies.push_back(verts[3]);

		// finish
	}

	void SpriteBatch::DrawComponents(Registry& reg) {
		reg.ForeachComponent<Transform2D, SpriteComponent>(
			[this](auto entity, auto tr, auto spr) {
			Draw(tr, spr);
		});
	}

	void SpriteBatch::DrawComponents(Registry& reg, int32 layer) {
		reg.ForeachComponent<Transform2D, SpriteComponent>(
			[this, layer](auto entity, auto tr, auto spr) {
			if (spr.layer == layer)
				Draw(tr, spr);
		});
	}

	void SpriteBatch::End() {

		// draw any remaining verticies
		DrawCurrent();

		// unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);

		// clean up
		m_camera = nullptr;
	}

	uint32 SpriteBatch::TryAddTexture(const Texture& texture) {
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

		return -2; // batch break
	}

	void SpriteBatch::DrawCurrent() {
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

/*
// set the shader
if (shader == nullptr) shader = defaultShader;
glUseProgram(shader.GetID());

// bind vertex array and buffer
glBindVertexArray(m_vao);
glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

// update the vertex data
glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex) * 4 * m_verticies.size(), m_verticies.data());

// draw
glDrawArrays(GL_TRIANGLES, 0, m_verticies.size());

// unbind
glBindBuffer(GL_ARRAY_BUFFER, 0);
glBindVertexArray(0);
glUseProgram(0);
*/