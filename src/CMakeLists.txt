# CMAKE_MINIMUM_REQUIRED(VERSION 3.13.1)
# for testing
set(HEADER_LIST )

SET(MODULE_NAME "binds")

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

pybind11_add_module(${MODULE_NAME} binds.cpp ${HEADER_LIST})
target_link_libraries(${MODULE_NAME} PRIVATE pybind11::module)
target_link_libraries(${MODULE_NAME} PRIVATE fmt::fmt)
target_include_directories(${MODULE_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include/ABM)
target_include_directories(${MODULE_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src/)

source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
