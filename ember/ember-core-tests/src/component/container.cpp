#include "component/component_container.h"

using namespace ember;

namespace test {
struct business_logic : public component::object {
	virtual void apply() = 0;
};
struct editor_logic : public business_logic {
	void apply() override {}
};
struct application : public component::object {
	application(business_logic& logic)
		: m_logic(logic) {}
	virtual void run() = 0;

	business_logic& m_logic;
};
struct editor_application : public application {
	editor_application(business_logic& logic)
		: application(logic) {}

	void run() override {
		m_logic.apply();
	}
};
}  // namespace test

TEST(container, construct) {
	component::container container;
	EXPECT_EQ(container.size(), 0);
}

TEST(container, insert_type) {
	component::container container;
	container.insert<test::editor_logic>();
	EXPECT_EQ(container.size(), 1);
}

TEST(container, resolve_type) {
	component::container container;
	container.insert<test::editor_logic>();
	[[maybe_unused]] const auto& logic = container.resolve<test::editor_logic>();
}

TEST(container, insert_object) {
	component::container container;
	container.insert(test::editor_logic{});
	EXPECT_EQ(container.size(), 1);
}

TEST(container, resolve_objec) {
	component::container container;
	container.insert(test::editor_logic{});
	[[maybe_unused]] const auto& logic = container.resolve<test::editor_logic>();
}

TEST(container, insert_with_dependency) {
	component::container container;
	container.insert<test::editor_logic>();
	container.insert<test::editor_application>(component::dependency<test::editor_logic>{});
	EXPECT_EQ(container.size(), 2);
}

TEST(container, resolve_with_dependency) {
	component::container container;
	container.insert<test::editor_logic>();
	container.insert<test::editor_application>(component::dependency<test::editor_logic>{});
	[[maybe_unused]] const auto& app = container.resolve<test::editor_application>();
}

TEST(container, resolve_invalid_type) {
	component::container container;
	try {
		[[maybe_unused]] auto& unused = container.resolve<test::editor_logic>();
	} catch (std::exception& except) {
		EXPECT_STREQ(except.what(), "invalid map<K, T> key");
	}
}

TEST(container, insert_with_alias) {
	component::container container;
	container.insert<test::editor_logic>().as<test::business_logic>();
	const auto& obj_1 = container.resolve<test::editor_logic>();
	const auto& obj_2 = container.resolve<test::business_logic>();
	EXPECT_EQ(std::addressof(obj_1), std::addressof(obj_2));
}

TEST(container, sample_usage) {
	component::container container;
	container.insert<test::editor_logic>().as<test::business_logic>();
	container.insert<test::editor_application>(component::dependency<test::business_logic>{}).as<test::application>();
	[[maybe_unused]] const auto& app = container.resolve<test::application>();
	EXPECT_NE(dynamic_cast<const test::editor_application*>(&app), nullptr);
}
