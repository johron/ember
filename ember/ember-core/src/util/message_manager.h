#pragma once
#include <any>
#include <typeindex>

namespace ember::util {
struct message_manager {
	struct cancel_token {
		cancel_token(message_manager* system, std::type_index event_id, uint32_t listener_id)
			: m_system(system)
			, m_event_id(event_id)
			, m_listener_id(listener_id) {}

		~cancel_token() {
			detach();
		}

		void detach() {
			if (m_system) {
				m_system->remove_listener(m_event_id, m_listener_id);
				m_system = nullptr;
			}
		}

		[[nodiscard]] bool is_detached() const {
			return m_system == nullptr;
		}

	private:
		message_manager* m_system;
		std::type_index m_event_id;
		uint32_t m_listener_id;
	};

	struct listener_token {
		listener_token() = default;
		~listener_token() = default;

		listener_token(std::shared_ptr<cancel_token> token)
			: m_token(token) {}

		listener_token(const listener_token& other) {
			*this = other;
		}

		listener_token(listener_token&& other) {
			*this = std::forward<listener_token>(other);
		}

		listener_token& operator=(const listener_token& other) {
			m_token = other.m_token;
			return *this;
		}

		listener_token& operator=(listener_token&& other) {
			std::swap(m_token, other.m_token);
			return *this;
		}

		void invalidate() {
			if (is_valid()) {
				m_token->detach();
			}
		}

		[[nodiscard]] bool is_valid() const {
			return m_token && !m_token->is_detached();
		}

	private:
		std::shared_ptr<cancel_token> m_token;
	};

	message_manager()
		: m_listener_id_counter(0) {}
	~message_manager() {
		detach_all();
	}

	message_manager(const message_manager& other) = delete;
	message_manager(message_manager&& other) = delete;
	message_manager& operator=(const message_manager& other) = delete;
	message_manager& operator=(message_manager&& other) = delete;

	template <class event_t, class object_t, class func_t>
	[[nodiscard]] listener_token register_listener(object_t* object, func_t func_ptr) {
		return register_listener<event_t>(std::bind(func_ptr, object, std::placeholders::_1));
	}

	template <class event_t>
	[[nodiscard]] listener_token register_listener(std::function<void(const event_t& event)> func) {
		const auto listener_id = m_listener_id_counter++;
		const auto token = std::make_shared<cancel_token>(this, typeid(event_t), listener_id);
		m_listeners[typeid(event_t)][listener_id] = listener_data{func, token};
		return listener_token{token};
	}

	void remove_listener(std::type_index event_id, uint32_t listener_id) {
		auto& listeners = m_listeners[event_id];
		listeners.erase(listener_id);
	}

	template <class event_t>
	void post(const event_t& event) {
		auto& listeners = m_listeners[typeid(event_t)];
		using callback = std::function<void(const event_t&)>;
		for (auto& entry : listeners) {
			const auto& func = std::any_cast<callback>(entry.second.m_callback);
			func(event);
		}
	}

	template <class event_t>
	void enqueue(event_t&& event) {
		m_message_queue.emplace_back([event, this]() { post(event); });
	}

	void flush() {
		std::vector<std::function<void()>> empty_queue;
		std::swap(empty_queue, m_message_queue);
		for (const auto& message : empty_queue) {
			message();
		}
	}

	template <class event_t>
	[[nodiscard]] std::size_t listener_count() const {
		const auto it = m_listeners.find(typeid(event_t));
		return it != m_listeners.end() ? it->second.size() : 0;
	}

private:
	void detach_all() {
		listener_tree empty;
		std::swap(m_listeners, empty);
		for (auto& group : empty) {
			for (auto& entry : group.second) {
				if (auto current_data = entry.second.m_token.lock()) {
					current_data->detach();
				} else {
					throw std::runtime_error(
						"found dead listener that should already have been removed - investigate!");
				}
			}
		}
	}

	struct listener_data {
		std::any m_callback;
		std::weak_ptr<cancel_token> m_token;
	};

	using listener_id_t = uint32_t;
	using listener_group = std::map<listener_id_t, listener_data>;
	using listener_tree = std::map<std::type_index, listener_group>;
	listener_tree m_listeners;
	std::vector<std::function<void()>> m_message_queue;
	listener_id_t m_listener_id_counter;
};
}  // namespace ember::util