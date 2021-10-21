#include "scene_manager.hpp"
#include "debug.hpp"
#include "engine.hpp"
#include "alice_events.hpp"

namespace alc {

	scene_group::~scene_group() { }

	size_t scene_group::get_index() const {
		return m_index;
	}

	void scene_group::__set_index(size_t index) {
		m_index = index;
	}

	bool scene_manager::load_scene(size_t sceneBindingIndex) {
		if (!s_eSettings) {
			ALC_DEBUG_WARNING("scene_manager is disabled");
			return false;
		}
		auto& bindings = s_eSettings->scenemanager.sceneBindings;

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
		auto& bindings = s_eSettings->scenemanager.sceneBindings;

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
		auto& bindings = s_eSettings->scenemanager.sceneBindings;

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
		auto& bindings = s_eSettings->scenemanager.sceneBindings;

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

	scene_group* scene_manager::get_primary_scene() {
		if (!s_eSettings) {
			ALC_DEBUG_WARNING("scene_manager is disabled");
			return nullptr;
		}
		return s_activeScenes[0].scene;
	}

	size_t scene_manager::active_scenes_size() {
		return s_activeScenes.size();
	}

	scene_group* scene_manager::get_active_scene(size_t index) {
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
		scene_group* scene = create_scene(sceneBindingIndex, index);

		// delete any previously existing scene if first
		if (firstslot) {
			if (s_activeScenes[0].scene) delete_scene(0);
		}
		// find index / add new index
		else {
			for (index = 0; index < s_activeScenes.size(); index++) {
				if (s_activeScenes[index].scene != nullptr) {
					break;
				}
			}
			if (index == s_activeScenes.size()) {
				s_activeScenes.emplace_back();
			}
		}

		auto* binding = &(s_eSettings->scenemanager.sceneBindings[sceneBindingIndex]);

		// set new scene 
		s_activeScenes[index].shouldDestroy = false;
		s_activeScenes[index].binding = binding;
		delete_scene(index);
		s_activeScenes[index].scene = scene;

		// invoke event and init only after setting it to the index
		alice_events::onSceneLoad(scene, index);
		scene->init_scene(binding->args);
	}

	scene_group* scene_manager::create_scene(size_t sceneBindingIndex, size_t index) {
		auto& binding = s_eSettings->scenemanager.sceneBindings[sceneBindingIndex];
		scene_group* scene = binding.create();
		scene->set_name(binding.name);
		scene->__set_index(index);
		scene->get_group()->__set_scene_group(scene); // now is treated as a scene group
		return scene;
	}

	void scene_manager::delete_scene(size_t index) {
		alice_events::onSceneUnload(s_activeScenes[index].scene, index);
		s_activeScenes[index].scene->delete_this();
		s_activeScenes[index].scene = nullptr;
	}

	void scene_manager::__init(const engine_settings* set) {
		if (set->scenemanager.sceneBindings.size() == 0) {
			ALC_DEBUG_ERROR("No scenes are bound. Cannot initialize scene manager");
			return;
		}

		s_eSettings = set;

		// start listening to update
		alice_events::onUpdate += scene_manager::__update;

		// initialize
		s_activeScenes.clear();
		s_scenesToLoad.clear();
		s_checkForSceneChanges = false;

		// check if a scene needs to be loaded initially

		// load by index
		if (set->scenemanager.initialSceneIndex != -1) {
			load_scene(set->scenemanager.initialSceneIndex);
		}
		// load by name
		else if (set->scenemanager.initialSceneStr != "") {
			load_scene(set->scenemanager.initialSceneStr);
		}
		// else do not load anything

		// load scene
		handle_scenes();
	}

	void scene_manager::__exit() {
		// was not initialized correctly, no need to uninitialize
		if (s_eSettings->scenemanager.sceneBindings.size() == 0) {
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

		// stop listening to update
		alice_events::onUpdate -= scene_manager::__update;

	}

	void scene_manager::__update(timestep ts) {
		// load/unload scenes
		if (s_checkForSceneChanges) handle_scenes();

		// no need to update scene_groups since they're updated by the world
		//// update scenes
		//for (size_t i = 0; i < s_activeScenes.size(); i++) {
		//	if (s_activeScenes[i].scene != nullptr)
		//		s_activeScenes[i].scene->update(ts);
		//}
	}

	scene_manager::active_scene::active_scene(alc::scene_group* scene_, const scene_binding* binding_)
		: scene(scene_), shouldDestroy(false), binding(binding_) { }

}