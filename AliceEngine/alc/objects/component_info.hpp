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

	// flags that are used when alerting group of changes to its gameobject's components
	enum class component_alert_flags : uint8 {
		none = 0,
		created = (1 << 0),
		deleted = (1 << 1)
	};

	// flags that are used when alerting group of changes to its gameobject's components
	// default is component_alert_flags::none
	template<typename ComponentTy>
	static constexpr component_alert_flags component_alert_flags_v = component_alert_flags::none;

}


// determines if only one of a component of type can exist on an object
// place in global namespace in header file
#define ALC_COMPONENT_ONLY_ONE(ComponentTy) \
template<> static constexpr bool alc::component_only_one_v<ComponentTy> = true;

// determines if only one of a behavior of type can exist on an object
// place in global namespace in header file
#define ALC_BEHAVIOR_ONLY_ONE(GroupBehaviorTy) \
template<> static constexpr bool alc::behavior_only_one_v<GroupBehaviorTy> = true;

// defines flags for components that are used when alerting group of changes to the component
// place in global namespace in header file
#define ALC_COMPONENT_ALERT_FLAGS(ComponentTy, flags) \
template<> static constexpr component_alert_flags alc::component_alert_flags_v<ComponentTy> = flags;


#endif // !ALC_OBJECTS_COMPONENT_INFO_HPP