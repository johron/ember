#pragma once
#include "../util/uncopyable.h"

namespace ember::module {
struct base : public util::disable_move_and_copy {
protected:
	base() = default;
};

struct optional : public base {};
struct mandatory : public base {};

template <class impl_t>
struct module_entry : public module::base {
	static QString name() {
		return impl_t::name();
	}

	virtual void on_activate() {}
	virtual void on_deactivate() {}

	QString m_name;
};

}  // namespace ember::module