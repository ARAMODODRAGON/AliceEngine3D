#include <iostream>
#include <alc\alc.hpp>

class Scene0 : public alc::iscene {

	void init(const std::string& args) override {

	}

	void exit() override {

	}

	void update(float delta) override {

	}

};

int main(int argc, char* argv[]) {

	alc::engine_settings set;

	set.window.titlebar = "suck my cornk";
	set.window.size = glm::uvec2(1280u, 720u);

	set.scenemanager.sceneBindings = {
		alc::bind_scene<Scene0>("Scene0")
	};
	set.scenemanager.initialSceneVal = 0;

	alc::engine::start(&set);

	return 0;
}