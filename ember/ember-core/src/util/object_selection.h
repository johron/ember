#pragma once

namespace ember::util {
template <class object_t>
struct selection : public dynamic_base {
public:
	void insert(object_t& object) {
		if (!contains(object)) {
			m_storage.emplace_back(&object);
			emit changed();
		}
	}

	void remove(object_t& object) {
		if (contains(object)) {
			util::erase(m_storage, &object);
			emit changed();
		}
	}

	void clear() {
		if (!m_storage.empty()) {
			m_storage.clear();
			emit changed();
		}
	}

	[[nodiscard]] bool contains(object_t& object) const {
		return util::find(m_storage, &object) != m_storage.end();
	}

	[[nodiscard]] auto begin() {
		return m_storage.begin();
	}

	[[nodiscard]] auto end() {
		return m_storage.end();
	}

	[[nodiscard]] const auto& content() const {
		return m_storage;
	}

private:
	std::vector<object_t*> m_storage;
};
}  // namespace ember::util