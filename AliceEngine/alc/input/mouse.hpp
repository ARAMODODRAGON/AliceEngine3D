#ifndef ALC_INPUT_MOUSE_HPP
#define ALC_INPUT_MOUSE_HPP
#include "../datatypes/button.hpp"
#include "../common.hpp"

namespace alc {

	class mouse {
		ALC_STATIC_CLASS(mouse);
	public:

		// gets the left button state
		static button get_left();

		// gets the middle button state
		static button get_middle();

		// gets the right button state
		static button get_right();

		// gets the fourth button state
		static button get_fourth();

		// gets the fifth button state
		static button get_fifth();

		// gets the position of the mouse in normalized coordinates
		static glm::vec2 get_position();

		// gets the normalized scroll direction
		static float get_scroll();

	private:
		static inline glm::vec2 s_position = glm::vec2(0.0f);
		static inline float s_scrollDir = 0.0f;
		struct buttonvalue {
			bool held : 1;
			bool changed : 1;
			buttonvalue() : held(false), changed(false) { }
		};
		static constexpr size_t c_button_count = 5;
		static inline buttonvalue s_buttons[c_button_count];
	public:
		static void __clear_changed_bits();
		static void __set_button(uint8 buttonum, bool state);
		static void __set_position(const glm::vec2& position);
		static void __set_scroll(float scroll);
	};

}

#endif // !ALC_INPUT_MOUSE_HPP