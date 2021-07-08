#include "engine.hpp"
#include "alice_events.hpp"
#include <chrono>

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

		// enable scene_manager
		const bool scenes_enabled = set->scenemanager.sceneBindings.size() > 0;
		if (scenes_enabled) scene_manager::__set_settings(set);

		// create game
		if (set->gameBinding){
			s_game = set->gameBinding();
			s_game->init();
		}

		// init and create scenes
		if (scenes_enabled) scene_manager::__init();

		// game loop //////////////////////////////////////////////////

		while (s_isRunning && !s_shouldQuit) {
			// get begining of frame and timestep
			lasttime = thistime;
			thistime = clock::now();
			timestep ts(duration(thistime - lasttime).count());

			// update
			if (s_game) s_game->update(ts);
			if (scenes_enabled) scene_manager::__update(ts);
			alice_events::onUpdate(ts);

			// TODO: render
			s_game->draw();
			if (scenes_enabled) scene_manager::__draw();

			// wait for end of frame
			if (s_targetFramerate != 0) {
				double length = duration(clock::now() - thistime).count();
				while (length < s_frameLength) std::this_thread::yield();
			}
		}

		// destroy ////////////////////////////////////////////////////

		// delete scenes 
		if (scenes_enabled) scene_manager::__exit();

		// delete game
		if (s_game) {
			s_game->exit();
			delete s_game, s_game = nullptr;
		}

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

}