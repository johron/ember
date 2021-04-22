set(target_name "ember-core")

set(src_precompiled
	"src/precompiled/precompiled_std.h"
	"src/precompiled/precompiled_qt.h"
    "src/precompiled/precompiled.h")

set(src_include
	"include/ember/ember-core.h")

file(GLOB_RECURSE src_files "${CMAKE_CURRENT_SOURCE_DIR}/src/*.*")
file(GLOB_RECURSE res_files "${CMAKE_CURRENT_SOURCE_DIR}/res/*.*")

add_library(${target_name} STATIC
	${src_include}
	${src_files} 
	${res_files})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${res_files})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})
source_group("include" FILES ${src_include})

if (DEFINED EMBER_BUILD_INTERNAL_RELEASE)
	target_compile_definitions(${target_name} PUBLIC EMBER_BUILD_TYPE_INTERNAL_RELEASE)
elseif(DEFINED EMBER_BUILD_EXTERNAL_RELEASE)
	target_compile_definitions(${target_name} PUBLIC EMBER_BUILD_TYPE_EXTERNAL_RELEASE)
else()
	target_compile_definitions(${target_name} PUBLIC EMBER_BUILD_TYPE_DEVELOPMENT)
endif()

target_compile_definitions(${target_name} PUBLIC 
	EMBER_VERSION_MAJOR=${ember_VERSION_MAJOR}
	EMBER_VERSION_MINOR=${ember_VERSION_MINOR}
	EMBER_VERSION_PATCH=${ember_VERSION_PATCH})

find_package(Qt5 COMPONENTS Core Widgets Network REQUIRED)

target_link_libraries(${target_name} PUBLIC "compiler_warnings")
target_link_libraries(${target_name} PUBLIC "compiler_features")
target_link_libraries(${target_name} PUBLIC "sanitizers")
target_link_libraries(${target_name} PUBLIC Qt5::Core)
target_link_libraries(${target_name} PUBLIC Qt5::Widgets)
target_link_libraries(${target_name} PUBLIC Qt5::Network)

set_target_properties(${target_name} PROPERTIES FOLDER "ember")
set_target_properties(${target_name} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${target_name} PROPERTIES AUTORCC ON)
set_target_properties(${target_name} PROPERTIES AUTOMOC ON)
set_target_properties(${target_name} PROPERTIES AUTOUIC ON)

target_precompile_headers(${target_name} PUBLIC ${src_precompiled})