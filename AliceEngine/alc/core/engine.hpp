#ifndef ALC_CORE_ENGINE_HPP
#define ALC_CORE_ENGINE_HPP
#include "../common.hpp"
#include "scene_manager.hpp"

namespace alc {

	// data struct to hold the settings for the engine 
	struct engine_settings final {

		// window initialization
		struct {
			glm::uvec2 size = glm::uvec2(0, 0);
			std::string titlebar = "";
		} window;

		// setup scenebindings -- optional
		// when no scenes are bound the scenemanager becomes disabled during the duration of the game's run
		struct {
			std::vector<scene_binding> sceneBindings;

			uint32 initialSceneVal = -1; // optional (priority)
			std::string initialSceneStr = nullptr; // optional
		} scenemanager;

		// game class setup -- optional
		//game_binding gameBinding = nullptr;

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
		static void start(const engine_settings* engineSettings);

		// quits the current instance of the engine
		static void quit();

	private:

		static inline const engine_settings* m_engineSettings = nullptr;

	public:
		static const engine_settings* __get_engine_settings();
	};

}

#endif // !ALC_CORE_ENGINE_HPP