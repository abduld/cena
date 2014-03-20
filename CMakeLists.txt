cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

Find_Package(LLVM REQUIRED)
link_directories( ${LLVM_LIB_DIR} )


Find_Package(Clang REQUIRED)
include_directories(${CLANG_INCLUDE_DIRS})

set(LLVM_LINK_COMPONENTS support)
set(LLVM_USED_LIBS clangTooling clangBasic clangAST clangASTMatchers clangBasic clangTooling)

add_executable(ast
  parser.cpp
  main.cpp
  )

target_link_libraries(ast ${CLANG_LIBS} ${LLVM_LIBS_CORE} ${LLVM_LDFLAGS} jansson)


ADD_DEFINITIONS("-std=c++11") 
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LLVM_COMPILE_FLAGS} -std=c++11 -g")

if (APPLE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -stdlib=libc++ ")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  -stdlib=libc++" )
endif()
