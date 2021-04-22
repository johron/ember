#pragma once
#include "inspector_dock.h"

namespace ember::msg {
template <class object_t>
struct inspect {
	object_t& m_object;
};
}  // namespace ember::msg

namespace ember::inspector {
template <class object_t>
struct view_provider {
	virtual ~view_provider() = default;
	virtual QWidget* make_view(object_t& object) const = 0;
};

struct manager : public QObject, public module::mandatory {
	Q_OBJECT
public:
	manager() {
		m_listeners.emplace_back(
			message::register_listener<msg::created_main_window>([this](const auto& msg) { on_message(msg); }));
	}

	void on_message(const msg::created_main_window& message) {
		auto dockable = new view::dock();
		message.m_window.addDockWidget(Qt::LeftDockWidgetArea, dockable);
		QObject::connect(this, &manager::view_changed, dockable, &view::dock::set_content);
	}

	template <class object_t>
	void on_message(const msg::inspect<object_t>& message, const view_provider<object_t>& provider) {
		emit view_changed(provider.make_view(message.m_object));
	}

	template <class object_t>
	void register_type(std::shared_ptr<view_provider<object_t>> view_provider) {
		m_listeners.emplace_back(message::register_listener<msg::inspect<object_t>>(
			[this, view_provider](const auto& message) { on_message(message, *view_provider); }));
	}

signals:
	void view_changed(QWidget* view);

private:
	std::vector<message::token> m_listeners;
};
}  // namespace ember::inspector