#pragma once
#include "../util/module_registry.h"
#include "module_base.h"

namespace ember::module {
struct registry : public util::module_registry<module::base> {
};

struct manager {

};


}  // namespace ember::component