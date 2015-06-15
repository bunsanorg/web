macro(bunsan_web_find_cppcms_tmpl_cc)
    find_program(CPPCMS_TMPL_CC cppcms_tmpl_cc)
endmacro()

function(bunsan_web_add_skin target skin)
    bunsan_web_find_cppcms_tmpl_cc()
    set(srcs)
    foreach(src ${ARGN})
        list(APPEND srcs ${CMAKE_CURRENT_SOURCE_DIR}/${src})
    endforeach()
    set(cpp_target ${CMAKE_CURRENT_BINARY_DIR}/${target}_skin_cpp.cpp)
    add_custom_command(
        OUTPUT ${cpp_target}
        COMMAND ${CPPCMS_TMPL_CC}
            -o ${cpp_target}
            ${srcs}
        DEPENDS ${srcs})
    bunsan_add_module_library(${target} ${cpp_target})
    set_target_properties(${target} PROPERTIES
                          OUTPUT_NAME ${skin})
endfunction()

function(bunsan_web_install_skins)
    bunsan_install_targets_to(${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/skins
        ${ARGN}
    )
endfunction()

function(bunsan_web_install_static)
    install(DIRECTORY static DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME})
endfunction()
