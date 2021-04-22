#pragma once
#include <QDockWidget>

namespace ember::widget {
struct dockable : public QDockWidget {
	dockable(QString title, QWidget* parent)
		: QDockWidget(title, parent) {
		setObjectName("dockable." + title.replace(' ', '_').toLower());
	}
};
}  // namespace ember::qt