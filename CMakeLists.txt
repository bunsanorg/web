cmake_minimum_required(VERSION 3.0)

include(${CMAKE_SOURCE_DIR}/system-config.cmake OPTIONAL)

project(bunsan_web)

find_package(BunsanCMake CONFIG REQUIRED)

bunsan_add_library(${PROJECT_NAME}
    src/mime_file.cpp
)
bunsan_use_bunsan_package(${PROJECT_NAME} bunsan_common bunsan_common)

bunsan_install_headers()
bunsan_install_targets(${PROJECT_NAME})
bunsan_install_module(NAME BunsanWeb
                      DIRECTORIES scripts/)
bunsan_install_project()

bunsan_include_tests()
