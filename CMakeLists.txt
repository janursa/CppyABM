CMAKE_MINIMUM_REQUIRED(VERSION 3.13.1)
set(CMAKE_CXX_STANDARD 17)
SET(TARGET "CppyABM")
SET(BINDMODULE "binds")
PROJECT(${TARGET} LANGUAGES CXX)
SET(BASE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
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
include(FetchContent)
if(${CMAKE_VERSION} VERSION_LESS 3.14)
    include(cmake/add_FetchContent_MakeAvailable.cmake)
endif()

include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(pybind11)
set(PYBIND11_CPP_STANDARD -std=c++17)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	add_subdirectory(cppyabm)
endif()




