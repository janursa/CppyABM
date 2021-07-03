CMAKE_MINIMUM_REQUIRED(VERSION 3.13.1)
set(CMAKE_CXX_STANDARD 17)

SET(TARGET "cppython_example")
PROJECT(${TARGET} LANGUAGES CXX)
SET(BINDMODULE "myBinds")

FIND_PACKAGE(cppyabm CONFIG)
FIND_PACKAGE(pybind11 CONFIG)
if (NOT pybind11_FOUND) #inlude pybind11
    message(STATUS "Clonning pybind11")
    include(FetchContent)
    FetchContent_Declare(
      pybind11
      GIT_REPOSITORY https://github.com/pybind/pybind11.git
      GIT_TAG        master
    )
    FetchContent_GetProperties(pybind11)
    if(NOT pybind11_POPULATED)
      FetchContent_Populate(pybind11)
      add_subdirectory(${pybind11_SOURCE_DIR} ${pybind11_BINARY_DIR})
    endif()
    set(PYBIND11_CPP_STANDARD -std=c++17)
endif()
# add_subdirectory(../../ cppyabm)
if (NOT cppyabm_FOUND)
    set(cppyabm_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../")
endif()

pybind11_add_module(${BINDMODULE} binds.cpp)
if (NOT cppyabm_FOUND)
    target_include_directories(${BINDMODULE} PUBLIC ${cppyabm_SOURCE_DIR}/include)
else()
    target_link_libraries(${BINDMODULE}  PUBLIC cppyabm::cppyabm)
endif()