#pragma once
#include <any>
#include <map>
#include <memory>
#include <set>
#include <typeindex>
#include "component_object.h"

namespace ember::component {
template <class object_t>
struct dependency {};

struct container;
template <class object_t>
struct binder {
	binder(container& container)
		: m_container(container) {}
	template <class alias_t>
	void as() {
		m_container.bind<object_t, alias_t>();
	}

private:
	container& m_container;
};

struct container {
	template <class object_t>
	friend struct binder;

	template <class object_t>
	[[maybe_unused]] binder<object_t> insert(std::unique_ptr<object_t> object) {
		static_assert(std::is_base_of_v<component::object, object_t>, "object does not inherit from component::object");
		m_objects.insert_or_assign(typeid(object_t), std::move(object));
		return binder<object_t>{*this};
	}

	template <class object_t>
	[[maybe_unused]] binder<object_t> insert(object_t&& object) {
		return insert(std::make_unique<object_t>(std::forward<object_t>(object)));
	}

	template <class object_t, class... arg_t>
	[[maybe_unused]] binder<object_t> insert(arg_t&&... args) {
		return insert(std::make_unique<object_t>(get_object(std::forward<arg_t>(args))...));
	}

	template <class object_t>
	[[nodiscard]] const object_t& resolve() const {
		static_assert(std::is_base_of_v<object, object_t>, "object does not inherit from component::object");
		return static_cast<object_t&>(*m_objects.at(get_type(typeid(object_t))));
	}

	template <class object_t>
	[[nodiscard]] object_t& resolve() {
		static_assert(std::is_base_of_v<object, object_t>, "object does not inherit from component::object");
		return static_cast<object_t&>(*m_objects.at(get_type(typeid(object_t))));
	}

	[[nodiscard]] std::size_t size() const {
		return m_objects.size();
	}

	template <class object_t>
	[[nodiscard]] bool contains() const {
		return m_objects.find(typeid(object_t)) != m_objects.end();
	}

private:
	template <class object_t, class alias_t>
	void bind() {
		m_bindings.insert_or_assign(typeid(alias_t), typeid(object_t));
	}
	std::type_index get_type(std::type_index type) {
		const auto it = m_bindings.find(type);
		return it != m_bindings.end() ? get_type(it->second) : type;
	}
	template <class object_t>
	const object_t& get_object(const object_t& object) const {
		return object;
	}
	template <class object_t>
	object_t& get_object(object_t& object) {
		return object;
	}
	template <class object_t>
	object_t get_object(object_t&& object) {
		return std::forward<object_t>(object);
	}
	template <class object_t>
	object_t& get_object(dependency<object_t>&&) {
		return resolve<object_t>();
	}

	std::map<std::type_index, std::unique_ptr<object>> m_objects;
	std::map<std::type_index, std::type_index> m_bindings;
};
}  // namespace ember::component