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
#include "Window.hpp"
#include "Debugger.hpp"
#include <SDL.h>
#include <SDL_mixer.h>
#include <glew.h>
#include <stdexcept>

namespace ALC {

	Window::Window(const string& windowTitle_, const uvec2& screenSize_)
		: window(nullptr), glContext(nullptr), screenSize(0) {

		screenSize = screenSize_;
		windowTitle = windowTitle_;

		#pragma region SDL & Window initialization

		// initialize sdl and mixer
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
			ALC_DEBUG_FATAL_ERROR("Failed to init SDL");
			throw std::runtime_error("Failed to init SDL");
		}

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 4, 4096) < 0) {
			ALC_DEBUG_FATAL_ERROR("Mixer failed to initialize. Error:\n" + string(Mix_GetError()));
			throw std::runtime_error("Mixer failed to initialize");
		}

		// set some attributes
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		// this matches to the refresh rate of the display
		SDL_GL_SetSwapInterval(1);

		// experimental? something something, yea!
		glewExperimental = GL_TRUE;


		// create window
		window = SDL_CreateWindow(
			windowTitle.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			screenSize.x,
			screenSize.y,
			SDL_WINDOW_OPENGL
		);

		// check if the window was created
		if (!window) {
			ALC_DEBUG_FATAL_ERROR("Failed to create window");
			throw std::runtime_error("Failed to create window");
		}

		// create context
		glContext = SDL_GL_CreateContext(window);

		// check if it was created
		GLenum error = glewInit();
		if (error != GLEW_OK) {
			ALC_DEBUG_FATAL_ERROR("Failed to initialize GLEW");
			throw std::runtime_error("Failed to initialize GLEW");
		}

		// enable this shit
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// print graphics card and opengl version
		ALC_DEBUG_LOG("Graphics card: " + string(reinterpret_cast<const char*>(glGetString(GL_RENDERER))));
		ALC_DEBUG_LOG("OpenGL version: " + string(reinterpret_cast<const char*>(glGetString(GL_VERSION))));

		#pragma endregion
	}

	Window::~Window() {
		// destroy window and context
		SDL_GL_DeleteContext(glContext);
		SDL_DestroyWindow(window); window = nullptr;

		Mix_Quit();
		SDL_Quit();
	}

	void Window::ClearScreen(const vec4& color) {
		// clear screen
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Window::SwapBuffers() {
		// swap buffers
		SDL_GL_SwapWindow(window);
	}

}