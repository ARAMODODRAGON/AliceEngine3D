#include "window.hpp"
#include <SDL.h>
#include <glew.h>
#include <stdexcept>
#include "debug.hpp"

namespace alc {

	window::window(const std::string& windowTitle_, const glm::uvec2& screenSize_)
		: m_window(nullptr), m_glContext(nullptr), m_screenSize(0) {

		m_screenSize = screenSize_;
		m_windowTitle = windowTitle_;

		#pragma region SDL & Window initialization

		// initialize sdl and mixer
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			ALC_DEBUG_FATAL_ERROR("Failed to init SDL");
			throw std::runtime_error("Failed to init SDL");
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
		m_window = SDL_CreateWindow(
			m_windowTitle.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			m_screenSize.x,
			m_screenSize.y,
			SDL_WINDOW_OPENGL
		);

		// check if the window was created
		if (!m_window) {
			ALC_DEBUG_FATAL_ERROR("Failed to create window");
			throw std::runtime_error("Failed to create window");
		}

		// create context
		m_glContext = SDL_GL_CreateContext(m_window);

		// setup
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);

		// check if it was created
		GLenum error = glewInit();
		if (error != GLEW_OK) {
			ALC_DEBUG_FATAL_ERROR("Failed to initialize GLEW");
			throw std::runtime_error("Failed to initialize GLEW");
		}

		// enable this shit
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_CULL_FACE);
		//glFrontFace(GL_CCW);
		//glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_EQUAL);

		// print graphics card and opengl version
		ALC_DEBUG_LOG("Graphics card: " + std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER))));
		ALC_DEBUG_LOG("OpenGL version: " + std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))));

		#pragma endregion
	}

	window::~window() {
		// destroy window and context
		SDL_GL_DeleteContext(m_glContext);
		SDL_DestroyWindow(m_window); m_window = nullptr;

		//Mix_Quit();
		SDL_Quit();
	}

	void window::clear_screen(const glm::vec4& color) {
		// clear screen
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void window::swap_buffers() {
		// swap buffers
		SDL_GL_SwapWindow(m_window);
	}

	glm::vec2 window::get_screen_size() const {
		return m_screenSize;
	}

}