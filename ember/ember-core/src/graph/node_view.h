#pragma once
#include <QGraphicsRectItem>
#include <QGraphicsItemGroup>
#include "node_model.h"

namespace ember::graph::view {


struct rouned_rect_item : public QGraphicsRectItem {
	rouned_rect_item(qreal x, qreal y, qreal w, qreal h, QString name, QGraphicsItem* parent)
		: QGraphicsRectItem(x, y, w, h, parent) 
		, m_name(name) {
		setFlag(QGraphicsItem::ItemIsSelectable);
		setFlag(QGraphicsItem::ItemIsMovable);
	}

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget = nullptr */) override {
		const auto bounds = boundingRect();
		painter->save();
		painter->setPen(QPen(Qt::black, 2));

		QLinearGradient grad(boundingRect().topLeft(), {boundingRect().left(), 28});
		grad.setColorAt(0.0, QColor(30, 30, 30));
		grad.setColorAt(0.8, QColor(30, 30, 30));
		grad.setColorAt(0.80001, QColor(80, 80, 80));

		painter->setBrush(grad);
		painter->drawRoundedRect(bounds, 4, 4);
		painter->setPen(Qt::white);
		painter->drawText(0, 0, bounds.width(), 22, Qt::AlignCenter, m_name);

		painter->restore();
	}

	QString m_name;
};

struct node_pin : public QGraphicsRectItem {
	node_pin(QGraphicsItem* parent)
		: QGraphicsRectItem(0, 0, 10, 10, parent) {
		setAcceptHoverEvents(true);
		setBrush(Qt::darkGreen);
	}

	void hoverEnterEvent(QGraphicsSceneHoverEvent* /*event*/) override {
		setBrush(Qt::green);
	}
	void hoverLeaveEvent(QGraphicsSceneHoverEvent* /*event*/) override {
		setBrush(Qt::darkGreen);
	}
};

struct input_pin_item : public node_pin {
	input_pin_item(input_pin& pin, QGraphicsItem* parent)
		: node_pin(parent) {
		auto text = new QGraphicsSimpleTextItem(pin.m_name, this);
 		text->setPos(12, -5);
	}
};

struct dynamic_pin_item : public node_pin {
	dynamic_pin_item(QGraphicsItem* parent)
		: node_pin(parent) {
		auto text = new QGraphicsSimpleTextItem("+", this);
		text->setPos(12, -5);
		setPen(QPen(Qt::PenStyle::DotLine));
	}
};

struct output_pin_item : public node_pin {
	output_pin_item(output_pin& pin, QGraphicsItem* parent)
		: node_pin(parent) {
		auto text = new QGraphicsSimpleTextItem(pin.m_name, this);
		text->setPos(-text->boundingRect().width()-3, -5);
	}
};

struct node : public rouned_rect_item {
	node(graph::node& node, QGraphicsRectItem* parent)
		: rouned_rect_item(0, 0, 150, 70, node.m_name, parent)
		, m_node(node) {

		setRect(0, 0, 150, std::max(node.m_input.size(), node.m_output.size()) * 20 + 30);

		for (auto i=0; i<node.m_input.size(); ++i) {
			const auto pin = new input_pin_item(node.m_input[i], this);
			pin->setPos(-6, 30 + i * 20);
		}

		for (auto i = 0; i < node.m_output.size(); ++i) {
			const auto pin = new output_pin_item(node.m_output[i], this);
			pin->setPos(boundingRect().width() - 6, 30 + i * 20);
		}
	}

private:
	graph::node& m_node;
};
}  // namespace ember::graph::view