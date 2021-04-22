#pragma once

namespace ember::qt {
struct graphics_view_zoom : public QObject {
	graphics_view_zoom(QGraphicsView* view)
		: QObject(view)
		, m_view(view) {
		m_view->viewport()->installEventFilter(this);
		m_view->setMouseTracking(true);
		m_modifiers = Qt::ControlModifier;
		m_zoom_factor = 1.0015f;
	}

	void gentle_zoom(double factor) {
		m_view->scale(factor, factor);
		m_view->centerOn(m_target_scene);

		const QPointF delta_viewport_pos =
			m_target_viewport - QPointF(m_view->viewport()->width() / 2.0, m_view->viewport()->height() / 2.0);
		const QPointF viewport_center = m_view->mapFromScene(m_target_scene) - delta_viewport_pos;
		m_view->centerOn(m_view->mapToScene(viewport_center.toPoint()));
		// emit zoomed();
	}

	bool eventFilter(QObject* /*watched*/, QEvent* event) {
		if (event->type() == QEvent::MouseMove) {
			QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
			QPointF delta = m_target_viewport - mouse_event->pos();
			if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5) {
				m_target_viewport = mouse_event->pos();
				m_target_scene = m_view->mapToScene(mouse_event->pos());
			}
		} else if (event->type() == QEvent::Wheel) {
			QWheelEvent* wheel_event = static_cast<QWheelEvent*>(event);
			if (QApplication::keyboardModifiers() & m_modifiers) {
				if (wheel_event->orientation() == Qt::Vertical) {
					double angle = wheel_event->angleDelta().y();
					double factor = qPow(m_zoom_factor, angle);
					gentle_zoom(factor);
					return true;
				}
			}
		}
		return false;
	}

private:
	QGraphicsView* m_view;
	QPointF m_target_viewport;
	QPointF m_target_scene;
	Qt::KeyboardModifiers m_modifiers;
	float m_zoom_factor;
};
}  // namespace ember::qt

/*
Graphics_view_zoom::Graphics_view_zoom(QGraphicsView* view)
	: QObject(view)
	, _view(view) {
	_view->viewport()->installEventFilter(this);
	_view->setMouseTracking(true);
	_modifiers = Qt::ControlModifier;
	_zoom_factor_base = 1.0015;
}

void Graphics_view_zoom::gentle_zoom(double factor) {
	_view->scale(factor, factor);
	_view->centerOn(target_scene_pos);
	QPointF delta_viewport_pos =
		target_viewport_pos - QPointF(_view->viewport()->width() / 2.0, _view->viewport()->height() / 2.0);
	QPointF viewport_center = _view->mapFromScene(target_scene_pos) - delta_viewport_pos;
	_view->centerOn(_view->mapToScene(viewport_center.toPoint()));
	emit zoomed();
}

void Graphics_view_zoom::set_modifiers(Qt::KeyboardModifiers modifiers) {
	_modifiers = modifiers;
}

void Graphics_view_zoom::set_zoom_factor_base(double value) {
	_zoom_factor_base = value;
}

bool Graphics_view_zoom::eventFilter(QObject* object, QEvent* event) {
	if (event->type() == QEvent::MouseMove) {
		QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
		QPointF delta = target_viewport_pos - mouse_event->pos();
		if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5) {
			target_viewport_pos = mouse_event->pos();
			target_scene_pos = _view->mapToScene(mouse_event->pos());
		}
	} else if (event->type() == QEvent::Wheel) {
		QWheelEvent* wheel_event = static_cast<QWheelEvent*>(event);
		if (QApplication::keyboardModifiers() == _modifiers) {
			if (wheel_event->orientation() == Qt::Vertical) {
				double angle = wheel_event->angleDelta().y();
				double factor = qPow(_zoom_factor_base, angle);
				gentle_zoom(factor);
				return true;
			}
		}
	}
	Q_UNUSED(object)
	return false;
}
*/