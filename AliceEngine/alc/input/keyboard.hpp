#ifndef ALC_INPUT_KEYBOARD_HPP
#define ALC_INPUT_KEYBOARD_HPP
#include "../datatypes/button.hpp"
#include "../common.hpp"

namespace alc {

	enum class keycode : uint32 {
		key_a,
		key_b,
		key_c,
		key_d,
		key_e,
		key_f,
		key_g,
		key_h,
		key_i,
		key_j,
		key_k,
		key_l,
		key_m,
		key_n,
		key_o,
		key_p,
		key_q,
		key_r,
		key_s,
		key_t,
		key_u,
		key_v,
		key_w,
		key_x,
		key_y,
		key_z,
		num_0,
		num_1,
		num_2,
		num_3,
		num_4,
		num_5,
		num_6,
		num_7,
		num_8,
		num_9,
		left_shift,
		right_shift,
		left_ctrl,
		right_ctrl,
		left_alt,
		right_alt,
		enter,
		escape,
		ret, // return
		backspace,
		tab,
		space,
		arrow_up,
		arrow_down,
		arrow_left,
		arrow_right,
		__SIZE__
	};

	class keyboard {
		ALC_STATIC_CLASS(keyboard);
	public:

		static button get_key(keycode code);

	private:
		static constexpr size_t c_keycount = static_cast<size_t>(keycode::__SIZE__);
		struct keyvalue {
			bool held : 1;
			bool changed : 1;
			keyvalue() : held(false), changed(false) { }
		};
		static inline keyvalue s_keys[c_keycount];
	public:
		static void __clear_changed_bits();
		static void __set_key(keycode code, bool state);
	};

}

#endif // !ALC_INPUT_KEYBOARD_HPP