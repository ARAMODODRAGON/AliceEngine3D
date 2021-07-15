#include <iostream>
#include <alc\alc.hpp>
#include <alc\core\alice_events.hpp>
#include <alc\objects\object_factory.hpp>
#include <alc\reflection\type.hpp>
#include "TypeSystemDemo.hpp"

class Playground : public alc::game {
public:

	void init() override {
		ALC_DEBUG_LOG("Created Playground");
	}

	void exit() override {
		ALC_DEBUG_LOG("Destroyed Playground");
	}

	void update(alc::timestep ts) override {
		if (alc::keyboard::get_key(alc::keycode::key_a).pressed()) {
			alc::scene_manager::load_scene("Level1");
		}

		//if (alc::keyboard::get_key(alc::keycode::num_0).pressed()) {
		//	ALC_DEBUG_LOG("key pressed");
		//}
		//
		//if (alc::mouse::get_left().pressed()) ALC_DEBUG_LOG("Left click");
		//if (alc::mouse::get_middle().pressed()) ALC_DEBUG_LOG("Middle click");
		//if (alc::mouse::get_right().pressed()) ALC_DEBUG_LOG("Right click");
		//if (alc::mouse::get_fourth().pressed()) ALC_DEBUG_LOG("Fourth click");
		//if (alc::mouse::get_fifth().pressed()) ALC_DEBUG_LOG("Fifth click");
		//
		//static glm::vec2 mousePos = glm::vec2(0.0f);
		//if (mousePos != alc::mouse::get_position()) {
		//	mousePos = alc::mouse::get_position();
		//	ALC_DEBUG_LOG("Mouse Pos: " + VTOS(mousePos));
		//}
		//
		//if (float scroll = alc::mouse::get_scroll(); scroll != 0.0f) {
		//	ALC_DEBUG_LOG("Mouse Scroll: " + VTOS(scroll));
		//}
		//
	}

	virtual void draw() override {

	}

};

class Level : public alc::scene {
public:

	Level() : m_objectFilePath("") { }
	~Level() { }

private:

	void init(const std::string& args) override {

		if (size_t pos = args.find_first_of("object_file:"); pos != std::string::npos) {
			const char quote = '\'';

			size_t begin = args.find_first_of(quote, pos);
			if (begin == std::string::npos) {
				ALC_DEBUG_ERROR("args incorrect format (" + args + ")");
				return;
			}

			size_t end = args.find_first_of(quote, begin + 1);
			if (end == std::string::npos) {
				ALC_DEBUG_ERROR("args incorrect format (" + args + ")");
				return;
			}

			m_objectFilePath = std::string(&(args[begin + 1]), &(args[end]));
			ALC_DEBUG_LOG("Loaded file '" + m_objectFilePath + "'");
		}
		// failure
		else {
			ALC_DEBUG_ERROR("no object file could be loaded");
			return;
		}

	}

	std::string m_objectFilePath;

};

int main(int argc, char* argv[]) {

	alc::engine_settings set;

	set.window.titlebar = "Playground";
	set.window.size = glm::uvec2(1280u, 720u);

	set.gameBinding = alc::bind_game<Playground>();

	set.scenemanager.sceneBindings = {
		alc::bind_scene<Level>("Level0", "object_file:'demoFileA.lob'"),
		alc::bind_scene<Level>("Level1", "object_file:'demoFileB.lob'")
	};

	alc::engine::start(&set);

	return 0;
}