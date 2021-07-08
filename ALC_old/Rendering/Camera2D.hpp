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
#ifndef ALC_RENDERING_CAMERA_HPP
#define ALC_RENDERING_CAMERA_HPP
#include "../General.hpp"

namespace ALC {

	class Camera2D {
	public:

		// default constructs the camera where the size matches the window size
		Camera2D();

		~Camera2D();

		// getters & setters

		vec2 GetPosition();
		void SetPosition(const vec2& position_);

		// camera size in world units
		vec2 GetCameraSize() const;
		// camera size in world units
		void SetCameraSize(const vec2& size_);

		// returns the transformation matrix
		mat4 GetTransform() const;

		// returns a matrix that converts from screen to world positions
		// uses the built in window size
		mat4 GetScreenToWorld() const;

		// returns a matrix that converts from screen to world positions
		mat4 GetScreenToWorld(const vec2& screensize) const;

		// returns a matrix that converts from world to screen positions
		// uses the built in window size
		mat4 GetWorldToScreen() const;

		// returns a matrix that converts from world to screen positions
		mat4 GetWorldToScreen(const vec2& screensize) const;

	private:
		vec2 m_position;
		vec2 m_size;
	};

}

#endif // !ALC_RENDERING_CAMERA_HPP