#include "scene_manager.hpp"
#include "debug.hpp"
#include "engine.hpp"

namespace alc {

	scene::~scene() { }

	size_t scene::get_index() const {
		return m_index;
	}

	std::string scene::get_name() const {
		return m_name;
	}

	void scene::__set_index(size_t index) {
		m_index = index;
	}

	void scene::__set_name(const std::string& name) {
		m_name = name;
	}

	bool scene_manager::load_scene(size_t sceneBindingIndex) {
		if (!s_eSettings) {
			ALC_DEBUG_WARNING("scene_manager is disabled");
			return false;
		}

		// check if valid index
		if (sceneBindingIndex >= s_eSettings->scenemanager.sceneBindings.size()) {
			ALC_DEBUG_WARNING("No scenebinding at index " + sceneBindingIndex);
			return false;
		}

		// set
		s_primarySceneToLoad = &s_eSettings->scenemanager.sceneBindings[sceneBindingIndex];
		return true;
	}

	bool scene_manager::load_scene(const std::string& sceneName) {
		if (!s_eSettings) {
			ALC_DEBUG_WARNING("scene_manager is disabled");
			return false;
		}

		// find scene with name
		for (size_t i = 0; i < s_eSettings->scenemanager.sceneBindings.size(); i++) {
			if (s_eSettings->scenemanager.sceneBindings[i].name == sceneName) {
				s_primarySceneToLoad = &s_eSettings->scenemanager.sceneBindings[i];
				return true;
			}
		}

		// no scene with name
		ALC_DEBUG_WARNING("No scenebinding with name " + sceneName);
		return false;
	}

	bool scene_manager::load_scene_additive(size_t sceneBindingIndex) {
		if (!s_eSettings) {
			ALC_DEBUG_WARNING("scene_manager is disabled");
			return false;
		}

		// check if valid index
		if (sceneBindingIndex >= s_eSettings->scenemanager.sceneBindings.size()) {
			ALC_DEBUG_WARNING("No scenebinding at index " + sceneBindingIndex);
			return false;
		}

		// set
		s_scenesToLoad.push_back(&s_eSettings->scenemanager.sceneBindings[sceneBindingIndex]);
		return true;
	}

	bool scene_manager::load_scene_additive(const std::string& sceneName) {
		if (!s_eSettings) {
			ALC_DEBUG_WARNING("scene_manager is disabled");
			return false;
		}

		// find scene with name
		for (size_t i = 0; i < s_eSettings->scenemanager.sceneBindings.size(); i++) {
			if (s_eSettings->scenemanager.sceneBindings[i].name == sceneName) {
				s_scenesToLoad.push_back(&s_eSettings->scenemanager.sceneBindings[i]);
				return true;
			}
		}

		// no scene with name
		ALC_DEBUG_WARNING("No scenebinding with name " + sceneName);
		return false;
	}

	bool scene_manager::unload_scene(size_t activeSceneIndex) {
		if (!s_eSettings) {
			ALC_DEBUG_WARNING("scene_manager is disabled");
			return false;
		}

		// check for valid index
		if (activeSceneIndex == 0 || activeSceneIndex > s_activeScenes.size()) {
			ALC_DEBUG_WARNING("Could not unload scene because it was not a valid index or the primary scene was selected");
			return false;
		}

		// mark to unload
		s_checkForSceneChanges = true;
		s_activeScenes[activeSceneIndex].shouldDestroy = true;
		return true;
	}

	scene* scene_manager::get_primary_scene() {
		if (!s_eSettings) {
			ALC_DEBUG_WARNING("scene_manager is disabled");
			return nullptr;
		}
		return s_activeScenes[0].scene.get();
	}

	size_t scene_manager::active_scenes_size() {
		return s_activeScenes.size();
	}

	scene* scene_manager::get_active_scene(size_t index) {
		if (!s_eSettings) {
			ALC_DEBUG_WARNING("scene_manager is disabled");
			return nullptr;
		}
		return s_activeScenes[index].scene.get();
	}

	std::string scene_manager::get_active_scene_name(size_t index) {
		if (!s_eSettings) {
			ALC_DEBUG_WARNING("scene_manager is disabled");
			return nullptr;
		}
		return s_activeScenes[index].binding->name;
	}

	void scene_manager::handle_scenes() {
		// load primary scene
		if (s_primarySceneToLoad) {
			// destroy old scene
			if (s_activeScenes.size() > 0) {
				s_activeScenes[0].scene->exit();
				s_activeScenes[0].scene.reset();
				ALC_DEBUG_LOG("Closed scene " + s_activeScenes[0].binding->name);
			}
			// no scenes, create empty first spot
			else {
				s_activeScenes.emplace_back(nullptr, nullptr);
			}
			// create scene
			scene* firstscene = s_primarySceneToLoad->create();
			firstscene->__set_index(0);
			firstscene->__set_name(s_primarySceneToLoad->name);
			//s_activeScenes[0] = active_scene(firstscene, s_primarySceneToLoad);
			s_activeScenes[0].scene.reset(firstscene);
			s_activeScenes[0].binding = s_primarySceneToLoad;
			s_activeScenes[0].shouldDestroy = false;
			firstscene->init(s_primarySceneToLoad->args);
			ALC_DEBUG_LOG("Created scene " + s_activeScenes[0].binding->name);
			s_primarySceneToLoad = nullptr;
		}

		// load / unload additive scenes

		// unload
		if (s_checkForSceneChanges) {
			s_checkForSceneChanges = false;
			for (auto it = s_activeScenes.begin(); it != s_activeScenes.end(); ++it) {
				if (it->shouldDestroy) {
					it->scene->exit();
					it->scene.reset();
					ALC_DEBUG_LOG("Closed scene " + it->binding->name);
					it = s_activeScenes.erase(it);
				}
			}
		}

		// load
		if (s_scenesToLoad.size() > 0) {
			for (size_t i = 0; i < s_scenesToLoad.size(); i++) {
				scene* scene = s_scenesToLoad[i]->create();
				scene->__set_index(s_activeScenes.size());
				scene->__set_name(s_scenesToLoad[i]->name);
				s_activeScenes.emplace_back(scene, s_scenesToLoad[i]);
				scene->init(s_scenesToLoad[i]->args);
				ALC_DEBUG_LOG("Created scene " + s_scenesToLoad[i]->name);
			}
			s_scenesToLoad.clear();
		}

	}

	void scene_manager::__set_settings(const engine_settings* set) {
		s_eSettings = set;
	}

	void scene_manager::__init() {
		const engine_settings* set = s_eSettings;

		// load by index
		if (s_primarySceneToLoad == nullptr && set->scenemanager.initialSceneIndex != -1 &&
			set->scenemanager.initialSceneIndex < set->scenemanager.sceneBindings.size()) {
			s_primarySceneToLoad = &set->scenemanager.sceneBindings[set->scenemanager.initialSceneIndex];
		}
		// load by name
		if (s_primarySceneToLoad == nullptr && set->scenemanager.initialSceneStr != "") {
			for (size_t i = 0; i < set->scenemanager.sceneBindings.size(); i++) {
				if (set->scenemanager.sceneBindings[i].name == set->scenemanager.initialSceneStr) {
					s_primarySceneToLoad = &set->scenemanager.sceneBindings[i];
				}
			}
		}
		// load first
		if (s_primarySceneToLoad == nullptr) s_primarySceneToLoad = &set->scenemanager.sceneBindings[0];

		// load scene
		handle_scenes();
	}

	void scene_manager::__exit() {
		// destroy all scenes
		for (size_t i = 0; i < s_activeScenes.size(); i++) {
			s_activeScenes[i].scene->exit();
			s_activeScenes[i].scene.reset();
			ALC_DEBUG_LOG("Closed scene " + s_activeScenes[i].binding->name);
		}
		s_activeScenes.clear();

		// remove pointers
		s_primarySceneToLoad = nullptr;
		s_scenesToLoad.clear();
		s_eSettings = nullptr;
		s_checkForSceneChanges = false;
	}

	void scene_manager::__update(timestep ts) {
		// load/unload scenes
		handle_scenes();

		// update scenes
		for (size_t i = 0; i < s_activeScenes.size(); i++) {
			s_activeScenes[i].scene->update(ts);
		}
	}

	//void scene_manager::__draw() {
	//	// draw scenes
	//	for (size_t i = 0; i < s_activeScenes.size(); i++) {
	//		s_activeScenes[i].scene->draw();
	//	}
	//}

	scene_manager::active_scene::active_scene(alc::scene* scene_, const scene_binding* binding_)
		: scene(scene_), shouldDestroy(false), binding(binding_) { }

}