#include <iostream>
#include <alc\alc.hpp>
#include <alc\core\alice_events.hpp>
#include <alc\objects\object_factory.hpp>
#include <alc\reflection\type.hpp>
#include "TypeSystemDemo.hpp"

class Playground : public alc::game {
public:

	void init() override {

	}

	void exit() override {

	}

	void update(alc::timestep ts) override {

	}

	virtual void draw() override {

	}

};

class Level : public alc::scene {
public:

	Level() { }
	~Level() { }

private:

	void init(const std::string& args) override {

	}

	alc::object_factory m_factory;

};

const alc::engine_settings GetSettings() {
	alc::engine_settings set;

	set.window.titlebar = "Playground";
	set.window.size = glm::uvec2(1280u, 720u);

	set.gameBinding = alc::bind_game<Playground>();

	set.scenemanager.sceneBindings = {
		alc::bind_scene<Level>("Level0")
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
