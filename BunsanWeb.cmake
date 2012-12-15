include(BunsanCommon)

function(bunsan_web_add_skin target)
    set(srcs)
    foreach(src ${ARGN})
        list(APPEND srcs ${CMAKE_CURRENT_SOURCE_DIR}/${src})
    endforeach()
    set(cpp_target ${CMAKE_CURRENT_BINARY_DIR}/${target}_skin_cpp.cpp)
    add_custom_command(
        OUTPUT ${cpp_target}
        COMMAND cppcms_tmpl_cc
            -o ${cpp_target}
            ${srcs}
        DEPENDS ${srcs})
    add_library(${target} SHARED ${cpp_target})
endfunction()
