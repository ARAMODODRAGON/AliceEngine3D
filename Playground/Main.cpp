#include <iostream>
#include <alc\alc.hpp>
#include <alc\core\alice_events.hpp>

class Playground : public alc::game {
public:

	void init() override {
		ALC_DEBUG_LOG("Created Playground");
		alc::scene_manager::load_scene_additive("level 2");
		alc::scene_manager::load_scene_additive("level 3");
	}

	void exit() override {
		ALC_DEBUG_LOG("Destroyed Playground");
	}

	void update(alc::timestep ts) override {
		
	}

	virtual void draw() override {

	}

};

class Level : public alc::scene {
public:

	void init(const std::string& args) override { 
		if (args == "quit") {
			alc::engine::quit();
		}
	}

	void exit() override { }

	void update(alc::timestep ts) override { }

};

int main(int argc, char* argv[]) {

	alc::engine_settings set;

	set.window.titlebar = "Playground";
	set.window.size = glm::uvec2(1280u, 720u);

	set.gameBinding = alc::bind_game<Playground>();

	set.scenemanager.sceneBindings = {
		alc::bind_scene<Level>("level 1"),
		alc::bind_scene<Level>("level 2"),
		alc::bind_scene<Level>("level 3", "quit")
	};

	alc::engine::start(&set);

	return 0;
}