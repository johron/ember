#pragma once
#include <QDate>
#include <QStandardPaths>
#include <QMessageBox>
#include <fstream>

namespace ember::util {
struct log_framework {
	log_framework();

	using log_function = std::function<void(QtMsgType, const QMessageLogContext&, const QString&)>;
	void add_handler(log_function&& log_func) {
		m_loggers.emplace_back(log_func);
	}

	void handle_message(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
		for (const auto& logger : m_loggers) {
			logger(type, context, msg);
		}
	}

private:
	std::vector<log_function> m_loggers;
};

namespace log {
struct dialog_popup {
	template <class... arg_t>
	dialog_popup(arg_t... args)
		: m_types{args...} {}

	void operator()(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
		if (util::find(m_types, type) != m_types.end()) {
			QMessageBox::warning(nullptr, QString("Warning: ") + context.category, msg);
		}
	}

	std::vector<QtMsgType> m_types;
};
}  // namespace log
}  // namespace ember::util
