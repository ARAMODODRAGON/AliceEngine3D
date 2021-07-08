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
#ifndef ALC_DATATYPES_RECT_HPP
#define ALC_DATATYPES_RECT_HPP
#include <glm\glm.hpp>

namespace ALC {

	using glm::vec2;

	struct Bounds2D {
		// data
		union {
			struct { vec2 min, max; };
			struct { float left, bottom, right, top; };
		};

		Bounds2D() : min(0.0f), max(0.0f) { }
		Bounds2D(const float halfSize_) : min(-halfSize_), max(halfSize_) { }
		Bounds2D(const vec2& min_, const vec2& max_)
			: min(min_), max(max_) { }
		Bounds2D(const float left_, const float bottom_, const float right_, const float top_)
			: min(left_, bottom_), max(right_, top_) { }

		float Width() const { return fabs(right - left); }
		float Height() const { return fabs(top - bottom); }

		// returns a centered copy of this Bounds2D
		Bounds2D Centered() const { 
			Bounds2D centered;
			centered.min = vec2(-Width() * 0.5f, -Height() * 0.5f);
			centered.max = -centered.min;
			return centered;
		}
			
		static bool Intersects(const Bounds2D& r0, const Bounds2D& r1) {
			if (r0.min.x > r1.max.x || r0.max.x < r1.min.x) return false;
			if (r0.min.y > r1.max.y || r0.max.y < r1.min.y) return false;
			return true;
		}

	};

}

#endif // !ALC_DATATYPES_RECT_HPP