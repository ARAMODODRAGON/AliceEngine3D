#include <ALC/ALC.hpp>
#include "PlaygroundGame.hpp"
#include "Scenes\Scene0.hpp"

int main(int argc, char* argv[]) {

	ALC::EngineSettings set;

	set.window.size = ALC::uvec2(1280, 720);
	set.window.title = "AliceEngine3D";

	set.scenes.bindings = {
		ALC_BIND_SCENE(Scene0)
	};

	set.jobsystem.enable = false;

	ALC::SceneManager::Start<PlaygroundGame>(set);

	return 0;
}