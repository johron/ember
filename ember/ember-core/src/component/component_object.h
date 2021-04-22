#pragma once

namespace ember::component {
struct object {
	object() = default;
	object(const object& other) = delete;
	object(object&& other) = default;
	virtual ~object() = default;
};
}  // namespace ember::component