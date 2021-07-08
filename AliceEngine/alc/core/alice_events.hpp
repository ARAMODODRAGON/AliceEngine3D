#ifndef ALC_CORE_ALICE_EVENTS_HPP
#define ALC_CORE_ALICE_EVENTS_HPP
#include "../common.hpp"
#include "../datatypes/function.hpp"

namespace alc {

	// static class holding all the events
	class alice_events final {
		ALC_STATIC_CLASS(alice_events);
	public:

		// basic update callback
		static inline event<void, float> onUpdate;

	};

}

#endif // !ALC_CORE_ALICE_EVENTS_HPP