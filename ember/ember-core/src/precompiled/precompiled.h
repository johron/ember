#pragma once


#include "../util/abstract_storage.h"
#include "../util/algorithm_util.h"
#include "../util/allocator_util.h"
#include "../util/file_util.h"
#include "../util/dynamic_base.h"
#include "../util/dynamic_value.h"
#include "../util/log_framework.h"
#include "../util/message_manager.h"
#include "../util/object_selection.h"
#include "../util/resource_util.h"
#include "../util/scoped_qrc_resource.h"
#include "../util/uncopyable.h"
#include "../util/variant_storage.h"
#include "../util/settings_storage.h"
#include "../util/singleton.h"
#include "../util/vector.h"

#include "../qt/button_types.h"
#include "../qt/connection.h"
#include "../qt/flow_layout.h"
#include "../qt/widget_helpers.h"

#include "../widgets/dockable.h"
#include "../widgets/toolbar.h"
#include "../widgets/icon.h"
#include "../widgets/label.h"

#include "../component/component_object.h"
#include "../component/component_container.h"

#include "../module/module_base.h"
#include "../module/module_registry.h"

#include "../editor/editor_messages.h"
#include "../editor/editor_application.h"
#include "../editor/editor_window.h"

#include "../module/built_in/network/network_manager.h"

#include "../module/built_in/user_settings/user_settings_manager.h"
#include "../module/built_in/user_settings/user_settings_dialog.h"

#include "../module/built_in/inspector/inspector_manager.h"
#include "../module/built_in/inspector/inspector_dock.h"

#include "../module/built_in/undo_stack/undo_stack.h"
#include "../module/built_in/undo_stack/undo_dock.h"

#include "../build/build_config.h"
#include "../build/build_info.h"


#include "../icon/impl_font_awesome.h"

#include "../editor/editor_actions.h"
#include "../editor/editor_main.h"