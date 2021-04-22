#pragma once
#include <array>
#include <QColor>

namespace ember::editor::test {
struct theme {
	struct overlay_color {
		QColor m_normal;
		QColor m_disabled;
		QColor m_active;
		QColor m_selected;
	};

	struct panel_color {
		QColor m_normal;
		QColor m_variant;
		overlay_color m_overlay;
	};

	panel_color m_primalry;
	panel_color m_secondary;
	panel_color m_background;
};
}

