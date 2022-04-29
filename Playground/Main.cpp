#include <alc\alc.hpp>
#include "Playground\Playground.hpp"

static const alc::engine_settings GetSettings() {
	alc::engine_settings set;

	set.window.titlebar = "Playground";
	set.window.size = glm::uvec2(1280u, 720u);

	set.gameBinding = alc::bind_game<Playground>();

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
