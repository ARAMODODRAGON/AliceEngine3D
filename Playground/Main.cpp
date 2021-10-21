#include <iostream>
#include <alc\alc.hpp>
#include <alc\core\alice_events.hpp>

class TestComponent final : public alc::component {

	virtual void on_create() {
		set_should_update(true);
	}
	virtual void on_delete() { }
	virtual void on_update(alc::timestep ts) { }

};

class TestBehavior final : public alc::behavior {

	int a = 20;

	virtual void on_create() {
		set_should_update(true);
	}
	virtual void on_delete() { }
	virtual void on_update(alc::timestep ts) {
		if (a > 0) {
			get_group()->create_object<TestComponent>("Object_" + VTOS(a));
			a--;
		} else {
			ALC_DEBUG_LOG("Max objects");
		}
	}

};

class Playground : public alc::game {
public:

	void init() override {
		//alc::scene_manager::load_scene(0);
		alc::world::create_group<TestBehavior>();

	}

	void exit() override {

	}

	void update(alc::timestep ts) override {

	}


};

//class Level final : public alc::scene_group {
//public:
//
//	Level() { }
//	~Level() { }
//
//private:
//
//	void on_init_scene(const std::string& args) override { }
//
//	void on_delete() override { }
//
//
//};

static const alc::engine_settings GetSettings() {
	alc::engine_settings set;

	set.window.titlebar = "Playground";
	set.window.size = glm::uvec2(1280u, 720u);

	set.gameBinding = alc::bind_game<Playground>();

	//set.scenemanager.sceneBindings = {
	//	alc::bind_scene<Level>("Level0")
	//};

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
