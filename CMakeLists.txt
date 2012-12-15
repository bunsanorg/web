project(bunsan_web)

include(${CMAKE_SOURCE_DIR}/system-config.cmake OPTIONAL)

cmake_minimum_required(VERSION 2.8)

include(${CMAKE_SOURCE_DIR}/BunsanWeb.cmake)

bunsan_use_boost(system filesystem serialization unit_test_framework thread)
bunsan_use(cppcms)

bunsan_add_shared_library(${PROJECT_NAME}
    src/stub.cpp
)

bunsan_install_headers()
bunsan_install_targets(${PROJECT_NAME})
install(FILES BunsanWeb.cmake DESTINATION ${CMAKE_ROOT}/Modules)

bunsan_include_tests()
