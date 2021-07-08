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
#include "Keyboard.hpp"

namespace ALC {

	array<Keyboard::KeyValue, KEYCODE_COUNT> Keyboard::keyValues;
	KeyCode Keyboard::s_lastkey = KeyCode::None;

	// returns a uint index for accessing keys in the array
	#define KEY_INDEX(key) static_cast<int32>(key)

	Button Keyboard::GetKey(KeyCode key) {
		// get the key
		KeyValue& keyval = keyValues[KEY_INDEX(key)];

		// return the values as a button struct
		return Button(keyval.isPressed, keyval.hasChanged);
	}

	string Keyboard::KeyToString(KeyCode key) {
		switch (key) {
			case KeyCode::KeyA: return "A";
			case KeyCode::KeyB: return "B";
			case KeyCode::KeyC: return "C";
			case KeyCode::KeyD: return "D";
			case KeyCode::KeyE: return "E";
			case KeyCode::KeyF: return "F";
			case KeyCode::KeyG: return "G";
			case KeyCode::KeyH: return "H";
			case KeyCode::KeyI: return "I";
			case KeyCode::KeyJ: return "J";
			case KeyCode::KeyK: return "K";
			case KeyCode::KeyL: return "L";
			case KeyCode::KeyM: return "M";
			case KeyCode::KeyN: return "N";
			case KeyCode::KeyO: return "O";
			case KeyCode::KeyP: return "P";
			case KeyCode::KeyQ: return "Q";
			case KeyCode::KeyR: return "R";
			case KeyCode::KeyS: return "S";
			case KeyCode::KeyT: return "T";
			case KeyCode::KeyU: return "U";
			case KeyCode::KeyV: return "V";
			case KeyCode::KeyW: return "W";
			case KeyCode::KeyX: return "X";
			case KeyCode::KeyY: return "Y";
			case KeyCode::KeyZ: return "Z";
			case KeyCode::Num0: return "0";
			case KeyCode::Num1: return "1";
			case KeyCode::Num2: return "2";
			case KeyCode::Num3: return "3";
			case KeyCode::Num4: return "4";
			case KeyCode::Num5: return "5";
			case KeyCode::Num6: return "6";
			case KeyCode::Num7: return "7";
			case KeyCode::Num8: return "8";
			case KeyCode::Num9: return "9";
			case KeyCode::LeftShift:	return "Left Shift";
			case KeyCode::RightShift:	return "Right Shift";
			case KeyCode::LeftCtrl:		return "Left Ctrl";
			case KeyCode::RightCtrl:	return "Right Ctrl";
			case KeyCode::LeftAlt:		return "Left Alt";
			case KeyCode::RightAlt:		return "Right Alt";
			case KeyCode::Return:		return "Return";
			case KeyCode::Escape:		return "Escape";
			case KeyCode::Backspace:	return "Backspace";
			case KeyCode::Tab:			return "Tab";
			case KeyCode::Space:		return "Space";
			case KeyCode::ArrowUp:		return "Arrow Up";
			case KeyCode::ArrowDown:	return "Arrow Down";
			case KeyCode::ArrowLeft:	return "Arrow Left";
			case KeyCode::ArrowRight:	return "Arrow Right";
			default: return std::to_string(static_cast<unsigned int>(key));
		}
	}

	void Keyboard::__ClearChangedBits() {
		// loop through and set every other bit to false
		for (size_t i = 0; i < keyValues.size(); ++i)
			keyValues[i].hasChanged = false;
		s_lastkey = KeyCode::None;
	}

	void Keyboard::__SetKey(KeyCode key, bool value) {
		// get the key
		KeyValue& keyval = keyValues[KEY_INDEX(key)];

		// get the old state
		bool old_state = keyval.isPressed;

		// update the key with the new state
		keyval.isPressed = value;

		// if the state changed then we should record that too
		if (old_state != value) keyval.hasChanged = true;

		s_lastkey = key;
	}

}