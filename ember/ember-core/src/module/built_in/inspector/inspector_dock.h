#pragma once
#include "inspector_view_empty.h"

namespace ember::inspector::view {
struct dock : public widget::dockable{
	Q_OBJECT
public:
	static const QString name;

	dock(QWidget* parent = nullptr)
		: widget::dockable(name, parent) {
		setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
		set_empty();
	}
		
	void set_empty() {
		set_content(new view::empty(this));
	}

	void set_content(QWidget* content) {
		if (auto current = widget()) {
			current->deleteLater();
		}
		setWidget(content);
	}
};

inline const QString dock::name = "Inspector";

}  // namespace ember::inspector::view