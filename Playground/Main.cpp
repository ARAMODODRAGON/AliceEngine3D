#include <iostream>
#include <alc\alc.hpp>
#include <alc\core\alice_events.hpp>
#include <alc\objects\singleton.hpp>

class TestSingletonType : public alc::singleton {
	ALC_SINGLETON_GETTER(TestSingletonType);
public:

	TestSingletonType() : m_a(10) {

	}
	~TestSingletonType() {

	}

	int GetA() { return m_a; }

private:

	int m_a;
};

class Playground : public alc::game {
public:

	void init() override {
		alc::scene_manager::load_scene(0);
		//alc::world::create("Test Object");
		ALC_DEBUG_LOG("Value is " + VTOS(TestSingletonType::get()->GetA()));
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

private:

	void on_init_scene(const std::string& args) override { }

	void on_destroy() override { }


};

static const alc::engine_settings GetSettings() {
	alc::engine_settings set;

	set.window.titlebar = "Playground";
	set.window.size = glm::uvec2(1280u, 720u);

	set.gameBinding = alc::bind_game<Playground>();

	set.objects.scenemanager.scenebindings = {
		alc::bind_scene<Level>("Level0")
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
