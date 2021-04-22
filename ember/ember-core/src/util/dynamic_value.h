#pragma once
#include "dynamic_base.h"

namespace ember::util {
template <class value_t>
struct dynamic_value : public dynamic_base {
public:
	dynamic_value() = default;
	dynamic_value(value_t value)
		: m_value(value) {}
	dynamic_value(const dynamic_value& other) = delete;
	~dynamic_value() = default;

	dynamic_value& operator=(const value_t other) {
		set(other);
		return *this;
	}

	operator const value_t&() const {
		return m_value;
	}

	[[nodiscard]] const value_t& get() const {
		return m_value;
	}

	void set(const value_t& value) {
		if (value != m_value) {
			m_value = value;
			emit changed();
		}
	}


	auto observe(std::function<void(value_t)> listener) {
		return QObject::connect(this, &dynamic_base::changed, [listener, this]() { listener(m_value); });
	}

	template <class event_t, class object_t, class func_t>
	auto observe(object_t* object, func_t func_ptr) {
		std::function<void(value_t)> func = std::bind(func_ptr, object, std::placeholders::_1);
		return observe(func);
		
	}

	auto observe(std::function<void()> listener) {
		return QObject::connect(this, &dynamic_base::changed, listener);
	}

private:
	value_t m_value;
};
}  // namespace ember::util