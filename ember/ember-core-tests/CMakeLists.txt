enable_testing()
include(GoogleTest)
find_package(GTest REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets Network REQUIRED)

set(target_name "ember-core-tests")

set(src_precompiled
	"src/precompiled.h")

file(GLOB_RECURSE src_files "${CMAKE_CURRENT_SOURCE_DIR}/src/*.*")
file(GLOB_RECURSE res_files "${CMAKE_CURRENT_SOURCE_DIR}/res/*.*")

add_executable(${target_name}
	${src_include}
	${src_files} 
	${res_files})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${res_files})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})
source_group("include" FILES ${src_include})

target_link_libraries(${target_name} GTest::Main)
target_link_libraries(${target_name} "ember-core")

target_include_directories(${target_name} PRIVATE "../ember-core/include")
target_include_directories(${target_name} PRIVATE "../ember-core/src")

set_target_properties(${target_name} PROPERTIES FOLDER "ember")
set_target_properties(${target_name} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${target_name} PROPERTIES AUTORCC ON)
set_target_properties(${target_name} PROPERTIES AUTOMOC ON)
set_target_properties(${target_name} PROPERTIES AUTOUIC ON)

target_precompile_headers(${target_name} PUBLIC ${src_precompiled})
conan_install_imports(${target_name})
gtest_discover_tests(${target_name})
