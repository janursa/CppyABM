CMAKE_MINIMUM_REQUIRED(VERSION 3.13.1)
set(CMAKE_CXX_STANDARD 17)

SET(TARGET "cppython_example")
PROJECT(${TARGET} LANGUAGES CXX)
SET(BINDMODULE "myBinds")

set(LOCAL_FETCH FALSE) # fetching from local directory. Otherwise, it will fetch from github
if (LOCAL_FETCH)
    message(STATUS "FETCHING CPPYABM LOCALLY")
    add_subdirectory(../../../ cppyabm)
    set (cppyabm_SOURCE_DIR "/Users/matin/Downloads/testProjs/CppyABM/")
    pybind11_add_module(${BINDMODULE} binds.cpp)
    target_include_directories(${BINDMODULE} PUBLIC ${cppyabm_SOURCE_DIR}/include)

else()
    FIND_PACKAGE(cppyabm CONFIG)
    FIND_PACKAGE(pybind11 CONFIG)
    if (NOT cppyabm_FOUND OR NOT pybind11_FOUND)
        message(STATUS "Either CppyABM or pybind11 is not installed. Clonning CppyABM.")
        include(FetchContent)
        FetchContent_Declare(
          cppyabm
          GIT_REPOSITORY https://github.com/janursa/CppyABM.git
          GIT_TAG        master
        )
        FetchContent_GetProperties(cppyabm)
        if(NOT cppyabm_POPULATED)
          FetchContent_Populate(cppyabm)
          add_subdirectory(${cppyabm_SOURCE_DIR} ${cppyabm_BINARY_DIR})
        endif()
    endif()

    pybind11_add_module(${BINDMODULE} binds.cpp)
    if (NOT cppyabm_FOUND)
        target_include_directories(${BINDMODULE} PUBLIC ${cppyabm_SOURCE_DIR}/include)
    else()
        target_link_libraries(${BINDMODULE}  PUBLIC cppyabm::cppyabm)
    endif()
endif()