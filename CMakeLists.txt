cmake_minimum_required(VERSION 2.8)

set( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH} )


Find_Package(LLVM REQUIRED)
Find_Package(Clang REQUIRED)
Find_Package(ZLIB REQUIRED)

include(symbolic/sources.cmake)

add_executable(ast
  parser.cpp
  main.cpp
  # external/backward_cpp/backward.cpp
  ${PROJECT_HPP}
)

link_directories(${LLVM_LIBRARY_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

include_directories(${CLANG_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/external)

add_definitions(${LLVM_CFLAGS})

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "clang")
  add_definitions(-fno-exceptions)
  add_definitions(-Wall)
endif()
add_definitions(-Wno-unused-function)
add_definitions(-Wfatal-errors)
add_definitions(-g)
add_definitions(-O0)


target_link_libraries(ast ${ZLIB_LIBRARIES} -lncurses ${CLANG_LIBS} ${LLVM_LDFLAGS} ${LLVM_MODULE_LIBS} )
target_link_libraries(ast pthread)
target_link_libraries(ast curses)
target_link_libraries(ast z)
target_link_libraries(ast clang)

# ADD_DEFINITIONS("-std=c++11")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LLVM_COMPILE_FLAGS} -std=c++11 ")

if (APPLE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ ")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++" )
endif()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
