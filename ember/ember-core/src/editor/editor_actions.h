#pragma once
namespace ember {
template <class resource_t>
struct resource_manager : public util::disable_move_and_copy {
	virtual void resource_new() = 0;
	virtual void resource_load(QString filename) = 0;
	virtual void resource_save(QString filename) = 0;
	virtual bool has_changes() const = 0;
};

struct resource_action : public QAction {
	template <class resource_t>
	resource_action(resource_manager<resource_t>& manager, QString name, QWidget* parent)
		: QAction(name, parent)
		, m_confirm_close([&manager, parent]() {
			if (manager.has_changes()) {
				const auto title = "Unsaved changes";
				const auto message = "There are unsaved changes, would you like to save before exiting?";
				const auto result = QMessageBox::question(
					parent, title, message, QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
				if (result == QMessageBox::Save) {
					manager.resource_save("");
					return true;
				} else if (result == QMessageBox::Discard) {
					return true;
				} else {
					return false;
				}
			} else {
				return true;
			}
		}) {}

protected:
	std::function<bool()> m_confirm_close;
};

struct action_new : public resource_action {
	template <class resource_t>
	action_new(resource_manager<resource_t>& manager, QWidget* parent)
		: resource_action(manager, "new", parent) {
		QObject::connect(this, &QAction::triggered, [&manager, parent]() {
			if (m_confirm_close()) {
				manager.resource_new();
			}
		});
	}
};

struct action_save : public QAction {};

/*
	struct save_level_as {
	static void apply(QWidget* parent, editor::app_core& editor) {
		const auto filename = QFileDialog::getSaveFileName(parent, "Save level", QString{}, "*.json");
		if (!filename.isEmpty()) {
			editor.level_save(filename);
		}
	}
};

struct save_level {
	static void apply(QWidget* parent, editor::app_core& editor) {
		const auto& level = editor.get_level();
		const auto filename = level.get_filename();
		if (filename.isEmpty()) {
			save_level_as::apply(parent, editor);
		} else {
			editor.level_save(filename);
		}
	}
};

struct confirm_exit {
	static bool apply(QWidget* parent, editor::app_core& editor) {
		if (editor.get_level().has_changes()) {
			const auto result =
				QMessageBox::question(parent,
									  "Unsaved changes",
									  "There are unsaved changes, would you like to save before exiting?",
									  QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
			switch (result) {
				case QMessageBox::Save:
					save_level::apply(parent, editor);
					return true;
				case QMessageBox::Discard:
					return true;
				case QMessageBox::Cancel:
				default:
					return false;
			}
		} else {
			return true;
		}
	}
};

struct load_level {
	static void apply(QWidget* parent, editor::app_core& editor) {
		if (confirm_exit::apply(parent, editor)) {
			const auto filename = QFileDialog::getOpenFileName(parent, "Load level", QString{}, "*.json");
			if (!filename.isEmpty()) {
				editor.level_load(filename);
			}
		}
	}
};

struct new_level {
	static void apply(QWidget* parent, editor::app_core& editor) {
		if (confirm_exit::apply(parent, editor)) {
			editor.level_new();
		}
	}
};
*/
}  // namespace ember