#pragma once

namespace ember::util {
struct disable_copy {
	disable_copy() = default;
	disable_copy(const disable_copy&) = delete;
	disable_copy& operator=(const disable_copy&) = delete;
	virtual ~disable_copy() = default;
};

struct disable_move {
	disable_move() = default;
	disable_move(disable_move&&) = delete;
	disable_move& operator=(disable_move&&) = delete;
	virtual ~disable_move() = default;
};

struct disable_move_and_copy : public disable_move, public disable_copy {
	disable_move_and_copy() = default;
	virtual ~disable_move_and_copy() = default;
};
}