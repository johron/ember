#pragma once
#include "../util/uncopyable.h"

namespace ember::qt {
struct connection : public util::disable_copy {
	connection() = default;
	explicit connection(QMetaObject::Connection id)
		: m_id(id) {}

	connection(connection&& other) noexcept
		: m_id(std::exchange(other.m_id, {})) {}

	connection& operator=(connection&& other) {
		m_id = std::exchange(other.m_id, {});
		return *this;
	}

	template <class... arg_t>
	connection(arg_t&&... args)
		: m_id(QObject::connect(args...)) {}

	~connection() {
		QObject::disconnect(m_id);
	}

	const auto& get_id() const {
		return m_id;
	}

private:
	QMetaObject::Connection m_id;
};

using connections = std::vector<connection>;
}  // namespace ember::qt