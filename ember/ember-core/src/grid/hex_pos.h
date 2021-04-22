#pragma once
#include <assert.h>
#include <corecrt_math.h>
#include <stdint.h>

namespace ember::hex {
namespace detail {
#if _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4201)	 // nonstandard extension used: nameless struct/union
#elif __GNUC__
#pragma GCC diagnostic ignored "-Wpedantic"	 // ISO C++ prohibits anonymous structs [-Wpedantic]
#pragma GCC diagnostic push
#endif
template <class value_t>
struct pos {
	constexpr pos(const value_t& q, const value_t& r)
		: m_data{q, r} {}

	constexpr pos(const pos<value_t>& other)
		: m_data{other.q(), other.r()} {}

	[[nodiscard]] constexpr auto q() const {
		return m_data[0];
	}

	[[nodiscard]] constexpr auto r() const {
		return m_data[1];
	}

	[[nodiscard]] constexpr auto s() const {
		return -m_data[0] - m_data[1];
	}

private:
	value_t m_data[2];
};

template <class value_t>
[[nodiscard]] constexpr bool operator==(const pos<value_t>& lhs, const pos<value_t>& rhs) {
	return lhs.q() == rhs.q() && lhs.r() == rhs.r();
}
template <class value_t>
[[nodiscard]] constexpr bool operator!=(const pos<value_t>& lhs, const pos<value_t>& rhs) {
	return !(lhs == rhs);
}
template <class value_t>
[[nodiscard]] constexpr auto operator+(const pos<value_t>& lhs, const pos<value_t>& rhs) {
	return pos<value_t>{lhs.q() + rhs.q(), lhs.r() + rhs.r()};
}
template <class value_t>
[[nodiscard]] constexpr auto operator-(const pos<value_t>& lhs, const pos<value_t>& rhs) {
	return pos<value_t>{lhs.q() - rhs.q(), lhs.r() - rhs.r()};
}
#if _MSC_VER
#pragma warning(pop)  // nonstandard extension used: nameless struct/union
#elif __GNUC__
#pragma GCC diagnostic pop	// ISO C++ prohibits anonymous structs [-Wpedantic]
#endif

struct top_flat {
	static const int64_t angle = 0;
};
struct top_pointed {
	static const int64_t angle = 30;
};

template <class topped_t>
struct orientation {
	double f[4];
	double b[4];
};

template <class topped_t>
struct layout {
	orientation<topped_t> m_orientation;
	uint64_t m_size;
};
}  // namespace detail

struct flat_layout : public detail::layout<detail::top_flat> {
	flat_layout(uint64_t size)
		: detail::layout<detail::top_flat>{
			  {{3.0 / 2.0, 0.0, sqrt(3.0) / 2.0, sqrt(3.0)}, {2.0 / 3.0, 0.0, -1.0 / 3.0, sqrt(3.0) / 3.0}},
			  size} {}
};

struct pointy_layout : public detail::layout<detail::top_pointed> {
	pointy_layout(uint64_t size)
		: detail::layout<detail::top_pointed>{
			  {{sqrt(3.0), sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0}, {sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0}},
			  size} {}
};

template <class value_t>
[[nodiscard]] value_t distance(const detail::pos<value_t>& first, const detail::pos<value_t>& second) {
	return (std::abs(first.q() - second.q()) + std::abs(first.r() - second.r()) + std::abs(first.s() - second.s())) / 2;
}

using pos = detail::pos<int32_t>;
}  // namespace ember::hex