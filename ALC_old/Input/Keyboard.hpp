/*
* MIT License
*
* Copyright (c) 2021 Domara Shlimon
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#ifndef ALC_INPUT_KEYBOARD_HPP
#define ALC_INPUT_KEYBOARD_HPP
#include "../General.hpp"
#include "Button.hpp"

namespace ALC {

	enum class KeyCode : int32 {
		KeyA,
		KeyB,
		KeyC,
		KeyD,
		KeyE,
		KeyF,
		KeyG,
		KeyH,
		KeyI,
		KeyJ,
		KeyK,
		KeyL,
		KeyM,
		KeyN,
		KeyO,
		KeyP,
		KeyQ,
		KeyR,
		KeyS,
		KeyT,
		KeyU,
		KeyV,
		KeyW,
		KeyX,
		KeyY,
		KeyZ,
		Num0,
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,
		LeftShift,
		RightShift,
		LeftCtrl,
		RightCtrl,
		LeftAlt,
		RightAlt,
		Return,
		Escape,
		Backspace,
		Tab,
		Space,
		ArrowUp,
		ArrowDown,
		ArrowLeft,
		ArrowRight,
		None,
		__SIZE__
	};
	constexpr size_t KEYCODE_COUNT = static_cast<size_t>(KeyCode::__SIZE__);

	class Keyboard {
		ALC_NON_CONSTRUCTABLE(Keyboard);
	public:

		/// getters
		static Button GetKey(KeyCode key);
		static string KeyToString(KeyCode key);
		static KeyCode GetLastKeypress() { return s_lastkey; }

	public:
		static void __ClearChangedBits();
		static void __SetKey(KeyCode key, bool value);
		struct KeyValue {
			bool isPressed : 1;
			bool hasChanged : 1;

			KeyValue() : isPressed(false), hasChanged(false) { }
		};
		static array<KeyValue, KEYCODE_COUNT> keyValues;
		static KeyCode s_lastkey;
	};

}

#endif // !ALC_INPUT_KEYBOARD_HPP