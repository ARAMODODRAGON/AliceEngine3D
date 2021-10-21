#include "engine.hpp"
#include "alice_events.hpp"
#include "../input/keyboard.hpp"
#include "../input/mouse.hpp"
#include "../jobs/job_queue.hpp"
#include "../objects/world.hpp"
#include <chrono>
#include <SDL.h>

namespace alc {

	using clock = std::chrono::steady_clock;
	using duration = std::chrono::duration<double>;
	using time_point = std::chrono::time_point<clock, duration>;

	void engine::start(const engine_settings* set) {
		// check if already running and refuse if so
		if (s_isRunning) {
			ALC_DEBUG_WARNING("Could not start since engine was already running!");
			return;
		}
		s_isRunning = true;

		// initialize /////////////////////////////////////////////////

		// create timer
		time_point lasttime, thistime;
		lasttime = thistime = clock::now();

		// create window
		s_window = new window(set->window.titlebar, set->window.size);

		// enable job system
		const bool jobs_enabled = set->jobs.enabled;
		if (jobs_enabled) job_queue::__init(set->jobs.maxthreads);

		// enable world
		const bool world_enabled = set->objects.enabled;
		if (world_enabled) world::__init();

		// enable scene_manager
		const bool scenes_enabled = set->objects.scenemanager.enabled;
		if (scenes_enabled) scene_manager::__init(set);

		// create game
		if (set->gameBinding) {
			s_game = set->gameBinding();
			s_game->init();
		}

		// game loop //////////////////////////////////////////////////

		while (s_isRunning && !s_shouldQuit) {

			// handle events before getting new time
			handle_events();

			// get begining of frame and timestep
			lasttime = thistime;
			thistime = clock::now();
			timestep ts(duration(thistime - lasttime).count());

			// update
			if (s_game) s_game->update(ts);
			alice_events::onUpdate(ts);
			if (scenes_enabled) scene_manager::__update(ts);
			if (world_enabled) world::__update(ts);

			// clear screen
			s_window->clear_screen(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

			// render


			// swap buffers
			s_window->swap_buffers();

			// wait for end of frame
			if (s_targetFramerate != 0) {
				double length = duration(clock::now() - thistime).count();
				while (length < s_frameLength) std::this_thread::yield();
			}
		}

		// destroy ////////////////////////////////////////////////////

		// delete scenes 
		if (scenes_enabled) scene_manager::__exit();

		if (world_enabled) world::__exit();

		// delete game
		if (s_game) {
			s_game->exit();
			delete s_game, s_game = nullptr;
		}

		// close jobs 
		if (jobs_enabled) job_queue::__exit();

		// close window
		delete s_window; s_window = nullptr;

	}

	void engine::quit() { s_shouldQuit = true; }

	const engine_settings* engine::get_engine_settings() {
		return s_engineSettings;
	}

	window* engine::get_window() {
		return s_window;
	}

	game* engine::get_game() {
		return s_game;
	}

	uint32 engine::get_target_framerate() {
		return s_targetFramerate;
	}

	void engine::set_target_framerate(uint32 framerate) {
		s_targetFramerate = framerate;
		if (s_targetFramerate == 0)
			s_frameLength = 0.0;
		else
			s_frameLength = 1.0 / static_cast<double>(s_targetFramerate);
	}

	void engine::handle_events() {
		keyboard::__clear_changed_bits();
		mouse::__clear_changed_bits();

		static SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_QUIT: quit(); break;						// close window and quit
				case SDL_KEYDOWN: case SDL_KEYUP:					// handle key 
				{
					// no need to check the key if it's repeating
					if (e.key.repeat) break;

					// letter on the keyboard
					if (e.key.keysym.sym > 96 && e.key.keysym.sym < 123)
						keyboard::__set_key(static_cast<keycode>(e.key.keysym.sym - 97), e.key.state);
					// number on keyboard
					else if (e.key.keysym.sym > 47 && e.key.keysym.sym < 58)
						keyboard::__set_key(static_cast<keycode>(e.key.keysym.sym - 48 + 26), e.key.state);
					// other
					else {
						switch (e.key.keysym.scancode) {
							case SDL_SCANCODE_TAB:       keyboard::__set_key(keycode::tab, e.key.state);         break;
							case SDL_SCANCODE_RETURN:	 keyboard::__set_key(keycode::ret, e.key.state);         break;
							case SDL_SCANCODE_BACKSPACE: keyboard::__set_key(keycode::backspace, e.key.state);   break;
							case SDL_SCANCODE_SPACE:     keyboard::__set_key(keycode::space, e.key.state);       break;
							case SDL_SCANCODE_ESCAPE:    keyboard::__set_key(keycode::escape, e.key.state);		 break;
							case SDL_SCANCODE_UP:        keyboard::__set_key(keycode::arrow_up, e.key.state);    break;
							case SDL_SCANCODE_DOWN:      keyboard::__set_key(keycode::arrow_down, e.key.state);  break;
							case SDL_SCANCODE_LEFT:      keyboard::__set_key(keycode::arrow_left, e.key.state);  break;
							case SDL_SCANCODE_RIGHT:     keyboard::__set_key(keycode::arrow_right, e.key.state); break;
							default: break;
						}
					}

					break; // SDL_KEYDOWN || SDL_KEYUP
				}
				case SDL_MOUSEMOTION:								// the mouse position
				{
					const glm::vec2 halfscreen(s_window->get_screen_size() * 0.5f);
					glm::vec2 position(e.motion.x, e.motion.y);
					position -= halfscreen;
					position /= halfscreen;
					position.y = -position.y;
					mouse::__set_position(position);
					break;
				}
				case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP:	// mouse buttons
				{
					switch (e.button.button) {
						case SDL_BUTTON_LEFT:	mouse::__set_button(0, e.button.state == SDL_PRESSED); break;
						case SDL_BUTTON_MIDDLE: mouse::__set_button(1, e.button.state == SDL_PRESSED); break;
						case SDL_BUTTON_RIGHT:	mouse::__set_button(2, e.button.state == SDL_PRESSED); break;
						case SDL_BUTTON_X1:		mouse::__set_button(3, e.button.state == SDL_PRESSED); break;
						case SDL_BUTTON_X2:		mouse::__set_button(4, e.button.state == SDL_PRESSED); break;
						default:break;
					}
					break;
				}
				case SDL_MOUSEWHEEL:								// mouse scroll wheel
				{
					mouse::__set_scroll(static_cast<float>(e.wheel.y)
										* (e.wheel.direction == SDL_MOUSEWHEEL_NORMAL ? 1.0f : -1.0f));
					break;
				}
				default: break;
			}
		}
	}

}