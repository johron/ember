#pragma once
#include "module_registry.h"



namespace ember::module {
struct default_registry : public registry {
	default_registry() {
		//construct_type<ember::user_settings::manager>();
// 		construct_type<ember::inspector::manager>();
// 		construct_type<ember::network::manager>();
// 		construct_type<ember::undo::stack>();
	}
};
}