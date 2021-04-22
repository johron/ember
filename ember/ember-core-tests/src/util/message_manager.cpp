#include "util/message_manager.h"

using namespace ember::util;

struct test_message {
	int m_number;
};

struct test_object {
	uint32_t m_count = 0;
	void on_message(const test_message& /*message*/) {
		++m_count;
	}
};

TEST(message_manager, register_listener_obj) {
	message_manager events;
	const auto token = events.register_listener<test_message>([](auto& /*event*/) {});
	EXPECT_EQ(events.listener_count<test_message>(), 1u);
}

TEST(message_manager, post_event_obj) {
	message_manager events;
	uint32_t message_count(0);
	const auto token = events.register_listener<test_message>([&message_count](auto& /*event*/) { ++message_count; });
	EXPECT_EQ(message_count, 0u);
	events.post(test_message{1});
	EXPECT_EQ(message_count, 1u);
}

TEST(message_manager, register_listener_ptr) {
	message_manager events;
	test_object object;
	const auto token = events.register_listener<test_message>(&object, &test_object::on_message);
	EXPECT_EQ(events.listener_count<test_message>(), 1u);
}

TEST(message_manager, post_event_ptr) {
	message_manager events;
	test_object object;
	const auto token = events.register_listener<test_message>(&object, &test_object::on_message);
	EXPECT_EQ(object.m_count, 0u);
	events.post(test_message{1});
	EXPECT_EQ(object.m_count, 1u);
}

TEST(message_manager, enqueue_event) {
	message_manager events;
	uint32_t message_count(0);
	const auto token = events.register_listener<test_message>([&message_count](auto& /*event*/) { ++message_count; });
	events.enqueue(test_message{1});
	EXPECT_EQ(message_count, 0u);
	events.flush();
	EXPECT_EQ(message_count, 1u);
}

TEST(message_manager, expired_listener_token_removes_listener) {
	message_manager events;
	uint32_t message_count(0);
	{
		auto token = events.register_listener<test_message>([&message_count](auto& /*event*/) { ++message_count; });
	}
	events.post(test_message{1});
	EXPECT_EQ(message_count, 0u);
}

TEST(message_manager, listener_token_can_be_copied) {
	message_manager events;
	uint32_t message_count(0);
	{
		auto first_token =
			events.register_listener<test_message>([&message_count](auto& /*event*/) { ++message_count; });
		auto second_token(first_token);
	}
	events.post(test_message{1});
	EXPECT_EQ(message_count, 0u);
}

TEST(message_manager, listener_token_can_expire_after_system) {
	auto system = std::make_unique<message_manager>();
	auto first_token = system->register_listener<test_message>([](auto& /*event*/) {});
	system.reset();
}

TEST(message_manager, invalidated_listener_token_stops_events) {
	message_manager events;
	uint32_t message_count(0);
	auto token = events.register_listener<test_message>([&message_count](auto& /*event*/) { ++message_count; });
	auto tokenCopy(token);	// even if a copy exists, invalidating the listener token will stop all events
	events.post(test_message{1});
	EXPECT_EQ(message_count, 1u);
	token.invalidate();
	events.post(test_message{1});
	EXPECT_EQ(message_count, 1u);
}