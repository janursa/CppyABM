CMAKE_MINIMUM_REQUIRED(VERSION 3.13.1)
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)


SET(TARGET "ABM")
PROJECT(${TARGET} VERSION 1.0 LANGUAGES CXX)
SET(STATICLIBS "BaseLibs")
SET(APP "app")
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    include(CTest)
    find_package(Doxygen)
    # if(Doxygen_FOUND)
    #     add_subdirectory(docs)
    # else()
    #     message(STATUS "Doxygen not found, not building docs")
    # endif()
endif()


include(FetchContent)
if(${CMAKE_VERSION} VERSION_LESS 3.14)
    include(cmake/add_FetchContent_MakeAvailable.cmake)
endif()

FetchContent_Declare(
  fmtlib
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        5.3.0
)
FetchContent_MakeAvailable(fmtlib)

include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(pybind11)
set(PYBIND11_CPP_STANDARD -std=c++17)


set(JSON_BuildTests OFF CACHE INTERNAL "")
find_package(nlohmann_json REQUIRED)

add_subdirectory(src)
add_subdirectory(binds)
add_subdirectory(app)



