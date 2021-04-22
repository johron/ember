#pragma once

namespace ember::util {
template <class T>
struct singleton {
	singleton(const singleton& other) = delete;
	singleton& operator=(const singleton& other) = delete;
	virtual ~singleton() = default;

	template <class... arg_t>
	static T* create(arg_t&&... args) {
		assert(!m_instance && "singleton isn't empty");
		m_instance = std::make_unique<T>(std::forward(args)...);
		return m_instance.get();
	}

	static void destroy() {
		assert(m_instance && "singleton already empty");
		m_instance = nullptr;
	}

	[[nodiscard]] static T* get_instance() {
		return m_instance.get();
	}

private:
	singleton() = default;

	static std::unique_ptr<T> m_instance;
};

template <class T>
inline std::unique_ptr<T> ember::util::singleton<T>::m_instance;
}