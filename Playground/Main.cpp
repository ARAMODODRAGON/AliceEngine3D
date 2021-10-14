#include <iostream>
#include <alc\alc.hpp>
#include <alc\core\alice_events.hpp>
#include <alc\objects\object_factory.hpp>

class Playground : public alc::game {
public:

	void init() override {
		alc::scene_manager::load_scene(0);
	}

	void exit() override {

	}

	void update(alc::timestep ts) override {

	}


};

static size_t index = 0;

class Level : public alc::scene {
public:

	Level() { }
	~Level() { }

private:

	void init(const std::string& args) override {
		if (get_index() > 0) return;

		ALC_DEBUG_LOG("Loaded scene " + get_name());
		alc::scene_manager::load_scene(++index);

		if (args.size() > 0) {
			alc::scene_manager::load_scene_additive(args);
		}
	}

	void exit() override { }

	alc::object_factory m_factory;

};

static const alc::engine_settings GetSettings(bool debug) {
	alc::engine_settings set;

	set.window.titlebar = "Playground";
	set.window.size = glm::uvec2(1280u, 720u);

	set.gameBinding = alc::bind_game<Playground>();

	set.scenemanager.sceneBindings = {
		alc::bind_scene<Level>("Level0"),
		alc::bind_scene<Level>("Level1", "Level1_grass"),
		alc::bind_scene<Level>("Level2"),
		alc::bind_scene<Level>("Level1_grass")
	};

	set.jobs.enabled = true;
	set.jobs.maxthreads = 4;

	return set;
}

int main(int argc, char* argv[]) {

	// get settings
	const alc::engine_settings set = GetSettings();

	// start the engine
	alc::engine::start(&set);

	return 0;
}
