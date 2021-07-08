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
#include "SpriteShaderSource.hpp"
#include <glew.h>
#include <stdexcept>

// the shader used in rendering sprites
// two seperate strings because we need to insert a number at "c_TextureCount"
static constexpr const char* sprbatchShaderSrc[] = { R""(
#type vertex
#version 450 core
layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_uvcoords;
layout (location = 2) in vec4 a_color;
layout (location = 3) in int a_textureIndex;

uniform mat4 u_transform;

out vec4 v_color;
out vec2 v_uvcoords;
out flat int v_textureIndex;

void main() {

	v_color = a_color;
	v_textureIndex = a_textureIndex;
	v_uvcoords = a_uvcoords;
	vec4 vertex = u_transform * vec4(a_position, 0.0, 1.0);
	gl_Position = vertex;
	
}

#type fragment
#version 450 core
out vec4 out_fragcolor;

const int c_TextureCount = )"", R""(;
layout (binding = 0) uniform sampler2D u_textures[c_TextureCount];

in vec4 v_color;
in vec2 v_uvcoords;
in flat int v_textureIndex;

void main() {

	// no texture
	if (v_textureIndex == -1) {
		out_fragcolor = v_color;
	} 
	// font texture
	else if (v_textureIndex < -1) {
		out_fragcolor = texture(u_textures[v_textureIndex + c_TextureCount], v_uvcoords).r * v_color;
	} 
	// normal texture
	else {
		out_fragcolor = texture(u_textures[v_textureIndex], v_uvcoords) * v_color;
	} 
	
}

)"" };

namespace ALC {
	namespace detail {
		uint32 GetMaxTextureCount() {
			static int32 count = -1;
			if (count == -1) {
				glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &count);
				ALC_DEBUG_LOG("Max textures per batch: " + VTOS(count));
			}
			return count;
		}
		Shader GetSpriteShader() {
			static string spriteShaderSource = "";
			if (spriteShaderSource == "") {
				GLint maxTextureCount = GetMaxTextureCount();
				if (maxTextureCount == -1) throw std::runtime_error("m_maxtextures was -1");
				spriteShaderSource = sprbatchShaderSrc[0] + VTOS(maxTextureCount) + sprbatchShaderSrc[1];
			}
			return ContentManager::LoadShaderSource(ContentManager::Default(), spriteShaderSource);
		}
	}
}
