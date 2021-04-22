#pragma once
#include <QUndoView>

namespace ember::undo::view {
struct dock : public widget::dockable {
	static const QString name;
	dock(QWidget* parent = nullptr)
		: widget::dockable(name, parent) {
		setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
		setWidget(new QUndoView(this));
	}
};

inline const QString dock::name = "Action History";
}  // namespace ember::undo::view