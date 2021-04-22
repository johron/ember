#pragma once
#include <QLabel>
#include "../icon/impl_font_awesome.h"

namespace ember::widget {
struct icon : public QLabel {
	icon(fa::icon icon, QSize size, QWidget* parent)
		: icon(ember::icon::awesome(icon).pixmap(size), parent) {}

	icon(const QPixmap& pixmap, QWidget* parent)
		: QLabel(parent) {
		setPixmap(pixmap);
	}

};
}