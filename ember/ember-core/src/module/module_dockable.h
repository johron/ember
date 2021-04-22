#pragma once
#include "../widgets/dockable.h"

namespace ember::module {
struct dockable : public widget::dockable {
	dockable(module::registry& /*registry*/, QWidget* parent)
		: widget::dockable("Module Registry", parent) {
		auto layout = new QFormLayout();
		setLayout(layout);
	}
};
}  // namespace ember::module