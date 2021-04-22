#pragma once
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QWheelEvent>


namespace ember::qt::detail {
namespace helper {

template <class numeric_t, bool integral = std::is_integral_v<numeric_t>>
struct spinbox {};

template <class numeric_t>
struct spinbox<numeric_t, true> {
	typedef QSpinBox type;
};

template <class numeric_t>
struct spinbox<numeric_t, false> {
	typedef QDoubleSpinBox type;
};
}  // namespace helper

template <class value_t, std::enable_if_t<std::is_arithmetic_v<value_t>, int> = 0>
struct numeric_view : public helper::spinbox<value_t>::type {
	using spinbox_t = typename helper::spinbox<value_t>::type;
	numeric_view(util::dynamic_value<value_t>& value,
				 value_t min = std::numeric_limits<value_t>::min(),
				 value_t max = std::numeric_limits<value_t>::max(),
				 QWidget* parent = nullptr)
		: spinbox_t(parent) {
		spinbox_t::setRange(min, max);
		spinbox_t::setValue(value);
		spinbox_t::setFocusPolicy(Qt::StrongFocus);
		m_connections.emplace_back(
			this, QOverload<int>::of(&spinbox_t::valueChanged), &value, &util::dynamic_value<value_t>::set);
		m_connections.emplace_back(value.observe([this](const auto& new_value) {
			QSignalBlocker block(this);
			spinbox_t::setValue(new_value);
		}));
	}

	void wheelEvent(QWheelEvent* event) override {
		if (spinbox_t::hasFocus()) {
			spinbox_t::wheelEvent(event);
		} else {
			event->ignore();
		}
	}

private:
	qt::connections m_connections;
};

struct boolean_view : public QCheckBox {
	boolean_view(util::dynamic_value<bool>& value, QWidget* parent)
		: QCheckBox(parent) {
		setChecked(value);
		m_connections.emplace_back(this, &QCheckBox::stateChanged, [&value](int state) { value.set(state != 0); });
		m_connections.emplace_back(value.observe([this](const auto& new_value) {
			QSignalBlocker block(this);
			setChecked(new_value);
		}));
	}

private:
	qt::connections m_connections;
};

struct string_view : public QComboBox {
	string_view(util::dynamic_value<QString>& value, const QStringList& options, QWidget* parent)
		: QComboBox(parent) 
		, m_options(options) {
		setFocusPolicy(Qt::StrongFocus);
		addItems(m_options);
		setCurrentIndex(m_options.indexOf(value));
		m_connections.emplace_back(this, &QComboBox::currentTextChanged, &value, &util::dynamic_value<QString>::set);

		m_connections.emplace_back(value.observe([this](const auto& new_value) {
			QSignalBlocker block(this);
			setCurrentIndex(m_options.indexOf(new_value));
		}));
	}

private:
	QStringList m_options;
	qt::connections m_connections;
};

}  // namespace ember::qt::detail