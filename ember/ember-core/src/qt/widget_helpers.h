#pragma once
#include "../util/dynamic_value.h"
#include "../icon/impl_font_awesome.h"
#include "widget_detail.h"
#include <QLabel>

namespace ember::qt {
inline auto make_label(const QString& text, uint64_t font_size = 11, QWidget* parent = nullptr) {
	auto label = new QLabel(text, parent);
	auto font = label->font();
	font.setPointSize(font_size);
	label->setFont(font);
	return label;
}

inline auto make_label(const QPixmap& pixmap, QWidget* parent = nullptr) {
	auto label = new QLabel(parent);
	label->setPixmap(pixmap);
	return label;
}

inline auto make_icon_button(QIcon icon, QWidget* parent) {
	auto button = new QPushButton(icon, "", parent);
	button->setObjectName("IconButton");
	return button;
}

template <class value_t, std::enable_if_t<std::is_arithmetic_v<value_t> && !std::is_same_v<value_t, bool>, int> = 0>
inline auto make_widget(util::dynamic_value<value_t>& value,
						value_t min = std::numeric_limits<value_t>::min(),
						value_t max = std::numeric_limits<value_t>::max(),
						QWidget* parent = nullptr) {
	return new detail::numeric_view<value_t>(value, min, max, parent);
}

template <class value_t, std::enable_if_t<std::is_arithmetic_v<value_t> && !std::is_same_v<value_t, bool>, int> = 0>
inline auto make_widget(util::dynamic_value<value_t>& value, QWidget* parent) {
	return make_widget(value, std::numeric_limits<value_t>::min(), std::numeric_limits<value_t>::max(), parent);
}

template <class value_t, std::enable_if_t<std::is_same_v<value_t, bool>, int> = 0>
inline auto make_widget(util::dynamic_value<value_t>& value, QWidget* parent) {
	return new detail::boolean_view(value, parent);
}

template <class value_t, std::enable_if_t<std::is_same_v<value_t, QString>, int> = 0>
inline auto make_widget(util::dynamic_value<value_t>& value, const QStringList& options, QWidget* parent) {
	return new detail::string_view(value, options, parent);
}

}  // namespace ember::qt