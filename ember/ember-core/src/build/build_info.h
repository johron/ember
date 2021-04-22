#pragma once

namespace ember {
struct build_info : public ember::module::base {
	build_info(const QString& name, const QVersionNumber& version)
		: m_name(name)
		, m_version(version) {}

	QString m_name;
	QVersionNumber m_version;
};
}  // namespace ember