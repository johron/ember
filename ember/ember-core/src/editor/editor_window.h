#pragma once
#include <QMainWindow>

namespace ember::msg {
struct created_main_window {
	QMainWindow& m_window;
};
struct created_menu_file {
	QMenu& m_menu;
};
struct created_menu_view {
	QMenu& m_menu;
};
struct created_menu_edit {
	QMenu& m_menu;
};
struct created_menu_help {
	QMenu& m_menu;
};
struct show_status_message {
	QString m_message;
};
}  // namespace ember::msg

namespace ember::editor {
struct main_window : public QMainWindow {
	main_window(editor::application& application, Qt::WindowFlags flags = Qt::WindowFlags());
	~main_window();

private:
	void save_state();
	void load_state();

	void on_message(const msg::show_status_message& msg);
	void on_message(const msg::graph_opened& msg);

	void create_menu_file(QMenu* menu);
	void create_menu_edit(QMenu* menu);
	void create_menu_view(QMenu* menu);
	void create_menu_help(QMenu* menu);
	void create_menu_dev(QMenu* menu);

	std::vector<message::token> m_listeners;
	application& m_application;
};
}  // namespace ember::editor
