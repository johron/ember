#pragma once
#include <optional>
#include <vector>
#include "../util/vector.h"
#include "hex_pos.h"

namespace ember::hex {
template <class topped_t>
[[nodiscard]] inline util::vector2f hex_to_pixel(const hex::detail::layout<topped_t>& layout, hex::pos hex) {
	const auto& ori = layout.m_orientation;
	const auto& size = layout.m_size;
	const auto x = (ori.f[0] * hex.q() + ori.f[1] * hex.r()) * size;
	const auto y = (ori.f[2] * hex.q() + ori.f[3] * hex.r()) * size;
	return {x, y};
}

namespace detail {
inline auto hex_round(const detail::pos<double>& pos) {
	auto qi = int(round(pos.q()));
	auto ri = int(round(pos.r()));
	auto si = int(round(pos.s()));
	const auto q_diff = abs(qi - pos.q());
	const auto r_diff = abs(ri - pos.r());
	const auto s_diff = abs(si - pos.s());
	if (q_diff > r_diff && q_diff > s_diff) {
		qi = -ri - si;
	} else if (r_diff > s_diff) {
		ri = -qi - si;
	} else {
		si = -qi - ri;
	}
	return hex::pos{static_cast<int16_t>(qi), static_cast<int16_t>(ri) /*, si*/};
}
}  // namespace detail

template <class topped_t>
[[nodiscard]] inline hex::pos pixel_to_hex(const hex::detail::layout<topped_t>& layout, const util::vector2f& pixel) {
	const auto& M = layout.m_orientation;
	const auto pt = pixel / layout.m_size;
	const double q = M.b[0] * pt.x + M.b[1] * pt.y;
	const double r = M.b[2] * pt.x + M.b[3] * pt.y;
	return detail::hex_round({q, r});
}

template <uint8_t corner, class topped_t>
constexpr inline auto vertex(double size) {
	constexpr auto deg_to_rad = 3.14159265358979323846 / 180;
	constexpr auto angle = (60 * corner - topped_t::angle) * deg_to_rad;
	static const auto a = cos(angle);
	static const auto b = sin(angle);
	return util::vector2f{size * a, size * b};
}

template <class topped_t>
[[nodiscard]] inline auto vertices(const detail::layout<topped_t>& layout) -> std::array<util::vector2f, 6> {
	return {vertex<0, topped_t>(layout.m_size),
			vertex<1, topped_t>(layout.m_size),
			vertex<2, topped_t>(layout.m_size),
			vertex<3, topped_t>(layout.m_size),
			vertex<4, topped_t>(layout.m_size),
			vertex<5, topped_t>(layout.m_size)};
}
}  // namespace ember::hex
