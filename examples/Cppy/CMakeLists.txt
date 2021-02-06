CMAKE_MINIMUM_REQUIRED(VERSION 3.13.1)
set(CMAKE_CXX_STANDARD 17)

SET(TARGET "cppython_example")
PROJECT(${TARGET} LANGUAGES CXX)
SET(BINDMODULE "myBinds")
SET(cppyabm_dir ${DIR})
FIND_PACKAGE(pybind11 ${PYBIND11_MIN_VERSION} CONFIG)
if (NOT pybind11_FOUND)
    # include(FetchContent)
    # FetchContent_Declare(
    #   pybind11
    #   GIT_REPOSITORY https://github.com/pybind/pybind11.git
    #   GIT_TAG        master
    # )
    # FetchContent_GetProperties(pybind11)
    # if(NOT pybind11_POPULATED)
    #   FetchContent_Populate(pybind11)
    #   add_subdirectory(${pybind11_SOURCE_DIR} ${pybind11_BINARY_DIR})
    # endif()
    # set(PYBIND11_CPP_STANDARD -std=c++17)
    SET(PYBIND11 TRUE)
    include(FetchContent)
    FetchContent_Declare(
      cppyabm
      GIT_REPOSITORY https://github.com/janursa/CppyABM.git
      GIT_TAG        master
    )
    message(STATUS ${cppyabm_BINARY_DIR})
    FetchContent_GetProperties(cppyabm)
    if(NOT cppyabm_POPULATED)
      FetchContent_Populate(cppyabm)
      add_subdirectory(${cppyabm_SOURCE_DIR} ${cppyabm_BINARY_DIR})
    endif()
endif()



pybind11_add_module(${BINDMODULE} binds.cpp ${HEADER_LIST})
target_include_directories(${BINDMODULE} PUBLIC ${cppyabm_dir})

