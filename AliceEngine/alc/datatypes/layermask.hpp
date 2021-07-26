#ifndef ALC_DATATYPES_LAYERMASK_HPP
#define ALC_DATATYPES_LAYERMASK_HPP
#include "../common.hpp"

namespace alc {

	template<typename I>
	struct layermask {
	private:
		struct accessor;
	public:

		// mask with all layers set
		constexpr layermask<I> ALL = ~static_cast<I>(0);

		// mask with no layers set
		static inline const layermask<I> NONE = static_cast<I>(0);

		// constructor
		constexpr layermask() : m_mask(0) { }

		// constructor
		constexpr layermask(const I mask_) : m_mask(mask_) { }

		// setter
		layermask& operator=(const I mask_) {
			m_mask = mask_;
			return *this;
		}

		// operators

		// cast to unsigned int
		operator I() const { return m_mask; }

		// accessor
		accessor operator[](size_t index) { return accessor(*this, index); }

		// const accessor
		const accessor operator[](size_t index) const { return accessor(*this, index); }

		// checks if the masks match exactly
		bool operator==(const layermask& other) const { return m_mask == other.m_mask; }

		// checks if the masks dont match exactly
		bool operator!=(const layermask& other) const { return m_mask != other.m_mask; }

		// returns true if *any* layers match
		bool operator&&(const layermask& other) const { return m_mask & other.m_mask; }

		// returns a layermask with only the matching layers
		layermask operator&(const layermask& other) const { return layermask(m_mask & other.m_mask); }

		// returns a layermask with only the matching layers
		layermask& operator&=(const layermask& other) {
			m_mask &= other.m_mask;
			return *this;
		}

		// used to check if any layers are set
		operator bool() const { return m_mask != 0; }

		// creates a new layermask with the layers of the two old ones
		layermask operator|(const layermask& other) const { return layermask(m_mask | other.m_mask); }

		// creates a new layermask with the layers of the two old ones
		layermask& operator|=(const layermask& other) {
			m_mask |= other.m_mask;
			return *this;
		}

		// returns a reversed layermask
		layermask operator~() const { return layermask(~m_mask); }

		// function to set/unset layers
		void set(size_t layer, bool value) {
			if (value)	m_mask |= (1 << layer);  // set true
			else		m_mask &= ~(1 << layer); // set false
		}

		// function to get layer
		bool get(size_t layer) const {
			return m_mask & (1 << layer);
		}

	private:
		struct accessor final {
			accessor(layermask& mask, size_t layer) : m_mask(mask), m_layer(layer) { }
			operator bool() const { return m_mask.get(m_layer); }
			accessor operator=(bool value) {
				m_mask.set(m_layer, value);
				return accessor(m_mask, m_layer);
			}
		private:
			layermask& m_mask;
			size_t m_layer;
		};

		I m_mask;
	};


	// 64 layer mask
	using layermask64 = layermask<uint64>;

	// 32 layer mask
	using layermask32 = layermask<uint32>;

	// 16 layer mask
	using layermask16 = layermask<uint16>;

	// 8 layer mask
	using layermask8 = layermask<uint8>;

}

#endif // !ALC_DATATYPES_LAYERMASK_HPP