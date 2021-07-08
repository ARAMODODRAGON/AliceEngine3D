#ifndef ALC_CORE_SCENE_MANAGER_HPP
#define ALC_CORE_SCENE_MANAGER_HPP
#include "../common.hpp"

namespace alc {

	// interface for scenes
	class iscene {
	public:
		virtual ~iscene() = 0 { }
		// events
		virtual void init(const std::string& args) { }
		virtual void exit() { }
		virtual void update(float delta) { }
	};

	// binding for loading scenes
	struct scene_binding final {
		std::string name = nullptr;
		iscene* (*create)() = nullptr;
		std::string args = nullptr;
	};

	// create a binding for a scene
	template<typename sceneTy>
	scene_binding bind_scene(const std::string& name, const std::string& args = "");

	// static scene manager to hold the active scene, load new scenes, and manages multiple scenes at the same time
	class scene_manager final {
		ALC_STATIC_CLASS(scene_manager);
	public:

		// loads a scene into the primary slot
		static bool load_scene(size_t sceneBindingIndex);

		// loads a scene into the primary slot
		static bool load_scene(const std::string& sceneName);

		// loads a scene alongside the exiting scenes
		static bool load_scene_additive(size_t sceneBindingIndex);

		// loads a scene alongside the exiting scenes
		static bool load_scene_additive(const std::string& sceneName);

		// unloads and additive scene based off its index in the scene_manager
		static bool unload_scene(size_t activeSceneIndex);

		// returns the main scene
		static iscene* get_primary_scene();

		// returns the number of scenes
		static size_t active_scenes_size();

		// returns the scene at index, where 0 is the primary scene
		static iscene get_active_scene(size_t index);

		// returns the name of the scene at the index, where 0 is the primary scene
		static iscene get_active_scene_name(size_t index);

	private:

		using scene_pair = std::pair<std::unique_ptr<iscene>, scene_binding*>;
		using load_pair = std::pair<bool, scene_binding*>;
		static inline std::vector<scene_pair> m_activeScenes;
		static inline std::vector<load_pair> m_scenesToLoad;

	public:
		static void __update(float delta);
		static void __endframe();
	};


	// implementations

	template<typename sceneTy>
	inline scene_binding bind_scene(const std::string& name, const std::string& args) {
		scene_binding sb;
		sb.name = name;
		sb.create = []()-> iscene* { return new sceneTy(); };
		sb.args = args;
		return sb;

	}

}

#endif // !ALC_CORE_SCENE_MANAGER_HPP