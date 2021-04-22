#pragma once
#include <QPushButton>

namespace ember::qt {
struct icon_button : public QPushButton {
	icon_button(QIcon icon, QWidget* parent = nullptr)
		: QPushButton(icon, "", parent) {
		setObjectName("IconButton");
	}
};
}  // namespace ember::qt