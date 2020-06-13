# CMAKE_MINIMUM_REQUIRED(VERSION 3.13.1)
# for testing
set(HEADER_LIST )


set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

add_library(${STATICLIBS} STATIC env.cpp ${HEADER_LIST})
target_link_libraries(${STATICLIBS} PRIVATE pybind11::module)
target_link_libraries(${STATICLIBS} PRIVATE fmt::fmt)
target_include_directories(${STATICLIBS} PUBLIC ${CMAKE_SOURCE_DIR}/include/ABM)

source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
