project(bunsan_web)

include(${CMAKE_SOURCE_DIR}/system-config.cmake OPTIONAL)

cmake_minimum_required(VERSION 2.8)

include(BunsanCommon)

bunsan_use_bunsan(common)
bunsan_use_boost(system filesystem serialization thread)
bunsan_use(cppcms)

bunsan_add_shared_library(${PROJECT_NAME}
    src/mime_file.cpp
)

bunsan_install_headers()
bunsan_install_targets(${PROJECT_NAME})
bunsan_install_plugins(BunsanCmake)

bunsan_include_tests()
