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
#include "../General.hpp"
#include "Camera2D.hpp"
#include "../Content/Content.hpp"

namespace ALC {


	struct SpriteVertex {
		vec2 position;
		vec2 uvcoords;
		vec4 color;
	};

	class SpriteBatch final {
		ALC_NON_CONSTRUCTABLE(SpriteBatch);
	public:

		// begin drawing the scene
		// assumes draw area to match the screen resolution where
		// top left is [0, 0] and bottom right is the resolution
		static void Begin();

		// begin drawing the scene
		// assumes draw area to match the given bounds 
		// where bounds.min is top left and bounds.max is bottom right
		static void Begin(const Bounds2D& bounds);

		// begin drawing the scene
		// the draw area is determined by the cameras transform matrix
		static void Begin(const Camera2D& camera);

		// begin drawing the scene
		// the draw area is determined by the given matrix
		static void Begin(const mat4& transform);

		// end drawing
		static void End();

		// draw a quad 
		static void Draw(const Bounds2D& quad, const vec4& color = ALC_COLOR_WHITE);

		// draw a quad with a texture
		static void Draw(const Bounds2D& quad, const Texture& texture, const vec4& color = ALC_COLOR_WHITE);

		// draw a quad with a texture and texture target
		static void Draw(const Bounds2D& quad, const Texture& texture, const Bounds2D& target, const vec4& color = ALC_COLOR_WHITE);

		// draw a triangle with the given values
		static void DrawTriangle(const SpriteVertex& sv0, const SpriteVertex& sv1, const SpriteVertex& sv2, const Texture& texture = nullptr);

		// TODO: text draw stuff

	public:
		static void __Init();
		static void __Exit();
	};

}

#endif // !ALC_RENDERING_SPRITEBATCH_HPP