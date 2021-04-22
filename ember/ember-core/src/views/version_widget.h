#pragma once

namespace ember::view {
struct version_widget : public QWidget {
	version_widget(build::target target, QVersionNumber version, QWidget* parent)
		: QWidget(parent) {
		setLayout(new QHBoxLayout());
		layout()->setContentsMargins(0, 0, 0, 0);
		layout()->addWidget(new widget::icon(get_icon(target), {20, 20}, this));
		layout()->addWidget(new widget::label(version.toString(), 11, this));
		layout()->addItem(new QSpacerItem(4, 0));
		setToolTip(get_text(target, version));
		setStyleSheet("QWidget { background: transparent }");
	}

	fa::icon get_icon(build::target target) {
		if (target == ember::build::target::development) {
			return fa::flask;
		} else if (target == ember::build::target::internal_release) {
			return fa::home;
		} else if (target == ember::build::target::external_release) {
			return fa::cloud;
		} else {
			return fa::questioncircle;
		}
	}

	QString get_text(build::target target, QVersionNumber version) {
		if (target == ember::build::target::development) {
			return "[development] version: " + version.toString();
		} else if (target == ember::build::target::internal_release) {
			return "[internal release] version: " + version.toString();
		} else if (target == ember::build::target::external_release) {
			return "[external release] version: " + version.toString();
		} else {
			throw std::runtime_error("Unknown build type - please update supported builds");
		}
	}

};
}