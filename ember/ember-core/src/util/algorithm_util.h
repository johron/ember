#pragma once

namespace ember::util {
template <class container_t, class predicate_t>
auto erase_if(container_t& container, const predicate_t& predicate) {
	return container.erase(std::remove_if(container.begin(), container.end(), predicate), container.end());
}

template <class container_t, class value_t>
auto erase(container_t& container, const value_t& value) {
	return container.erase(std::remove(container.begin(), container.end(), value), container.end());
}

template <class container_t, class value_t>
[[nodiscard]] auto find(const container_t& container, const value_t& value) {
	return std::find(container.begin(), container.end(), value);
}

template <class container_t, class predicate_t>
[[nodiscard]] auto find_if(const container_t& container, predicate_t&& predicate) {
	return std::find_if(container.begin(), container.end(), std::forward<predicate_t>(predicate));
}

template <class container_t, class predicate_t>
[[nodiscard]] auto find_if(container_t& container, predicate_t&& predicate) {
	return std::find_if(container.begin(), container.end(), std::forward<predicate_t>(predicate));
}

template <class container_t, class predicate_t>
[[nodiscard]] auto remove_if(container_t& container, predicate_t&& predicate) {
	return std::remove_if(container.begin(), container.end(), std::forward<predicate_t>(predicate));
}

template <class container_t, class predicate_t>
auto sort(container_t& container, predicate_t&& predicate) {
	return std::sort(container.begin(), container.end(), predicate);
}

template <class container_t, class functor_t>
void for_each(container_t& container, functor_t& functor) {
	std::for_each(container.begin(), container.end(), functor);
}

}  // namespace ember::util