#pragma once
#include <QApplication>
#include "editor_application.h"
#include "editor_messages.h"
#include "editor_window.h"

namespace ember {
template <class components_t>
int main(int argc, char** argv, const char* organization_name) {
	static_assert(std::is_base_of_v<component::container, components_t>);
	qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");

	QApplication application(argc, argv);
	application.setOrganizationName(organization_name);
	application.setStyle("Fusion");

	util::log_framework logger;
	logger.add_handler(util::log::dialog_popup(QtCriticalMsg, QtFatalMsg));

	message::create();

	editor::application editor(std::make_unique<components_t>());
	editor::main_window window(editor);
	window.show();
	const auto return_code = application.exec();

	message::destroy();
	return return_code;
}
}  // namespace ember