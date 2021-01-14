CMAKE_MINIMUM_REQUIRED(VERSION 3.13.1)
set(CMAKE_CXX_STANDARD 17)

SET(TARGET "cpp_example")
PROJECT(${TARGET} LANGUAGES CXX)
SET(APP "appABM")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../ CPPYABM)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	add_executable(${APP} cpp_main.cpp )
	target_compile_features(${APP} PRIVATE cxx_std_17)
	target_link_libraries(${APP} PRIVATE ${CPPLIB})
	target_include_directories(${APP}  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
	target_include_directories(${APP} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
endif()


