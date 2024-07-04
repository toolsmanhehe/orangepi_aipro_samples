cmake_minimum_required(VERSION 3.14)
add_executable(code_generator codegen.cpp)

target_compile_options(code_generator PRIVATE -std=c++11)
target_link_libraries(code_generator PRIVATE stdc++)

add_executable(code_generator_single codegen.cpp)
if (DEFINED ENV{MY_REG_OP_H})
    target_compile_definitions(code_generator_single PRIVATE -DSINGLE_CUSTOM_REG_OP="$ENV{MY_REG_OP_H}")
else ()
    target_compile_definitions(code_generator_single PRIVATE -DSINGLE_CUSTOM_REG_OP="${CMAKE_CURRENT_LIST_DIR}/custom_reg_op.h")
endif ()
target_compile_options(code_generator_single PRIVATE -std=c++11)
target_link_libraries(code_generator_single PRIVATE stdc++)

add_custom_command(OUTPUT AUTO_GENERATED_GE_RAW_CUSTOM_SINGLE_OPS
        COMMAND code_generator_single ${CMAKE_CURRENT_LIST_DIR}/auto_generated_ge_raw_custom_ops.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        COMMENT "Generating ge apis"
        DEPENDS code_generator_single
        VERBATIM)

add_custom_target(generate_ge_raw_custom_ops ALL
        DEPENDS AUTO_GENERATED_GE_RAW_CUSTOM_SINGLE_OPS
        VERBATIM)
