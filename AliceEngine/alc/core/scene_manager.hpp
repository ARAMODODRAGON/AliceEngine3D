#ifndef ALC_CORE_SCENE_MANAGER_HPP
#define ALC_CORE_SCENE_MANAGER_HPP
#include "../common.hpp"
#include "../datatypes/timestep.hpp"

namespace alc {

	// interface for scenes
	class scene {
	public:
		virtual ~scene() = 0;
		// events
		virtual void init(const std::string& args) { }
		virtual void exit() { }
		virtual void update(timestep ts) { }

		// returns the index of this scene in the scene_manager
		size_t get_index() const;

		// returns the name of this instance of this scene
		std::string get_name() const;

	private:
		size_t m_index;
		std::string m_name;
	public:
		void __set_index(size_t index);
		void __set_name(const std::string& name);
	};

	// binding for loading scenes
	struct scene_binding final {
		std::string name = "";
		scene* (*create)() = nullptr;
		std::string args = "";
	};

	// create a binding for a scene
	template<typename Ty>
	scene_binding bind_scene(const std::string& name, const std::string& args = "");

	struct engine_settings;

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
		static scene* get_primary_scene();

		// returns the number of scenes
		static size_t active_scenes_size();

		// returns the scene at index, where 0 is the primary scene
		static scene* get_active_scene(size_t index);

		// returns the name of the scene at the index, where 0 is the primary scene
		static std::string get_active_scene_name(size_t index);

	private:

		struct active_scene {
			std::unique_ptr<scene> scene;
			bool shouldDestroy;
			const scene_binding* binding;
			active_scene() = default;
			active_scene(alc::scene* scene, const scene_binding* binding);
		};
		static inline std::vector<active_scene> s_activeScenes;
		static inline std::list<std::pair<bool, size_t>> s_scenesToLoad;
		static inline const engine_settings* s_eSettings = nullptr;
		static inline bool s_checkForSceneChanges = false;

		static void handle_scenes();
		static void do_load_scene(bool firstslot, size_t sceneBindingIndex = -1);
		static scene* create_scene(size_t sceneBindingIndex, size_t index);
		static void delete_scene(size_t index);

	public:
		static void __init(const engine_settings* set);
		static void __exit();
		static void __update(timestep ts);
	};


	// implementations

	template<typename Ty>
	inline scene_binding bind_scene(const std::string& name, const std::string& args) {
		scene_binding sb;
		sb.name = name;
		sb.create = []()-> scene* { return new Ty(); };
		sb.args = args;
		return sb;

	}

}

#endif // !ALC_CORE_SCENE_MANAGER_HPP