cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${CMAKE_SOURCE_DIR}/cmake/modules"
)


execute_process(
  COMMAND llvm-config-3.5 --cxxflags
  OUTPUT_VARIABLE LibClang_DEFINITIONS)

SET(GCC_COVERAGE_COMPILE_FLAGS  "-fno-rtti -std=c++11")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})
add_definitions(${LibClang_DEFINITIONS})

execute_process(
  COMMAND llvm-config-3.5 --includedir
  OUTPUT_VARIABLE LibClang_INCLUDE_DIR)

execute_process(
  COMMAND llvm-config-3.5 --ldflags
  OUTPUT_VARIABLE LibClang_LIB_DIR)

include_directories(
  ${LibClang_INCLUDE_DIR}
)

link_directories(
  ${LibClang_LIBRARIES_DIR}
)

add_executable(ast main.cpp parser.cpp)
target_link_libraries(ast
  ${LibClang_LIBRARIES}
  clangAST
  clangBasic
  clangDriver
  clangFrontend
  clangRewriteFrontend
  clangStaticAnalyzerFrontend
  clangTooling
)

