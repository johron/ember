#pragma once
#include "graph_model.h"
#include "graph_view.h"

namespace ember::msg {
struct graph_opened {
	graph::model& m_model;
	graph::scene& m_view;
};
struct graph_saved {
	graph::model& m_model;
};
}  // namespace ember::msg

namespace ember::graph {
struct editor : public QObject {
	Q_OBJECT
public:
	void graph_create() {
		qDebug() << "new graph";
		m_model = std::make_unique<graph::model>();
		m_view = std::make_unique<graph::scene>(*m_model);
		message::post(msg::graph_opened{*m_model, *m_view});
	}
	void graph_load(QString filename) {
		qDebug() << "load graph" << filename;
		m_model = std::make_unique<graph::model>();
		m_view = std::make_unique<graph::scene>(*m_model);
		message::post(msg::graph_opened{*m_model, *m_view});
	}
	void graph_save(QString /*filename*/) {
		// to do
		message::post(msg::graph_saved{*m_model});
	}

private:
	std::unique_ptr<graph::model> m_model;
	std::unique_ptr<graph::scene> m_view;
};
}