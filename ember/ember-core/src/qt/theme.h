#pragma once

namespace ember::qt {
struct theme {
	using icon_colors = std::array<QColor, 4>;
	icon_colors m_icon_colors;
};

struct default_theme : theme {
	default_theme()
		: theme{{0xffffff, 0x444444, 0xffffff, 0xffffff}} {}
};

static const theme current_theme = default_theme();
}  // namespace ember::qt