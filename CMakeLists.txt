CMAKE_MINIMUM_REQUIRED(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)
SET(TARGET "CppyABM")
SET(BINDMODULE "binds")
PROJECT(${TARGET} LANGUAGES CXX)
SET(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    include(CTest)
    find_package(Doxygen)
    if(Doxygen_FOUND)
        add_subdirectory(doxygen)
    else()
        message(STATUS "Doxygen not found, not building docs")
    endif()
    
endif()

if (${PYBIND11}) #inlude pybind11
    message(STATUS "inside pybind11 cppyabm")
    include(FetchContent)
    FIND_PACKAGE(pybind11 ${PYBIND11_MIN_VERSION} CONFIG)
    if (NOT pybind11_FOUND)
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
    endif()
    set(PYBIND11_CPP_STANDARD -std=c++17)
endif()

if (${BIND_FLAG}) #generates the python library
    if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    	add_subdirectory(cppyabm)
    endif()
endif()

INSTALL (
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h*")



