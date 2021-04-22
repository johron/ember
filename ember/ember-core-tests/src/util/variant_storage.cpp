#include "util/variant_storage.h"

using namespace ember::util;

TEST(variant_storage, empty) {
	variant_storage storage;
	EXPECT_TRUE(storage.empty());
	storage.set("test", "data");
	EXPECT_FALSE(storage.empty());
}

TEST(variant_storage, size) {
	variant_storage storage;
	EXPECT_EQ(storage.size(), 0);
	storage.set("test", "data");
	EXPECT_EQ(storage.size(), 1);
	storage.erase("test");
	EXPECT_EQ(storage.size(), 0);
}

TEST(variant_storage, data_storage) {
	variant_storage storage;
	EXPECT_EQ(storage.get("test"), QVariant{});
	storage.set("test", "data");
	EXPECT_EQ(storage.get<QString>("test"), "data");
}

TEST(variant_storage, signal_inserted) {
	variant_storage storage;
	int signal_triggered = 0;
	QObject::connect(&storage, &variant_storage::inserted, [&signal_triggered]() { signal_triggered++; });
	storage.set("test", "data");
	EXPECT_EQ(signal_triggered, 1);
}

TEST(variant_storage, signal_changed) {
	variant_storage storage;
	int signal_triggered = 0;
	QObject::connect(&storage, &variant_storage::changed, [&signal_triggered]() { signal_triggered++; });
	storage.set("test", "data");
	storage.set("test", 2);
	EXPECT_EQ(signal_triggered, 2);
}

TEST(variant_storage, signal_erased) {
	variant_storage settings_storage;
	int signal_triggered = 0;
	QObject::connect(&settings_storage, &variant_storage::erased, [&signal_triggered]() { signal_triggered++; });
	settings_storage.erase("test");
	EXPECT_EQ(signal_triggered, 0);
	settings_storage.set("test", "data");
	settings_storage.erase("test");
	EXPECT_EQ(signal_triggered, 1);
}

TEST(variant_storage, clear) {
	variant_storage storage;
	int signal_triggered = 0;
	QObject::connect(&storage, &variant_storage::erased, [&signal_triggered]() { signal_triggered++; });
	for (int i = 0; i < 10; ++i) {
		storage.set(QString::number(i), i);
	}
	EXPECT_EQ(storage.size(), 10);
	storage.clear();
	EXPECT_EQ(signal_triggered, 10);
	EXPECT_EQ(storage.size(), 0);
}

TEST(variant_storage, contains) {
	variant_storage storage;
	EXPECT_FALSE(storage.contains("test"));
	storage.set("test", true);
	EXPECT_TRUE(storage.contains("test"));
	storage.erase("test");
	EXPECT_FALSE(storage.contains("test"));
}

TEST(variant_storage, keys) {
	variant_storage storage;
	storage.set("boolean", true);
	storage.set("numeric", 12);
	const QStringList expected{"boolean", "numeric"};
	EXPECT_EQ(storage.keys(), expected);
}

TEST(variant_storage, begin_end) {
	variant_storage storage;
	storage.set("first", 1);
	storage.set("second", 2);
	std::vector<int> numbers;
	for (const auto& entry : storage) {
		numbers.push_back(entry.toInt());
	}
	const std::vector<int> expected{1, 2};
	EXPECT_EQ(numbers, expected);
}