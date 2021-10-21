#ifndef ALC_OBJECTS_COMPONENT_INFO_HPP
#define ALC_OBJECTS_COMPONENT_INFO_HPP
#include "../common.hpp"

namespace alc {

	// determines if only one of a component of type can exist on an object
	template<typename ComponentTy>
	static constexpr bool component_only_one_v = false;

	// determines if only one of a behavior of type can exist on an object
	template<typename GroupBehaviorTy>
	static constexpr bool behavior_only_one_v = false;

}

// determines if only one of a component of type can exist on an object
// place in global namespace in header file
#define ALC_COMPONENT_ONLY_ONE(ComponentTy) \
template<> static constexpr bool alc::component_only_one_v<ComponentTy> = true;

// determines if only one of a behavior of type can exist on an object
// place in global namespace in header file
#define ALC_BEHAVIOR_ONLY_ONE(GroupBehaviorTy) \
template<> static constexpr bool alc::behavior_only_one_v<GroupBehaviorTy> = true;

#endif // !ALC_OBJECTS_COMPONENT_INFO_HPP