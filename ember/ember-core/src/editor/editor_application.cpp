#include "editor_application.h"
#include <QCoreApplication>

inline void resource_init() {
// 	Q_INIT_RESOURCE(editor_textures);
// 	Q_INIT_RESOURCE(editor_fonts);
// 	Q_INIT_RESOURCE(theme_default);
}

inline void resource_cleanup() {
// 	Q_CLEANUP_RESOURCE(editor_textures);
// 	Q_CLEANUP_RESOURCE(editor_fonts);
// 	Q_CLEANUP_RESOURCE(theme_default);
}
namespace ember::editor {
application::application(std::unique_ptr<component::container> container)
	: m_container(std::move(container))
	, m_editor(std::make_unique<graph::editor>()) {
	resource_init();
	const auto id = QFontDatabase::addApplicationFont(":/editor/roboto_condensed/RobotoCondensed-Regular.ttf");
	if (id >= 0) {
		qApp->setFont(QFont(QFontDatabase::applicationFontFamilies(id).front()));
	} else {
		qCritical() << "failed to add application font";
	}
	qApp->setStyleSheet(ember::util::file::read<QByteArray>(":/default/theme.qss"));
}

application::~application() {
	resource_cleanup();
}

void application::object_create() {
	m_editor->graph_create();
}

void application::object_load(QString filename) {
	m_editor->graph_load(filename);
}

void application::object_save(QString filename) {
	m_editor->graph_save(filename);
}

}  // namespace ember::editor
