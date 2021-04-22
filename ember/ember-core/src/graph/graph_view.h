#pragma once
#include <QGraphicsScene>
#include "node_view.h"

namespace ember::graph {
struct scene : QGraphicsScene {
	scene(graph::model& model)
		: m_model(model) {
		for (const auto& node : model.m_nodes) {
			addItem(new view::node(*node, nullptr));
		}
	}

private:
	graph::model& m_model;
};
}