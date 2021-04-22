#pragma once
#include <type_traits>

#if _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4201)	 // nonstandard extension used: nameless struct/union
#elif __GNUC__
#pragma GCC diagnostic ignored "-Wpedantic"	 // ISO C++ prohibits anonymous structs [-Wpedantic]
#pragma GCC diagnostic push
#endif

namespace ember::util {

template <class T, std::size_t N>
struct vector_data {};

template <class T>
struct vector_data<T, 2> {
	union {
		T m_data[2];
		struct {
			T x;
			T y;
		};
	};
};

template <class T>
struct vector_data<T, 3> {
	union {
		T m_data[3];
		struct {
			T x;
			T y;
			T z;
		};
	};
};

template <class T, std::size_t N>
struct vector_base : public vector_data<T, N> {
protected:
	constexpr vector_base()
		: vector_data<T, N>{} {}

	template <class... arg_t>
	constexpr vector_base(arg_t&&... args)
		: vector_data<T, N>{static_cast<T>(std::forward<arg_t>(args))...} {}

public:
	[[nodiscard]] constexpr std::size_t size() const {
		return N;
	}

	[[nodiscard]] auto begin() {
		return m_data;
	}

	[[nodiscard]] auto begin() const {
		return m_data;
	}

	[[nodiscard]] auto end() {
		return m_data + N;
	}

	[[nodiscard]] auto end() const {
		return m_data + N;
	}

	[[nodiscard]] T& operator[](std::size_t index) {
		return m_data[index];
	}

	[[nodiscard]] constexpr const T& operator[](std::size_t index) const {
		return m_data[index];
	}

	using vector_data<T, N>::m_data;
};

template <class T, std::size_t N, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
struct vector : public vector_base<T, N> {
	constexpr vector()
		: vector_base<T, N>{} {}

	template <class... arg_t, std::enable_if_t<sizeof...(arg_t) == N, int> = 0>
	constexpr vector(arg_t&&... args)
		: vector_base<T, N>{std::forward<arg_t>(args)...} {}

	template <class U>
	constexpr vector(const vector<U, N>& other)
		: vector<T, N>{other, std::make_index_sequence<N>{}} {}

	template <class U, std::size_t... I>
	constexpr vector(const vector<U, N>& other, std::index_sequence<I...>)
		: vector_base<T, N>{other[I]...} {}
};

namespace detail::vector {
template <class T, class U, std::size_t N, std::size_t... I>
constexpr auto add(const util::vector<T, N>& lhs, const util::vector<U, N>& rhs, std::index_sequence<I...>) {
	using result_t = std::common_type_t<T, U>;
	return util::vector<result_t, N>{(lhs[I] + rhs[I])...};
}

template <class T, class U, std::size_t N, std::size_t... I>
constexpr auto sub(const util::vector<T, N>& lhs, const util::vector<U, N>& rhs, std::index_sequence<I...>) {
	using result_t = std::common_type_t<T, U>;
	return util::vector<result_t, N>{(lhs[I] - rhs[I])...};
}

template <class T, class U, std::size_t N, std::size_t... I>
constexpr auto mul(const util::vector<T, N>& lhs, const U& rhs, std::index_sequence<I...>) {
	using result_t = std::common_type_t<T, U>;
	return util::vector<result_t, N>{(static_cast<result_t>(lhs[I]) * rhs)...};
}

template <class T, class U, std::size_t N, std::size_t... I>
constexpr auto div(const util::vector<T, N>& lhs, const U& rhs, std::index_sequence<I...>) {
	using result_t = std::common_type_t<T, U>;
	return util::vector<result_t, N>{(static_cast<result_t>(lhs[I]) / rhs)...};
}

}  // namespace detail::vector

template <class T, class U, std::size_t N>
constexpr bool operator==(const vector<T, N>& lhs, const vector<U, N>& rhs) {
	using common_t = std::common_type_t<T, U>;
	for (std::size_t i = 0; i < N; ++i) {
		if (static_cast<common_t>(lhs[i]) != static_cast<common_t>(rhs[i])) {
			return false;
		}
	}
	return true;
}

template <class T, class U, std::size_t N>
[[nodiscard]] constexpr bool operator!=(const vector<T, N>& lhs, const vector<U, N>& rhs) {
	return !(lhs == rhs);
}

template <class T, class U, std::size_t N>
[[nodiscard]] constexpr auto operator+(const vector<T, N>& lhs, const vector<U, N>& rhs) {
	return detail::vector::add(lhs, rhs, std::make_index_sequence<N>{});
}

template <class T, class U, std::size_t N>
constexpr auto operator+=(vector<T, N>& lhs, const vector<U, N>& rhs) {
	lhs = detail::vector::add(lhs, rhs, std::make_index_sequence<N>{});
	return lhs;
}

template <class T, class U, std::size_t N>
[[nodiscard]] constexpr auto operator-(const vector<T, N>& lhs, const vector<U, N>& rhs) {
	return detail::vector::sub(lhs, rhs, std::make_index_sequence<N>{});
}

template <class T, class U, std::size_t N>
constexpr auto operator-=(vector<T, N>& lhs, const vector<U, N>& rhs) {
	lhs = detail::vector::sub(lhs, rhs, std::make_index_sequence<N>{});
	return lhs;
}

template <class T, class U, std::size_t N>
[[nodiscard]] constexpr auto operator*(const vector<T, N>& lhs, const U& rhs) {
	return detail::vector::mul(lhs, rhs, std::make_index_sequence<N>{});
}

template <class T, class U, std::size_t N>
constexpr auto operator*=(vector<T, N>& lhs, const U& rhs) {
	lhs = detail::vector::mul(lhs, rhs, std::make_index_sequence<N>{});
	return lhs;
}

template <class T, class U, std::size_t N>
[[nodiscard]] constexpr auto operator/(const vector<T, N>& lhs, const U& rhs) {
	return detail::vector::div(lhs, rhs, std::make_index_sequence<N>{});
}

template <class T, class U, std::size_t N>
constexpr auto operator/=(vector<T, N>& lhs, const U& rhs) {
	lhs = detail::vector::div(lhs, rhs, std::make_index_sequence<N>{});
	return lhs;
}

using vector2f = vector<float, 2>;
using vector2d = vector<double, 2>;
using vector2i = vector<int32_t, 2>;
using vector2u = vector<uint32_t, 2>;

using vector3f = vector<float, 3>;
using vector3d = vector<double, 3>;
using vector3i = vector<int32_t, 3>;
using vector3u = vector<uint32_t, 3>;

}  // namespace ember::util

#if _MSC_VER
#pragma warning(pop)  // nonstandard extension used: nameless struct/union
#elif __GNUC__
#pragma GCC diagnostic pop	// ISO C++ prohibits anonymous structs [-Wpedantic]
#endif