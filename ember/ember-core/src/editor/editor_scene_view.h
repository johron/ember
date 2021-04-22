#pragma once
#include <QGraphicsView>
#include <qmath.h>


namespace ember {
namespace msg {
struct view_context_menu {
	QMenu& m_menu;
};
}
struct editor_scene_view : public QGraphicsView {
	static const QString name;
	editor_scene_view(QWidget* parent)
		: QGraphicsView(parent) {
		setObjectName(name);
		setMouseTracking(true);
		setRenderHint(QPainter::Antialiasing);
	}

	void enable_selection(bool enabled) {
		if (enabled) {
			setDragMode(QGraphicsView::RubberBandDrag);
		} else {
			setDragMode(QGraphicsView::NoDrag);
		}
	}

	void wheelEvent(QWheelEvent* event) override {
		if (event->modifiers() & Qt::ControlModifier) {
			const double angle = event->angleDelta().y();
			const double factor = qPow(1.0008, angle);
			const auto target_viewport_pos = event->position();
			const auto target_scene_pos = mapToScene(target_viewport_pos.x(), target_viewport_pos.y());

			scale(factor, factor);
			centerOn(target_scene_pos);
			QPointF delta = target_viewport_pos - QPointF(viewport()->width() / 2.0, viewport()->height() / 2.0);
			QPointF viewport_center = mapFromScene(target_scene_pos) - delta;
			centerOn(mapToScene(viewport_center.toPoint()));
			event->accept();
		} else {
			QGraphicsView::wheelEvent(event);
		}
	}

	void contextMenuEvent(QContextMenuEvent* event) override {
		QMenu menu(this);
		message::post(msg::view_context_menu{menu});
		menu.addAction("Reset zoom", [this]() { resetTransform(); });
		menu.exec(event->globalPos());
	}
};

inline const QString editor_scene_view::name = "editor.view";
}  // namespace ember::board