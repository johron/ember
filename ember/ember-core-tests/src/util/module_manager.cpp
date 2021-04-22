#include "util/module_registry.h"

using base_t = ember::module::base;
using test_builder = ember::util::module_registry<base_t>;

struct logging_module : public base_t {};
struct network_module : public base_t {};
struct tracking_module : public base_t {
	tracking_module(network_module& network)
		: m_network(network) {}
	network_module& m_network;
};

struct store_module : public base_t {
	store_module(network_module& network, tracking_module& tracking)
		: m_network(network)
		, m_tracking(tracking) {}
	network_module& m_network;
	tracking_module& m_tracking;
};

TEST(module_registry, insert) {
	test_builder module_registry;
	EXPECT_FALSE(module_registry.is_registered<logging_module>());
	module_registry.register_type<logging_module>();
	EXPECT_FALSE(module_registry.is_constructed<logging_module>());
}

TEST(module_registry, construct) {
	test_builder module_registry;
	module_registry.register_type<logging_module>();
	EXPECT_FALSE(module_registry.is_constructed<logging_module>());
	module_registry.construct<logging_module>();
	EXPECT_TRUE(module_registry.is_registered<logging_module>());
}

struct numeric_module : public base_t {
	explicit numeric_module(int number)
		: m_number(number) {}
	int m_number;
};

struct fixed_ten_module : public numeric_module {
	fixed_ten_module()
		: numeric_module(10) {}
};

TEST(module_registry, module_alias) {
	test_builder module_registry;
	module_registry.register_type<fixed_ten_module, numeric_module>();
	auto& sample = module_registry.get<numeric_module>();
	EXPECT_EQ(sample.m_number, 10);
}

TEST(module_registry, get_module) {
	test_builder module_registry;
	module_registry.register_type<logging_module>();
	module_registry.construct<logging_module>();
	const auto& sample_module = module_registry.get<logging_module>();
	static_assert(std::is_same_v<std::decay_t<decltype(sample_module)>, logging_module>);
}

TEST(module_registry, visit) {
	test_builder module_registry;
	module_registry.register_type<logging_module>();
	module_registry.register_type<network_module>();
	module_registry.visit([](base_t&) {});
}

TEST(module_registry, construct_unregistered_type) {
	test_builder module_registry;
	try {
		module_registry.construct<logging_module>();
	} catch (const std::domain_error& error) {
		EXPECT_EQ(error.what(), std::string("unknown type, unable to construct"));
	}
}

TEST(module_registry, destroy_single_type) {
	test_builder module_registry;
	module_registry.register_type<logging_module>();
	module_registry.register_type<network_module>();
	module_registry.construct<network_module>();
	EXPECT_FALSE(module_registry.is_constructed<logging_module>());
	EXPECT_TRUE(module_registry.is_constructed<network_module>());
	module_registry.destroy<network_module>();
	EXPECT_FALSE(module_registry.is_constructed<network_module>());
}

TEST(module_registry, unload_unregistered) {
	test_builder module_registry;
	try {
		module_registry.destroy<logging_module>();
	} catch (const std::domain_error& error) {
		EXPECT_EQ(error.what(), std::string("unknown type, unable to unload"));
	}
}

TEST(module_registry, construct_with_dependencies) {
	test_builder module_registry;
	module_registry.register_type<network_module>();
	module_registry.register_type<tracking_module>(test_builder::dependency<network_module>{});
	module_registry.register_type<store_module>(test_builder::dependency<network_module>{},
											   test_builder::dependency<tracking_module>{});

	module_registry.construct<store_module>();
}