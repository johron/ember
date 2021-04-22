#pragma once
#include <QUndoStack>
#include "undo_dock.h"

namespace ember::undo {
struct stack : public module::mandatory, public QUndoStack {
	stack() {
		m_listeners.emplace_back(message::register_listener<msg::created_main_window>(
			[](const auto& msg) { msg.m_window.addDockWidget(Qt::LeftDockWidgetArea, new view::dock()); }));
	}

private:
	std::vector<message::token> m_listeners;
};
}  // namespace ember::undo