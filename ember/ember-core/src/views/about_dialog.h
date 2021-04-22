#pragma once

namespace ember::view {
struct about_dialog : public QDialog {
	struct header : public QWidget {
		header(QWidget* parent)
			: QWidget(parent) {
			setLayout(new QHBoxLayout());
			layout()->setMargin(0);
			layout()->addWidget(qt::make_label(res::icon::app_logo_with_name(400, 400), this));
		}
	};

	struct versions : public QWidget {
		versions(const ember::build_info& build_info, QWidget* parent)
			: QWidget(parent) {
			setLayout(new QVBoxLayout());
			layout()->setMargin(2);
			layout()->setSpacing(0);
			layout()->addWidget(qt::make_label("Versions", 14, this));

			auto version_form = new QFormLayout();
			version_form->setMargin(0);
			version_form->addRow("nova-core", qt::make_label(ember::build::version::number().toString(), this));
			version_form->addRow(build_info.m_name, qt::make_label(build_info.m_version.toString(), this));
			auto version_content = new QWidget(this);
			version_content->setLayout(version_form);
			layout()->addWidget(version_content);
		}
	};

	about_dialog(const ember::build_info& build_info, QWidget* parent = nullptr)
		: QDialog(parent) {
		setWindowTitle("About");
		setLayout(new QVBoxLayout());
		layout()->addWidget(new header(this));
		layout()->addWidget(new versions(build_info, this));
	}
};
}  // namespace ember::view