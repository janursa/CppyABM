CMAKE_MINIMUM_REQUIRED(VERSION 3.13.1)
set(CMAKE_CXX_STANDARD 17)

SET(TARGET "cpp_example")
PROJECT(${TARGET} LANGUAGES CXX)
SET(APP "appABM")

SET(cppyabm_dir ${DIR})

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	add_executable(${APP} cpp_main.cpp )
	target_compile_features(${APP} PRIVATE cxx_std_17)
	target_include_directories(${APP}  PUBLIC ${cppyabm_dir})
endif()


