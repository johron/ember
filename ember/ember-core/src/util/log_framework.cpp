#include "log_framework.h"

namespace {
using log_function = std::function<void(QtMsgType, const QMessageLogContext&, const QString&)>;
log_function the_logger;

void log_entrypoint(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
	the_logger(type, context, msg);
}
}  // namespace

namespace ember::util {
log_framework::log_framework() {
	::the_logger = [this](QtMsgType type, const QMessageLogContext& context, const QString& msg) {
		handle_message(type, context, msg);
	};
	auto originalHandler = qInstallMessageHandler(::log_entrypoint);
	add_handler(originalHandler);
}
}  // namespace ember::util