#pragma once
#include <QScrollArea>

namespace ember::inspector::view {
struct empty : public QScrollArea {
	empty(QWidget* parent = nullptr)
		: QScrollArea(parent) {
		setWidgetResizable(true);
		setWidget(new QWidget(this));
		widget()->setLayout(new QHBoxLayout(this));
		widget()->layout()->setContentsMargins(2, 2, 2, 2);
		widget()->layout()->setAlignment(Qt::AlignCenter);
		widget()->layout()->addWidget(qt::make_label(icon::awesome(fa::search).pixmap(150, 150), this));
	}
};
}