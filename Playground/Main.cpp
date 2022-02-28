#include <iostream>
#include <alc\alc.hpp>
#include <alc\core\alice_events.hpp>
#include <alc\objects\singleton.hpp>
#include <alc\jobs\job_queue.hpp>
#include <alc\content\model.hpp>

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

class Level final : public alc::scene {
public:

	Level() { }
	~Level() { }

	void on_create() override {

	}

private:

	void on_init_scene(const std::string& args) override { }

	void on_destroy_scene() override { }


};

static const alc::engine_settings GetSettings() {
	alc::engine_settings set;

	set.window.titlebar = "Playground";
	set.window.size = glm::uvec2(1280u, 720u);

	set.gameBinding = alc::bind_game<Playground>();

	set.objects.scenemanager.scenebindings = {
		alc::bind_scene<Level>("Level0")
	};

	set.renderer2d.layerInfo = {
		alc::bind_layer("Layer 0"),
		alc::bind_layer("Layer 1"),
		alc::bind_layer("Layer 2")
	};

	//set.jobs.enabled = true;
	//set.jobs.maxthreads = 4;

	return set;
}

int main(int argc, char* argv[]) {

	// get settings
	const alc::engine_settings set = GetSettings();

	// start the engine
	alc::engine::start(&set);

	return 0;
}
