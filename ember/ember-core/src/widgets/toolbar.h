#pragma once
#include <QToolBar>

namespace ember::widget {
struct toolbar : public QToolBar {
	toolbar(QString title, QWidget* parent)
		: QToolBar(title, parent) {
		setObjectName("toolbar." + title.replace(' ', '_').toLower());
	}
};
}