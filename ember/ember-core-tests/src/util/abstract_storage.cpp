#include "util/abstract_storage.h"

struct base_t {
	virtual ~base_t() = default;
	virtual QString get_name() const = 0;
	virtual int64_t get_number() const = 0;
};

struct concrete_t : public base_t {
	concrete_t(int number)
		: m_number(number) {}
	QString get_name() const override {
		return "concrete";
	}

	int64_t get_number() const override{
		return m_number;
	}

	bool operator==(const concrete_t& other) const {
		return m_number == other.m_number;
	}

	int64_t m_number;
};

using namespace ember::util;
TEST(abstract_storage, insert_object) {
	abstract_storage<base_t> storage;
	EXPECT_TRUE(storage.empty());
	storage.insert(concrete_t{12});
	EXPECT_EQ(storage.size(), 1);
}

TEST(abstract_storage, insert_pointer) {
	abstract_storage<base_t> storage;
	EXPECT_TRUE(storage.empty());
	storage.insert(std::make_unique<concrete_t>(12));
	EXPECT_EQ(storage.size(), 1);
}

TEST(abstract_storage, insert_type) {
	abstract_storage<base_t> storage;
	EXPECT_TRUE(storage.empty());
	storage.insert<concrete_t>(12);
	EXPECT_EQ(storage.size(), 1);
}

TEST(abstract_storage, get_default_type) {
	abstract_storage<base_t> storage;
	storage.insert(std::make_unique<concrete_t>(12));
	const auto& ioc_object = storage.get(0);
	static_assert(std::is_same_v<decltype(ioc_object), const base_t&>, "return type does not match");
}

TEST(abstract_storage, get_derived_type) {
	abstract_storage<base_t> storage;
	storage.insert(std::make_unique<concrete_t>(12));
	const auto& object = storage.get<concrete_t>(0);
	static_assert(std::is_same_v<decltype(object), const concrete_t&>, "return type does not match");
}

TEST(abstract_storage, remove_by_object) {
	abstract_storage<base_t> storage;
	storage.insert(std::make_unique<concrete_t>(12));
	const auto& obj = storage.get<concrete_t>(0);
	EXPECT_EQ(storage.size(), 1);
	storage.remove(obj);
	EXPECT_EQ(storage.size(), 0);
}

TEST(abstract_storage, remove_by_predicate) {
	abstract_storage<base_t> storage;
	storage.insert(std::make_unique<concrete_t>(12));
	const auto predicate = [](const base_t& object) { return object.get_name() == "concrete"; };
	EXPECT_EQ(storage.size(), 1);
	storage.remove(predicate);
	EXPECT_EQ(storage.size(), 0);
}

TEST(abstract_storage, range_based_loop) {
	abstract_storage<base_t> storage;
	for (int i = 0; i < 10; ++i) {
		storage.insert<concrete_t>(1);
	}

	int64_t sum(0);
	for (const auto& item : storage) {
		sum += item->get_number();
	}
	EXPECT_EQ(sum, 10);
}