#ifndef ALC_DATATYPES_TIMESTEP_HPP
#define ALC_DATATYPES_TIMESTEP_HPP

namespace alc {

	// structure to represent a delta value
	struct timestep final {

		constexpr timestep();
		constexpr timestep(double delta);

		// returns the stored delta value as a float
		operator float() const;

		// returns the stored delta value 
		double get() const;

	private:
		double m_delta;
	};

	// implementations

	inline constexpr timestep::timestep() : m_delta(0) { }

	inline constexpr timestep::timestep(double delta) : m_delta(delta) { }

	inline timestep::operator float() const { 
		return static_cast<float>(m_delta);
	}

	inline double timestep::get() const {
		return m_delta;
	}

}

#endif // !ALC_DATATYPES_TIMESTEP_HPP