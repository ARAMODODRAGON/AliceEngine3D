#ifndef ALC_CORE_ALICE_EVENTS_HPP
#define ALC_CORE_ALICE_EVENTS_HPP
#include "../common.hpp"
#include "../datatypes/function.hpp"
#include "../datatypes/timestep.hpp"

namespace alc {

	// static class holding all the events
	class alice_events final {
		ALC_STATIC_CLASS(alice_events);
	public:

		// basic update callback
		static inline event<void, timestep> onUpdate;

		// post update callback
		static inline event<void, timestep> onPostUpdate;

		// when loading/unloading scenes
		// second parameter indicates the index of of this scene (0 would indicate that it is the primary scene)
		static inline event<void, class scene*, uint32> onSceneUnload;

		// second parameter indicates the index of of this scene (0 would indicate that it is the primary scene)
		static inline event<void, class scene*, uint32> onSceneLoad;

	};

}

#endif // !ALC_CORE_ALICE_EVENTS_HPP