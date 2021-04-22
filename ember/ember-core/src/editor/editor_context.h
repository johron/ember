#pragma once
#include "../module/module_registry.h"
#include <QObject>

namespace ember::editor {
template <class impl_t>
struct context {
	bool load(const QJsonDocument& json) {
		return static_cast<impl_t*>(this)->load(json);
	}

	bool save(QJsonDocument& json) {
		return static_cast<const impl_t*>(this)->save(json);
	}

	void activate() {
		static_cast<impl_t*>(this)->activate();
	}

	void deactivate() {
		static_cast<impl_t*>(this)->deactivate();
	}
};
}  // namespace ember

template <class resource_t>
struct editor {

};

struct dungeon {};
struct dungeon_editor : public ember::editor::context<dungeon_editor> {
	bool load(const QJsonDocument& json) {
		return false;
	}
	bool save(QJsonDocument& json) const {
		return false;
	}

	dungeon& resource() {
		return *m_resource;
	}

	std::unique_ptr<dungeon> m_resource;
};


struct node_graph {};
struct node_editor : public ember::editor::context<node_editor> {
	bool load(const QJsonDocument& json) {}
	bool save(QJsonDocument& json) const {}
};