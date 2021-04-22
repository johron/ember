#pragma once
#include <QStackedWidget>
#include <QButtonGroup>
#include <QDialogButtonBox>

namespace ember::user_settings {
struct manager;
struct dialog : public QDialog {
	struct section_button : public QPushButton {
		section_button(QIcon icon, QString name, QWidget* parent)
			: QPushButton(icon, name, parent) {
			setIconSize({32, 32});
			setCheckable(true);
		}
	};

	struct section_view : public QWidget {
		section_view(QWidget* parent)
			: QWidget(parent)
			, m_button_group(new QButtonGroup(this))
			, m_pages_view(new QStackedWidget())
			, m_button_view(new QWidget()) {
			m_button_view->setLayout(new QVBoxLayout());
			m_button_view->layout()->setAlignment(Qt::AlignTop | Qt::AlignLeft);
			m_button_view->layout()->setContentsMargins(0, 0, 0, 0);
			setLayout(new QHBoxLayout());
			layout()->setContentsMargins(0, 0, 0, 0);
			layout()->addWidget(m_button_view);
			layout()->addWidget(m_pages_view);
		}

		void add_section(QIcon icon, QString name, QWidget* content) {
			auto button = new section_button(icon, name, this);
			m_button_group->addButton(button);
			m_button_view->layout()->addWidget(button);
			m_pages_view->addWidget(content);

			QObject::connect(button, &QPushButton::toggled, [content, this](bool toggled) {
				if (toggled) {
					m_pages_view->setCurrentWidget(content);
				}
			});

			if (m_pages_view->count() == 1) {
				button->setChecked(true);
			}
		}

	private:
		QButtonGroup* m_button_group;
		QStackedWidget* m_pages_view;
		QWidget* m_button_view;
	};

	dialog(std::vector<std::unique_ptr<section>>& sections, QWidget* parent)
		: QDialog(parent) {
		setWindowTitle("Preferences");
		setMinimumSize({600, 400});

		auto view = new section_view(this);
		for (auto& section : sections) {
			view->add_section(section->get_icon(), section->get_name(), section->get_widget(this));
		}

		auto buttons = new QDialogButtonBox(QDialogButtonBox::Close);
		QObject::connect(buttons, &QDialogButtonBox::accepted, this, &dialog::accept);
		QObject::connect(buttons, &QDialogButtonBox::rejected, this, &dialog::reject);

		auto content = new QVBoxLayout();
		content->addWidget(view);
		content->addWidget(buttons);
		setLayout(content);
	}

private:
	QWidget* m_left_section;
	QWidget* m_right_section;
};
}