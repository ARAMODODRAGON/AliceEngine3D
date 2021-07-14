#include <iostream>
#include <alc\alc.hpp>
#include <alc\core\alice_events.hpp>
#include <alc\objects\object_factory.hpp>
#include <alc\reflection\type.hpp>

class Playground : public alc::game {
public:

	void init() override {
		ALC_DEBUG_LOG("Created Playground");
	}

	void exit() override {
		ALC_DEBUG_LOG("Destroyed Playground");
	}

	void update(alc::timestep ts) override {
		if (alc::keyboard::get_key(alc::keycode::num_0).pressed()) {
			ALC_DEBUG_LOG("key pressed");
		}

		if (alc::mouse::get_left().pressed()) ALC_DEBUG_LOG("Left click");
		if (alc::mouse::get_middle().pressed()) ALC_DEBUG_LOG("Middle click");
		if (alc::mouse::get_right().pressed()) ALC_DEBUG_LOG("Right click");
		if (alc::mouse::get_fourth().pressed()) ALC_DEBUG_LOG("Fourth click");
		if (alc::mouse::get_fifth().pressed()) ALC_DEBUG_LOG("Fifth click");

		static glm::vec2 mousePos = glm::vec2(0.0f);
		if (mousePos != alc::mouse::get_position()) {
			mousePos = alc::mouse::get_position();
			ALC_DEBUG_LOG("Mouse Pos: " + VTOS(mousePos));
		}

		if (float scroll = alc::mouse::get_scroll(); scroll != 0.0f) {
			ALC_DEBUG_LOG("Mouse Scroll: " + VTOS(scroll));
		}

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


};

int main(int argc, char* argv[]) {

	alc::engine_settings set;
	
	set.window.titlebar = "Playground";
	set.window.size = glm::uvec2(1280u, 720u);
	
	set.gameBinding = alc::bind_game<Playground>();
	
	alc::engine::start(&set);

	return 0;
}