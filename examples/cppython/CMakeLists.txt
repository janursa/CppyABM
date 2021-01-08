CMAKE_MINIMUM_REQUIRED(VERSION 3.13.1)
set(CMAKE_CXX_STANDARD 17)

SET(TARGET "cppython_example")
PROJECT(${TARGET} LANGUAGES CXX)
SET(BINDMODULE "myBinds")

add_subdirectory(../cpp/ CPPABM)

pybind11_add_module(${BINDMODULE} mixed_binds.cpp ${HEADER_LIST})
target_include_directories(${BINDMODULE} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../)

