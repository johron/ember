#include "editor_window.h"
#include <QColorDialog>
#include <QStatusBar>
#include <QMenuBar>
#include "../views/widget_gallery.h"
#include "../views/version_widget.h"
#include "../widgets/icon.h"
#include "../widgets/label.h"
#include "editor_scene_view.h"
#include "../module/module_dockable.h"

namespace ember::editor {
main_window::main_window(editor::application& application, Qt::WindowFlags flags)
	: QMainWindow(nullptr, flags)
	, m_application(application) {
	setWindowIcon(icon::awesome(fa::flask));
	setWindowTitle("Nova editor");
	setDockNestingEnabled(true);

	//addDockWidget(Qt::LeftDockWidgetArea, new module::dockable(application.registry(), this));

	setCentralWidget(new editor_scene_view{this});
	message::post(msg::created_main_window{*this});
	const auto msg_dispatch = [this](const auto& msg) { on_message(msg); };
	m_listeners.emplace_back(message::register_listener<msg::show_status_message>(msg_dispatch));
	m_listeners.emplace_back(message::register_listener<msg::graph_opened>(msg_dispatch));

	create_menu_file(menuBar()->addMenu("&File"));
	create_menu_edit(menuBar()->addMenu("&Edit"));
	create_menu_view(menuBar()->addMenu("&View"));
	create_menu_help(menuBar()->addMenu("&Help"));
	if (build::get_target() == build::target::development) {
		create_menu_dev(menuBar()->addMenu("Developer"));
	}

	load_state();
	statusBar()->setSizeGripEnabled(false);
	statusBar()->insertPermanentWidget(0, new view::version_widget(build::get_target(), build::version::number(), this));
	application.object_create();
}
main_window::~main_window() {
	save_state();
}

void main_window::save_state() {
	auto& settings = m_application.get<ember::user_settings::manager>();
	settings.write("editor_window/geometry", saveGeometry());
	settings.write("editor_window/state", saveState());
	if (auto view = findChild<ember::editor_scene_view*>()) {
		settings.write("editor_window/view_transform", view->transform());
	}
}
void main_window::load_state() {
	auto& settings = m_application.get<ember::user_settings::manager>();
	restoreGeometry(settings.read<QByteArray>("editor_window/geometry"));
	restoreState(settings.read<QByteArray>("editor_window/state"));
	if (auto view = findChild<ember::editor_scene_view*>()) {
		view->setTransform(settings.read<QTransform>("editor_window/view_transform", QTransform{}));
	}
}
void ember::editor::main_window::on_message(const msg::show_status_message& msg) {
	statusBar()->showMessage(msg.m_message);
}
void main_window::on_message(const msg::graph_opened& msg) {
	if (const auto view = findChild<editor_scene_view*>()) {
		view->setScene(&msg.m_view);
	}
}
void main_window::create_menu_file(QMenu* menu) {
	menu->addAction("New graph");
	menu->addAction("Load graph");
	menu->addAction("Save graph");
	menu->addAction("Save graph as...");
	menu->addAction(
		"Exit", [this]() { close(); }, QKeySequence::Quit);
}
void main_window::create_menu_edit(QMenu* menu) {
	message::post(msg::created_menu_edit{*menu});
}
void main_window::create_menu_view(QMenu* menu) {
	for (auto dock : findChildren<QDockWidget*>()) {
		menu->addAction(dock->toggleViewAction());
	}
	message::post(msg::created_menu_view{*menu});
}
void main_window::create_menu_help(QMenu* menu) {
	message::post(msg::created_menu_help{*menu});
}
void main_window::create_menu_dev(QMenu* menu) {
	menu->addAction("Stylesheet Helper", [this]() { ember::view::style_helper::pick_style(this); });
	menu->addAction("Widget Gallery", [this]() {
		auto g = new ember::view::widget_gallery(this);
		g->show();
	});
}
}  // namespace ember::editor