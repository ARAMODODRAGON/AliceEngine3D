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
#ifndef _CORE_WINDOW_HPP
#define _CORE_WINDOW_HPP
#include "../General.hpp"

struct SDL_Window;

namespace ALC {

	class Window {

		// window & opengl
		SDL_Window* window;
		void* glContext;
		uvec2 screenSize;
		string windowTitle;

	public:

		Window(const string& windowTitle_, const uvec2& screenSize_);
		~Window();

		// events
		void ClearScreen(const vec4& color);
		void SwapBuffers();

		// getters & setters
		vec2 GetScreenSize() const { return screenSize; }

	};

}

#endif // !_CORE_WINDOW_HPP