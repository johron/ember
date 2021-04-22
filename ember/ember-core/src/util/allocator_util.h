#pragma once

namespace ember::util {
template <class object_t>
struct unique_allocator {
	template <class... arg_t>
	auto operator()(arg_t&&... args) {
		return std::make_unique<object_t>(std::forward<arg_t>(args)...);
	}
};

template <class object_t>
struct heap_allocator {
	template <class... arg_t>
	auto operator()(arg_t&&... args) {
		return new object_t(std::forward<arg_t>(args)...);
	}
};

template <class object_t>
struct stack_allocator {
	template <class... arg_t>
	auto operator()(arg_t&&... args) {
		return object_t(std::forward<arg_t>(args)...);
	}
};
}  // namespace ember::util