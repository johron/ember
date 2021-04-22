#pragma once
#include "node_model.h"

namespace ember::graph {
struct model {
	model() {
		m_nodes.push_back(std::make_unique<accumulate_node>());
		m_nodes.push_back(std::make_unique<constant_node<int>>(5));
	}

	std::vector<std::unique_ptr<node>> m_nodes;
};
}