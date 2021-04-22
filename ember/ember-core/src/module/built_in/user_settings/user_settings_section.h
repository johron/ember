#pragma once
#include <QFormLayout>

namespace ember::user_settings {
struct section : public QObject {
	Q_OBJECT
public:
	virtual ~section() = default;

	virtual QIcon get_icon() const = 0;
	virtual QString get_name() const = 0;
	virtual QWidget* get_widget(QWidget* parent) = 0;

	void write(QString key, QVariant value) {
		emit request_write(key, value);
	}

	template <class value_t>
	value_t read(QString key) const {
		QVariant data;
		emit request_read(key, data);
		return data.value<value_t>();
	}

signals:
	void request_write(const QString& key, const QVariant& value);
	void request_read(const QString& key, QVariant& value);
};
}  // namespace ember::settings

namespace ember::user_settings::detail {
struct general : section {
	QIcon get_icon() const override {
		return icon::awesome(fa::cog);
	}

	QString get_name() const override {
		return "General";
	}

	QWidget* get_widget(QWidget* parent) override {
		auto layout = new QFormLayout();

		auto widget = new QWidget(parent);
		widget->setLayout(layout);
		return widget;
	}
};
}  // namespace ember::preferences::detail