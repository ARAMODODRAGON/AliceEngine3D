#ifndef ALC_CORE_ENGINE_HPP
#define ALC_CORE_ENGINE_HPP
#include "debug.hpp"
#include "scene_manager.hpp"
#include "game.hpp"
#include "window.hpp"

namespace alc {

	// data struct to hold the settings for the engine 
	struct engine_settings final {

		// basic initialization
		struct {
			uint32 targetFramerate = 0; // if 0 then the framerate becomes uncapped
		} general;

		// window initialization
		struct {
			std::string titlebar = "";
			glm::uvec2 size = glm::uvec2(0, 0);
		} window;

		// setup scenebindings -- optional
		// when no scenes are bound the scenemanager becomes disabled during the duration of the game's run
		struct {
			std::vector<scene_binding> sceneBindings;

			uint32 initialSceneIndex = -1; // optional (priority)
			std::string initialSceneStr = ""; // optional
		} scenemanager;

		// game class setup -- optional
		game_binding gameBinding = nullptr;

		// setup jobsystem -- optional
		struct {
			bool enabled = false; // must be enabled to use jobsystem
		} jobs;

	};

	// static engine class used to start and stop this engine instance
	class engine final {
		ALC_STATIC_CLASS(engine);
	public:

		// starts the engine using the given settings
		// engine will hold a pointer to the settings and read from it, must not modify the settings while the engine runs
		static void start(const engine_settings* set);

		// quits the current instance of the engine
		static void quit();

		// returns the engine_settings 
		static const engine_settings* get_engine_settings();

		// returns the window  
		static window* get_window();

		// returns the game  
		static game* get_game();

		// gets the target framerate
		static uint32 get_target_framerate();

		// sets the target framerate
		// uncapped if set to 0
		static void set_target_framerate(uint32 framerate);

	private:
		static inline const engine_settings* s_engineSettings = nullptr;
		static inline bool s_isRunning = false;
		static inline bool s_shouldQuit = false;
		static inline uint32 s_targetFramerate = 0;
		static inline double s_frameLength = 0.0;
		static inline window* s_window = nullptr;
		static inline game* s_game = nullptr;
	};

}

#endif // !ALC_CORE_ENGINE_HPP