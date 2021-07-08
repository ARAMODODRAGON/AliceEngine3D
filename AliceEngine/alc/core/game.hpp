#ifndef ALC_CORE_GAME_HPP
#define ALC_CORE_GAME_HPP
#include "../datatypes/timestep.hpp"

namespace alc {

	// a class that exists during the duration of the game, accessable through the engine
	class game {
	public:
		virtual ~game() = 0 { }

		virtual void init() = 0;
		virtual void exit() = 0;
		virtual void update(timestep ts) = 0;
		virtual void draw() = 0;
	};

	using game_binding = game * (*)();

	// binding a game class 
	template<typename Ty>
	inline game_binding bind_game() {
		return []()-> game* { return new Ty(); };
	}

}

#endif // !ALC_CORE_GAME_HPP