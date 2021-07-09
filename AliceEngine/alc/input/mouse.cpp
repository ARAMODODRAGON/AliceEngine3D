#include "mouse.hpp"

namespace alc {

	void mouse::__clear_changed_bits() {
		for (size_t i = 0; i < c_button_count; i++) {
			s_buttons[i].changed = false;
		}
		s_scrollDir = 0.0f;
	}

	void mouse::__set_button(uint8 buttonum, bool state) {
		s_buttons[buttonum].changed = (s_buttons[buttonum].held != state);
		s_buttons[buttonum].held = state;
	}

	void mouse::__set_position(const glm::vec2& position) {
		s_position = position;
	}

	void mouse::__set_scroll(float scroll) {
		s_scrollDir = scroll;
	}

	button mouse::get_left() {
		return button(s_buttons[0].held, s_buttons[0].changed);
	}

	button mouse::get_middle() {
		return button(s_buttons[1].held, s_buttons[1].changed);
	}

	button mouse::get_right() {
		return button(s_buttons[2].held, s_buttons[2].changed);
	}

	button mouse::get_fourth() {
		return button(s_buttons[3].held, s_buttons[3].changed);
	}

	button mouse::get_fifth() {
		return button(s_buttons[4].held, s_buttons[4].changed);
	}

	glm::vec2 mouse::get_position() {
		return s_position;
	}

	float mouse::get_scroll() {
		return s_scrollDir;
	}

}