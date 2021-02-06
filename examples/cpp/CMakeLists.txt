CMAKE_MINIMUM_REQUIRED(VERSION 3.13.1)
set(CMAKE_CXX_STANDARD 17)

SET(TARGET "cpp_example")
PROJECT(${TARGET} LANGUAGES CXX)
SET(APP "appABM")
FIND_PACKAGE(cppyabm ${cppyabm_MIN_VERSION} CONFIG)
if (NOT cppyabm_FOUND)
    # SET(PYBIND11 TRUE)
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
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	add_executable(${APP} cpp_main.cpp )
	target_compile_features(${APP} PRIVATE cxx_std_17)
	target_include_directories(${APP}  PUBLIC ${cppyabm_SOURCE_DIR}/include)
endif()


