#pragma once
#include <QMenu>
#include "user_settings_section.h"
#include "user_settings_dialog.h"

namespace ember::user_settings {
struct manager : public util::settings_storage, public component::object {
	manager()
		: util::settings_storage(qApp->organizationName(), qApp->applicationName()) {
		m_listeners.emplace_back(
			message::register_listener<msg::created_menu_file>([this](const msg::created_menu_file& msg) {
				msg.m_menu.addSeparator();
				msg.m_menu.addAction("Preferences", [this]() {
					user_settings::dialog dialog(m_sections, nullptr);
					dialog.exec();
				});
			}));

		add_section<detail::general>();
	}

	template <class section_t, class... arg_t>
	void add_section(arg_t&&... args) {
		m_sections.emplace_back(std::make_unique<section_t>(std::forward<arg_t>(args)...));
		const auto read_data = [this](QString key) { return read(key); };
		const auto write_data = [this](QString key, QVariant value) { write(key, value); };
		QObject::connect(m_sections.back().get(), &section::request_write, write_data);
		QObject::connect(m_sections.back().get(), &section::request_read, read_data);
	}

	[[nodiscard]] auto begin() const {
		return m_sections.begin();
	}

	[[nodiscard]] auto end() const {
		return m_sections.end();
	}

	template <class value_t = QVariant>
	value_t read(QString key, QVariant default_value = QVariant{}) const {
		return util::settings_storage::get<value_t>(key, default_value);
	}

	void write(QString key, QVariant value) {
		util::settings_storage::set(key, value);
	}

private:
	std::vector<std::unique_ptr<section>> m_sections;
	std::vector<message::token> m_listeners;
};
}  // namespace ember::settings
