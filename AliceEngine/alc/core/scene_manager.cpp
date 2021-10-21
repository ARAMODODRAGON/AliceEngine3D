#include "scene_manager.hpp"
#include "debug.hpp"
#include "engine.hpp"
#include "alice_events.hpp"

namespace alc {

	scene::~scene() { scene_manager::__remove_scene(this); }

	bool scene_manager::load_scene(size_t sceneBindingIndex) {
		if (!s_eSettings) {
			ALC_DEBUG_WARNING("scene_manager is disabled");
			return false;
		}
		auto& bindings = s_eSettings->objects.scenemanager.scenebindings;

		// check valid index
		if (bindings.size() <= sceneBindingIndex) {
			ALC_DEBUG_ERROR("Cannot load scene with index of " + VTOS(sceneBindingIndex));
			return false;
		}

		// add scene to load
		s_checkForSceneChanges = true;
		s_scenesToLoad.emplace_back(true, sceneBindingIndex);
		return true;
	}

	bool scene_manager::load_scene(const std::string& sceneName) {
		if (!s_eSettings) {
			ALC_DEBUG_WARNING("scene_manager is disabled");
			return false;
		}
		auto& bindings = s_eSettings->objects.scenemanager.scenebindings;

		// check valid index
		size_t sceneBindingIndex = -1;

		for (size_t i = 0; i < bindings.size(); i++) {
			if (bindings[i].name == sceneName) {
				sceneBindingIndex = i;
				break;
			}
		}

		if (bindings.size() <= sceneBindingIndex) {
			ALC_DEBUG_ERROR("Cannot load scene with name of " + sceneName);
			return false;
		}

		// add scene to load
		s_checkForSceneChanges = true;
		s_scenesToLoad.emplace_back(true, sceneBindingIndex);
		return true;
	}

	bool scene_manager::load_scene_additive(size_t sceneBindingIndex) {
		if (!s_eSettings) {
			ALC_DEBUG_WARNING("scene_manager is disabled");
			return false;
		}
		auto& bindings = s_eSettings->objects.scenemanager.scenebindings;

		// check valid index
		if (bindings.size() <= sceneBindingIndex) {
			ALC_DEBUG_ERROR("Cannot load scene with index of " + VTOS(sceneBindingIndex));
			return false;
		}

		// add scene to load
		s_checkForSceneChanges = true;
		s_scenesToLoad.emplace_back(false, sceneBindingIndex); // false indicates additive scene
		return true;
	}

	bool scene_manager::load_scene_additive(const std::string& sceneName) {
		if (!s_eSettings) {
			ALC_DEBUG_WARNING("scene_manager is disabled");
			return false;
		}
		auto& bindings = s_eSettings->objects.scenemanager.scenebindings;

		// check valid index
		size_t sceneBindingIndex = -1;

		for (size_t i = 0; i < bindings.size(); i++) {
			if (bindings[i].name == sceneName) {
				sceneBindingIndex = i;
				break;
			}
		}

		if (bindings.size() <= sceneBindingIndex) {
			ALC_DEBUG_ERROR("Cannot load scene with name of " + sceneName);
			return false;
		}

		// add scene to load
		s_checkForSceneChanges = true;
		s_scenesToLoad.emplace_back(false, sceneBindingIndex); // false indicates additive scene
		return true;
	}

	bool scene_manager::unload_scene(size_t activeSceneIndex) {
		if (!s_eSettings) {
			ALC_DEBUG_WARNING("scene_manager is disabled");
			return false;
		}

		// check if valid
		if (activeSceneIndex >= s_activeScenes.size()) {
			ALC_DEBUG_ERROR("Could not unload scene, invalid index");
			return false;
		}
		// check if scene exists
		else if (s_activeScenes[activeSceneIndex].scene == nullptr) {
			ALC_DEBUG_ERROR("Could not unload scene, scene already unloaded");
			return false;
		}

		// find scene and mark to unload
		s_checkForSceneChanges = true;
		s_activeScenes[activeSceneIndex].shouldDestroy = true;

		return true;
	}

	scene* scene_manager::get_primary_scene() {
		if (!s_eSettings) {
			ALC_DEBUG_WARNING("scene_manager is disabled");
			return nullptr;
		}
		return s_activeScenes[0].scene;
	}

	size_t scene_manager::active_scenes_size() {
		return s_activeScenes.size();
	}

	scene* scene_manager::get_active_scene(size_t index) {
		if (!s_eSettings) {
			ALC_DEBUG_WARNING("scene_manager is disabled");
			return nullptr;
		}
		return s_activeScenes[index].scene;
	}

	std::string scene_manager::get_active_scene_name(size_t index) {
		if (!s_eSettings) {
			ALC_DEBUG_WARNING("scene_manager is disabled");
			return nullptr;
		}
		return s_activeScenes[index].binding->name;
	}

	void scene_manager::handle_scenes() {
		s_checkForSceneChanges = false;

		// check if any scenes need to be loaded
		if (s_scenesToLoad.size() > 0) {
			while (s_scenesToLoad.size() > 0) {
				// get binding
				auto [firstslot, binding] = s_scenesToLoad.front();

				// load
				do_load_scene(firstslot, binding);

				// erase position
				s_scenesToLoad.erase(s_scenesToLoad.begin());
			}
		}

		// check if any need to be deleted
		for (size_t i = 0; i < s_activeScenes.size(); i++) {
			if (s_activeScenes[i].shouldDestroy) {
				if (s_activeScenes[i].scene != nullptr) delete_scene(i);
				s_activeScenes[i].shouldDestroy = false;
				s_activeScenes[i].binding = nullptr;
			}
		}
	}

	void scene_manager::do_load_scene(bool firstslot, size_t sceneBindingIndex) {
		if (s_activeScenes.size() == 0) { s_activeScenes.emplace_back(); }

		// create
		size_t index = (firstslot ? 0 : s_activeScenes.size());
		scene* scene = create_scene(sceneBindingIndex);

		// delete any previously existing scene if first
		if (firstslot) {
			if (s_activeScenes[0].scene) delete_scene(0);
		}
		// add new index
		else {
			s_activeScenes.emplace_back();
		}

		auto* binding = &(s_eSettings->objects.scenemanager.scenebindings[sceneBindingIndex]);

		// set new scene 
		s_activeScenes[index].shouldDestroy = false;
		s_activeScenes[index].binding = binding;
		delete_scene(index);
		s_activeScenes[index].scene = scene;

		// invoke event and init only after setting it to the index
		alice_events::onSceneLoad(scene, index);
		scene->on_init_scene(binding->args);
	}

	scene* scene_manager::create_scene(size_t sceneBindingIndex) {
		auto& binding = s_eSettings->objects.scenemanager.scenebindings[sceneBindingIndex];
		scene* scene = binding.create(binding.name);
		return scene;
	}

	void scene_manager::delete_scene(size_t index) {
		alice_events::onSceneUnload(s_activeScenes[index].scene, index);
		s_activeScenes[index].scene->delete_this();
		if (index != 0) s_activeScenes.erase(s_activeScenes.begin() + index);
	}

	void scene_manager::__init(const engine_settings* set) {
		if (set->objects.scenemanager.scenebindings.size() == 0) {
			ALC_DEBUG_ERROR("No scenes are bound. Cannot initialize scene manager");
			return;
		}

		s_eSettings = set;

		// initialize
		s_activeScenes.clear();
		s_scenesToLoad.clear();
		s_checkForSceneChanges = false;

		// check if a scene needs to be loaded initially

		// load by index
		if (set->objects.scenemanager.initialSceneIndex != -1) {
			load_scene(set->objects.scenemanager.initialSceneIndex);
		}
		// load by name
		else if (set->objects.scenemanager.initialSceneStr != "") {
			load_scene(set->objects.scenemanager.initialSceneStr);
		}
		// else do not load anything

		// load scene
		handle_scenes();
	}

	void scene_manager::__exit() {
		// was not initialized correctly, no need to uninitialize
		if (s_eSettings->objects.scenemanager.scenebindings.size() == 0) {
			return;
		}

		// remove all scenes to load
		s_scenesToLoad.clear();

		// mark all scenes to be destroyed
		for (size_t i = 0; i < s_activeScenes.size(); i++) {
			s_activeScenes[i].shouldDestroy = true;
		}
		// will destroy all scenes marked as shouldDestroy
		handle_scenes();
		s_activeScenes.clear();

	}

	void scene_manager::__update(timestep ts) {
		// load/unload scenes
		if (s_checkForSceneChanges) handle_scenes();
	}

	void scene_manager::__remove_scene(scene* s) {
		for (size_t i = 0; i < s_activeScenes.size(); i++) {
			if (s_activeScenes[i].scene == s) {
				s_activeScenes.erase(s_activeScenes.begin() + i);
				return;
			}
		}
	}

	scene_manager::active_scene::active_scene(alc::scene* scene_, const scene_binding* binding_)
		: scene(scene_), shouldDestroy(false), binding(binding_) { }

}