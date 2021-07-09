#include "keyboard.hpp"

namespace alc {

	button keyboard::get_key(keycode code) {
		auto& key = s_keys[static_cast<uint32>(code)];
		return button(key.held, key.changed);
	}

	void keyboard::__clear_changed_bits() { 
		for (size_t i = 0; i < c_keycount; i++) {
			s_keys[i].changed = false;
		}
	}

	void keyboard::__set_key(keycode code, bool state) { 
		auto& key = s_keys[static_cast<uint32>(code)];
		key.changed = (key.held != state);
		key.held = state;

		//if (get_key(code).pressed())
		//	ALC_DEBUG_LOG("pressed key");
	}

}
