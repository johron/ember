#include "util/dynamic_value.h"

using namespace ember::util;

TEST(dynamic_value, construct) {
	dynamic_value<QString> original("sample string");
	dynamic_value<QString> copy(original.get());
	EXPECT_EQ(original, copy);
}

TEST(dynamic_value, implicit_conversion) {
	const auto string_interface = [](const QString& string) { return string.length(); };
	dynamic_value<QString> string("test");
	EXPECT_EQ(string_interface(string), 4);
}

TEST(dynamic_value, change_signal) {
	auto change_count(0);
	const auto change_func = [&change_count]() { ++change_count; };
	dynamic_value<QString> string("test");
	QObject::connect(&string, &dynamic_value<QString>::changed, change_func);
	EXPECT_EQ(change_count, 0);
	string = "something different";
	EXPECT_EQ(change_count, 1);
}

TEST(dynamic_value, observe_no_param) {
	auto change_count(0);
	const auto change_func = [&change_count]() { ++change_count; };
	dynamic_value<QString> string("test");
	string.observe(change_func);
	EXPECT_EQ(change_count, 0);
	string = "something different";
	EXPECT_EQ(change_count, 1);
	string.set("a second change");
	EXPECT_EQ(change_count, 2);
}

TEST(dynamic_value, observe_with_param) {
	auto string_length(0);
	const auto change_func = [&string_length](const QString& string) { string_length = string.length(); };
	dynamic_value<QString> string("test");
	string.observe(change_func);
	EXPECT_EQ(string_length, 0);
	string = "something different";
	EXPECT_EQ(string_length, 19);
	string.set("a second change");
	EXPECT_EQ(string_length, 15);
}

TEST(dynamic_value, get) {
	dynamic_value<uint64_t> int_value(123);
	static_assert(std::is_same_v<decltype(int_value.get()), const uint64_t&>);
	EXPECT_EQ(int_value.get(), 123);
}

TEST(dynamic_value, set) {
	dynamic_value<uint64_t> int_value(123);
	EXPECT_EQ(int_value.get(), 123);
	int_value.set(456);
	EXPECT_EQ(int_value.get(), 456);
}
