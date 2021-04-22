#pragma once
#include "algorithm_util.h"

namespace ember::util {
template <class abstract_t>
struct abstract_storage {
	template <class object_t>
	object_t& insert(object_t&& object) {
		static_assert(std::is_base_of_v<abstract_t, object_t>, "object is not derived from abstract storage type");
		m_storage.push_back(std::make_unique<object_t>(std::forward<object_t>(object)));
		return static_cast<object_t&>(*m_storage.back());
	}

	template <class object_t, class... arg_t>
	object_t& insert(arg_t&&... args) {
		static_assert(std::is_base_of_v<abstract_t, object_t>, "object is not derived from abstract storage type");
		m_storage.push_back(std::make_unique<object_t>(std::forward<arg_t>(args)...));
		return static_cast<object_t&>(*m_storage.back());
	}

	template <class object_t>
	object_t& insert(std::unique_ptr<object_t> object) {
		static_assert(std::is_base_of_v<abstract_t, object_t>, "object is not derived from abstract storage type");
		m_storage.push_back(std::move(object));
		return static_cast<object_t&>(*m_storage.back());
	}

	template <class concrete_t, std::enable_if_t<!std::is_invocable_v<concrete_t, abstract_t>, int> = 0>
	void remove(const concrete_t& object) {
		static_assert(std::is_base_of_v<abstract_t, concrete_t>, "object is not derived from abstract storage type");
		auto is_equal = [&object](std::unique_ptr<abstract_t>& current) {
			return static_cast<concrete_t&>(*current) == object;
		};
		util::erase_if(m_storage, is_equal);
	}

	void remove(std::function<bool(abstract_t&)> predicate) {
		auto wrapper = [predicate](std::unique_ptr<abstract_t>& object) { return predicate(*object); };
		util::erase_if(m_storage, wrapper);
	}

	template <class concrete_t = abstract_t>
	[[nodiscard]] concrete_t& get(std::size_t index) {
		return static_cast<concrete_t&>(*m_storage.at(index));
	}

	template <class concrete_t = abstract_t>
	[[nodiscard]] const concrete_t& get(std::size_t index) const {
		return static_cast<concrete_t&>(*m_storage.at(index));
	}

	[[nodiscard]] auto begin() const {
		return m_storage.begin();
	}

	[[nodiscard]] auto end() const {
		return m_storage.end();
	}

	[[nodiscard]] std::size_t size() const {
		return m_storage.size();
	}

	[[nodiscard]] bool empty() const {
		return m_storage.empty();
	}

private:
	std::vector<std::unique_ptr<abstract_t>> m_storage;
};
}  // namespace ember::util