#pragma once
#include <QIconEngine>
#include <QPainter>
#include <array>

namespace ember::icon {
class icon_font_engine : public QIconEngine {
public:
	icon_font_engine(QFont font, QChar text, std::array<QColor, 4> colors)
		: m_font(font)
		, m_text(text)
		, m_colors(colors) {}

	icon_font_engine* clone() const override {
		return new icon_font_engine(m_font, m_text, m_colors);
	}

	void paint(QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State /*state*/) override {
		painter->save();
		painter->setPen(m_colors[mode]);
		m_font.setPixelSize(rect.height() * 0.9f);
		painter->setFont(m_font);
		painter->drawText(rect, m_text, QTextOption(Qt::AlignCenter | Qt::AlignVCenter));
		painter->restore();
	}

	QPixmap pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state) override {
		QPixmap pixmap(size);
		pixmap.fill(Qt::transparent);  // we need transparency
		QPainter painter(&pixmap);
		paint(&painter, QRect(QPoint(0, 0), size), mode, state);
		return pixmap;
	}

private:
	QFont m_font;
	QChar m_text;
	std::array<QColor, 4> m_colors;
};
}  // namespace ember::icon
