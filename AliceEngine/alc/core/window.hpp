#ifndef ALC_CORE_WINDOW_HPP
#define ALC_CORE_WINDOW_HPP
#include "../common.hpp"

struct SDL_Window;

namespace alc {

	class window final {
	public:

		window(const std::string& windowTitle_, const glm::uvec2& screenSize_);
		~window();

		// events
		void clear_screen(const glm::vec4& color);
		void swap_buffers();

		// getters & setters
		glm::vec2 get_screen_size() const;

	private:
		SDL_Window* m_window;
		void* m_glContext;
		glm::uvec2 m_screenSize;
		std::string m_windowTitle;
	};

}

#endif // !ALC_CORE_WINDOW_HPP