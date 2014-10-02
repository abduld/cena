

Project (SymbolicAST)

cmake_minimum_required(VERSION 2.5 FATAL_ERROR)
Set(CMAKE_BUILD_TYPE Debug)

Set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
Set(CMAKE_VERBOSE_MAKEFILE ON)

Set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake/modules)

include(EnableCXX11)
include(VcMacros)

enable_testing()


set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  add_definitions(-fno-exceptions)
  add_definitions(-Wall)
endif()
add_definitions(-Wfatal-errors)

set(GTEST_SOURCES
    "../external/gtest/gtest-all.cc"
)
set(GTEST_INCLUDE_DIR
    "../external"
)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${GTEST_INCLUDE_DIR}
)


get_filename_component(CurrentDirectory "${CMAKE_CURRENT_LIST_FILE}" PATH)
MESSAGE( STATUS "Current directory = ${CurrentDirectory}")

file(GLOB THESE_TEST_FILES
    ${CurrentDirectory}/*_test.cpp
)

file(GLOB THESE_HPP_FILES
    ${CurrentDirectory}/*.hpp
)

set_source_files_properties(
    ${SRC_FILES}
    PROPERTIES FOLDER ${CurrentDirectory}
)

set_source_files_properties(
    ${THESE_HPP_FILES}
    PROPERTIES HEADER_FILE_ONLY ON
               FOLDER ${CurrentDirectory}
)

list(APPEND SymbolicAST_HPP ${THESE_HPP_FILES})
list(APPEND TEST_SOURCES ${THESE_TEST_FILES})

add_executable(ast_test ${TEST_SOURCES} ${GTEST_SOURCES})
target_link_libraries(ast_test pthread)
add_test( ast_test runUnitTests )
