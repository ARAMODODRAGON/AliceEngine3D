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
#ifndef ALC_RENDERING_SPRITEBATCH_HPP
#define ALC_RENDERING_SPRITEBATCH_HPP
#include "../Entities/Registry.hpp"
#include "Camera.hpp"
#include "../Content/Texture.hpp"
#include "../Content/Shader.hpp"
#include "../Physics/Transform2D.hpp"

namespace ALC {

	struct SpriteComponent final {

		// disables the spritecomponent from drawing
		bool shouldDraw = true;

		// the offset from the Entity's center
		vec2 offset = vec2(0.0f);

		// the area in which to draw the sprite 
		// relative to the center + offset
		rect bounds;

		// the area of the texture to draw to screen
		// where 0,0 is the top left
		// and bottom left is the texture size in pixels
		rect textureBounds;

		// the texture to draw to screen
		Texture texture;

		// the color of the sprite,
		// if no texture then the sprite is a solid color
		vec4 color = ALC_COLOR_WHITE;

		// a layer value that can be used to specifically draw a set of sprites
		int32 layer = 0;

	};


	class SpriteBatch final {
		ALC_NO_COPY(SpriteBatch)
	public:

		SpriteBatch();
		~SpriteBatch();

		// begin drawing the sprites
		void Begin(Camera& camera, Shader shader = nullptr);

		// draw a sprite 
		void Draw(const Transform2D& transform, const SpriteComponent& sprite);

		// draws a sprite with given values
		void Draw(const vec2& position, const rect& bounds, const rect& textureBounds, 
				  const Texture& texture = nullptr, const vec4& color = ALC_COLOR_WHITE);

		// draw all sprite components in registry
		void DrawComponents(Registry& reg);
		
		// draw all sprite components in registry with the given layer value
		void DrawComponents(Registry& reg, int32 layer);

		// finish drawing the sprites
		// uses the default shader if no shader is specified
		void End();

	private:

		struct vertex {
			vec2 position;
			vec2 uvcoords;
			vec4 color;
			int32 textureIndex = -1;
		};

		vector<vertex> m_verticies;
		vector<uint32> m_textures;
		uint32 m_vao, m_vbo;
		uint32 m_TextureCountLoc;
		uint32 m_bufferSize;
		Camera* m_camera;
		Shader m_defaultShader;
		Shader m_currentShader;

		uint32 TryAddTexture(const Texture& texture);
		void DrawCurrent();
	};

}

#endif // !ALC_RENDERING_SPRITEBATCH_HPP