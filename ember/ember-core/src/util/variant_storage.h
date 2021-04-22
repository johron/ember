#pragma once
#include <QObject>
#include <QVariant>

namespace ember::util {
struct variant_storage : public QObject {
	Q_OBJECT
public:
	void set(const QString& key, const QVariant& value) {
		const auto it = m_storage.find(key);
		m_storage[key] = value;
		if (it == m_storage.end()) {
			emit inserted(key, value);
		} 

		emit changed(key, value);
	}

	[[nodiscard]] QVariant get(const QString& key, const QVariant& default_value = QVariant{}) const {
		const auto it = m_storage.find(key);
		return it != m_storage.end() ? *it : default_value;
	}

	template <class value_t>
	[[nodiscard]] auto get(const QString& key, const QVariant& default_value = QVariant{}) const {
		return get(key, default_value).value<value_t>();
	}

	[[nodiscard]] QStringList keys() const {
		return m_storage.keys();
	}

	[[nodiscard]] auto begin() const {
		return m_storage.begin();
	}

	[[nodiscard]] auto end() const {
		return m_storage.end();
	}

	void erase(const QString& key) {
		const auto it = m_storage.find(key);
		if (it != m_storage.end()) {
			m_storage.erase(it);
			emit erased(key);
		}
	}

	[[nodiscard]] auto empty() const {
		return m_storage.empty();
	}

	[[nodiscard]] auto size() const {
		return m_storage.size();
	}

	void clear() {
		const auto data = std::exchange(m_storage, QVariantMap{});
		for (const auto& key : data.keys()) {
			emit erased(key);
		}
	}

	[[nodiscard]] bool contains(const QString& key) const {
		return m_storage.find(key) != m_storage.end();
	}

signals:
	void inserted(const QString& key, const QVariant& value);
	void changed(const QString& key, const QVariant& value);
	void erased(const QString& key);

private:
	QVariantMap m_storage;
};
}  // namespace ember::util