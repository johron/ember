macro(conan_cmake_init)
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
                  "${CMAKE_BINARY_DIR}/conan.cmake"
                  TLS_VERIFY ON)
  endif()

  include(${CMAKE_BINARY_DIR}/conan.cmake)
endmacro()

macro(conan_install_imports cmake_target)
  if (EXISTS ${CMAKE_BINARY_DIR}/conan_paths.cmake)
    message(STATUS "nova: install conan imports for ${cmake_target}")
    add_custom_command(TARGET ${cmake_target} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/bin $<TARGET_FILE_DIR:${cmake_target}>)
  endif()
endmacro()

conan_cmake_init()
if (NOT EXISTS ${CMAKE_BINARY_DIR}/conan_paths.cmake)
  conan_cmake_autodetect(settings)
  conan_add_remote(NAME bincrafters 
                  URL "https://api.bintray.com/conan/bincrafters/public-conan"
                  VERIFY_SSL True)   
  conan_cmake_configure(
    REQUIRES 
      "gtest/1.8.1@bincrafters/stable"
      "qt/5.15.2@bincrafters/stable"
    GENERATORS "cmake_paths"
    BUILD_REQUIRES "cmake/3.15.7"
    IMPORTS "bin, *.dll -> ./bin"
    IMPORTS "lib, *.dylib* -> ./bin"
    IMPORTS "plugins/platforms, *.dll -> ./bin/platforms"
    OPTIONS qt:qttools=True)


  conan_cmake_install(
    PATH_OR_REFERENCE .
    SETTINGS ${settings}
    BUILD "missing"
    OUTPUT_QUIET)
endif()

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)



