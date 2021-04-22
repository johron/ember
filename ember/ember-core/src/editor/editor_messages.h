#pragma once

namespace ember {
struct message : public util::singleton<util::message_manager> {
	using token = util::message_manager::listener_token;

	template <class event_t>
	static void post(const event_t& event) {
		get_instance()->post(event);
	}

	template <class event_t>
	static void enqueue(event_t&& event) {
		get_instance()->enqueue(std::forward<event_t>(event));
	}

	static void flush() {
		get_instance()->flush();
	}

	template <class event_t, class object_t, class func_t>
	[[nodiscard]] static auto register_listener(object_t* object, func_t func_ptr){
		return get_instance()->register_listener<event_t>(object, func_ptr);
	}

	template <class event_t>
	[[nodiscard]] static auto register_listener(std::function<void(const event_t& event)> func) {
		return get_instance()->register_listener(func);
	}

	template <class event_t>
	[[nodiscard]] static auto listener_count() {
		return get_instance()->listener_count<event_t>();
	}
};
}  // namespace ember::msg