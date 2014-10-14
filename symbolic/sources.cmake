
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

list(APPEND PROJECT_HPP ${THESE_HPP_FILES})
list(APPEND TEST_SOURCES ${THESE_TEST_FILES})