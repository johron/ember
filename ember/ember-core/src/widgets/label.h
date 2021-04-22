#pragma once
#include <QLabel>

namespace ember::widget {
struct label : public QLabel {
	label(QString text, QWidget* parent)
		: QLabel(text, parent) {}
	label(QString text, int64_t size, QWidget* parent)
		: QLabel(text, parent) {
		auto current_font = font();
		current_font.setPointSize(size);
		setFont(current_font);
	}
};
}