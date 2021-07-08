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
#include "UIBatch.hpp"
#include <glew.h>
#include "detail\SpriteShaderSource.hpp"
#include "../SceneManager.hpp"

namespace ALC {

	UIBatch::UIBatch()
		: m_vao(-1), m_vbo(-1), m_TextureCountLoc(-1), m_bufferSize(0) {

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

	UIBatch::~UIBatch() {
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
	}

	void UIBatch::Begin(Shader shader) {
		m_textures.clear();
		m_verticies.clear();

		// set the shader
		Shader currentShader = shader;
		if (currentShader == nullptr) currentShader = m_defaultShader;
		glUseProgram(currentShader);

		// bind vertex array and buffer
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		// set uniform data
		vec2 screensize = SceneManager::GetWindow()->GetScreenSize();
		if (m_screensize.x > 0.0f) screensize.x = m_screensize.x;
		if (m_screensize.y > 0.0f) screensize.y = m_screensize.y;
		mat4 transform = glm::ortho(0.0f, screensize.x, screensize.y, 0.0f);
		glUniformMatrix4fv(currentShader.GetUniform("u_transform"), 1, GL_FALSE, &(transform[0].x));
	}

	void UIBatch::DrawQuad(const rect& position, const vec4& color, const rect& target, const Texture& texture) {
		// dont draw
		if (NearlyEqual(color.a, 0.0f)) return;

		// check if should batch break
		uint32 textureindex = TryAddTexture(texture);
		if (textureindex == -2) {
			DrawCurrent();
			m_textures.push_back(texture);
			textureindex = 0;
		}

		vec2 size = texture.GetSize();
		if (NearlyZero(size)) size = vec2(1.0f);

		// create verticies
		vertex verts[4];

		// set texture index
		verts[0].textureIndex = verts[1].textureIndex
			= verts[2].textureIndex = verts[3].textureIndex = textureindex;

		// set the positions
		/* bottom left  */ verts[0].position = position.min;
		/* top left     */ verts[1].position = vec2(position.left, position.top);
		/* top right    */ verts[2].position = position.max;
		/* bottom right */ verts[3].position = vec2(position.right, position.bottom);

		// set color
		verts[0].color = verts[1].color
			= verts[2].color = verts[3].color = color;

		// default uvs
		if (NearlyEqual(target.min, target.max)) {
			/* bottom left  */ verts[0].uvcoords = vec2(0.0f, 0.0f);
			/* top left     */ verts[1].uvcoords = vec2(0.0f, 1.0f);
			/* top right    */ verts[2].uvcoords = vec2(1.0f, 1.0f);
			/* bottom right */ verts[3].uvcoords = vec2(1.0f, 0.0f);
		}
		// given uvs
		else {
			/* bottom left  */ verts[0].uvcoords = target.min / size;
			/* top left     */ verts[1].uvcoords = vec2(target.left, target.top) / size;
			/* top right    */ verts[2].uvcoords = target.max / size;
			/* bottom right */ verts[3].uvcoords = vec2(target.right, target.bottom) / size;
		}

		// push into vector
		m_verticies.push_back(verts[0]);
		m_verticies.push_back(verts[1]);
		m_verticies.push_back(verts[2]);
		m_verticies.push_back(verts[0]);
		m_verticies.push_back(verts[2]);
		m_verticies.push_back(verts[3]);

		// finish
	}

	void UIBatch::DrawText(const string& text, const Font& font, const vec2& position, const vec4& color, const HAlign hAlign, const VAlign vAlign, const vec2& scale) {
		// dont draw
		if (NearlyEqual(color.a, 0.0f) || text == "") return;

		// font must be valid
		if (font == nullptr) {
			ALC_DEBUG_WARNING("Font must be valid. Ignoring draw call");
			return;
		}

		// check if should batch break
		uint32 textureindex = TryAddTexture(font.GetTexture());
		if (textureindex == -2) {
			DrawCurrent();
			m_textures.push_back(font.GetTexture());
			textureindex = 0;
		}

		// get texture size
		const vec2 size = font.GetSize();

		// make sure our vector is big enough for all the verticies
		m_verticies.reserve(m_verticies.size() + text.size());

		// create verticies
		vertex verts[4];

		// set texture index. subtract 'maxtexturecount' to tell the shader that this is a font
		verts[0].textureIndex = verts[1].textureIndex
			= verts[2].textureIndex = verts[3].textureIndex = textureindex - detail::GetMaxTextureCount();

		// set color
		verts[0].color = verts[1].color
			= verts[2].color = verts[3].color = color;

		vector<float> offsets = font.StringAlignOffsetX(text, hAlign, scale);
		uint32 curLine = 0;

		vec2 offset(-offsets[0], -font.StringAlignOffsetY(text, vAlign, scale) + font.GetFontSize());
		for (const char* p = text.c_str(); *p; p++) {
			if (*p < 32 && *p != '\n') continue;
			// get character
			const Font::Character& c = font[*p];

			// calculate values
			float x0 = position.x + offset.x + ((c.position.x / c.bitSize.x) * c.bitSize.x);
			float y0 = position.y + offset.y - ((c.position.y / c.bitSize.y) * c.bitSize.y);
			float w = c.bitSize.x * scale.x;
			float h = c.bitSize.y * scale.y;

			// move cursor to the start of the next character
			offset.x += c.advance.x * scale.x;

			// skip character with no size
			if ((!w || !h) && *p != '\n') continue;

			if (*p == '\n') { // Newline text
				curLine++; // Move to the next offset position for the text
				offset.x = -offsets[curLine]; // Reset the x offset of the text to the horizontal alignment
				offset.y += (float) (font.GetFontSize() + font.GetVerticalSpacing()) * scale.y;
				continue;
			}

			// set uvCoords
			/* bottom left  */ verts[0].uvcoords = vec2(c.xoffset, 0.0f);
			/* top left     */ verts[1].uvcoords = vec2(c.xoffset, c.bitSize.y / size.y);
			/* top right    */ verts[2].uvcoords = vec2(c.xoffset + c.bitSize.x / size.x, c.bitSize.y / size.y);
			/* bottom right */ verts[3].uvcoords = vec2(c.xoffset + c.bitSize.x / size.x, 0.0f);

			// set positions
			/* bottom left  */ verts[0].position = vec2(x0, y0);
			/* top left     */ verts[1].position = vec2(x0, y0 + h);
			/* top right    */ verts[2].position = vec2(x0 + w, y0 + h);
			/* bottom right */ verts[3].position = vec2(x0 + w, y0);

			// push into vector
			m_verticies.push_back(verts[0]);
			m_verticies.push_back(verts[1]);
			m_verticies.push_back(verts[2]);
			m_verticies.push_back(verts[0]);
			m_verticies.push_back(verts[2]);
			m_verticies.push_back(verts[3]);
		}

		// finish
	}

	void UIBatch::End() {
		// draw any remaining verticies
		DrawCurrent();

		// unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	uint32 UIBatch::TryAddTexture(const Texture& texture) {
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

	void UIBatch::DrawCurrent() {
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