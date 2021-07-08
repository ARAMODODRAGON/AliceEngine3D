#include <iostream>
#include <alc\alc.hpp>
#include <alc\core\alice_events.hpp>

class Playground : public alc::game {
public:

	void init() override {
		ALC_DEBUG_LOG("Created Playground");
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

	void init(const std::string& args) override { }

	void exit() override { }

	void update(alc::timestep ts) override { }

};

int main(int argc, char* argv[]) {

	alc::engine_settings set;

	set.window.titlebar = "Playground";
	set.window.size = glm::uvec2(1280u, 720u);

	set.gameBinding = alc::bind_game<Playground>();

	set.scenemanager.sceneBindings = {
		alc::bind_scene<Level>("level 1")
	};

	alc::engine::start(&set);

	return 0;
}