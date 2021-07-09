#ifndef ALC_DATATYPES_BUTTON_HPP
#define ALC_DATATYPES_BUTTON_HPP

namespace alc {

	struct button {

		constexpr button() 
			: m_held(false), m_changed(false) { }

		constexpr button(bool held, bool changed)
			: m_held(held), m_changed(changed) { }

		// returns the held state of the button
		operator bool() const { return m_held; }

		// returns the held state of the button
		bool held() const { return m_held; }

		// returns if this was pressed 
		bool pressed() const { return m_held && m_changed; }

		// returns if this was released
		bool released() const { return !m_held && m_changed; }

	private:
		bool m_held : 1;
		bool m_changed : 1;
	};

}

#endif // !ALC_DATATYPES_BUTTON_HPP