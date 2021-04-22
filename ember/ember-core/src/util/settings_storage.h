#pragma once
#include <QCoreApplication>
#include <QSettings>

#include "variant_storage.h"

namespace ember::util {
struct settings_storage : public variant_storage {
	settings_storage(const QString& filename, QObject* parent = nullptr)
		: m_settings(filename, QSettings::IniFormat, parent) {
		initialize();
	}

	settings_storage(QString organization, QString application, QObject* parent = nullptr)
		: m_settings(QSettings::IniFormat, QSettings::UserScope, organization, application, parent) {
		initialize();
	}

	auto filename() const {
		return m_settings.fileName();
	}

private:
	void initialize() {
		for (const auto& key : m_settings.allKeys()) {
			set(key, m_settings.value(key));
		}
		const auto set_value = [this](const QString& key, const QVariant& value) { m_settings.setValue(key, value); };
		QObject::connect(this, &variant_storage::changed, set_value);
		const auto remove_key = [this](const QString& key) { m_settings.remove(key); };
		QObject::connect(this, &variant_storage::erased, remove_key);
	}

	QSettings m_settings;
};
}  // namespace ember::util