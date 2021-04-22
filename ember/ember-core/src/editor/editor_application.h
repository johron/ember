#pragma once
#include <QUndoCommand>
#include "../graph/graph_editor.h"

namespace ember::editor {
struct application : public util::disable_move_and_copy{
	application(std::unique_ptr<component::container> container);
	~application();

	void object_create();
	void object_load(QString filename);
	void object_save(QString filename);

	template <class module_t>
	module_t& get() { 
		return m_container->resolve<module_t>();
	}

	template <class module_t>
	const module_t& get() const {
		return m_container->resolve<module_t>();
	}

	template <class command_t, class... arg_t>
	void apply(arg_t&&... args) {
		apply(new command_t(std::forward<arg_t>(args)...));
	}

	void apply(QUndoCommand* command) {
		m_undo_stack.push(command);
	}

	const auto& get_undo_stack() const {
		return m_undo_stack;
	}

private:
	std::unique_ptr<component::container> m_container;
	std::unique_ptr<graph::editor> m_editor;
	QUndoStack m_undo_stack;
};
}  // namespace ember