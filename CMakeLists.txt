cmake_minimum_required(VERSION 2.8)

set( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH} )


Find_Package(LLVM REQUIRED)
Find_Package(Clang REQUIRED)
Find_Package(Boost REQUIRED)






set(LLVM_USED_LIBS clangTooling clangBasic clangAST clangASTMatchers clangBasic clangTooling)

add_executable(ast
  parser.cpp
  main.cpp
  )

link_directories(${LLVM_LIBRARY_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

include_directories(${CLANG_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/external)
include_directories(${Boost_INCLUDE_DIRS})

add_definitions(${LLVM_CFLAGS})


message(STATUS "Adding external includes ${PROJECT_SOURCE_DIR}/external")

target_link_libraries(ast ${CLANG_LIBS} ${LLVM_MODULE_LIBS} ${LLVM_LDFLAGS} ${Boost_LIBRARIES} jansson)


ADD_DEFINITIONS("-std=c++11") 
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LLVM_COMPILE_FLAGS} -std=c++11 -g")

if (APPLE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -stdlib=libc++ ")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  -stdlib=libc++" )
endif()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
