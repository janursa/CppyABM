CMAKE_MINIMUM_REQUIRED(VERSION 3.13.1)
set(CMAKE_CXX_STANDARD 17)

SET(TARGET "purely_cpp")
PROJECT(${TARGET} LANGUAGES CXX)
SET(ABMLIB "ABM")
SET(APP "appABM")

# add CPPYABM
add_subdirectory(../../ CPPYABM)
# create abm lib
set(SOURCE_FILES "cpp_example.cpp")
add_library(${ABMLIB} STATIC ${SOURCE_FILES})
target_include_directories(${ABMLIB} PUBLIC ${CMAKE_SOURCE_DIR}/../../..)
set_target_properties(${ABMLIB} PROPERTIES POSITION_INDEPENDENT_CODE ON)
# app
add_executable(${APP} cpp_main.cpp )
target_compile_features(${APP} PRIVATE cxx_std_17)
target_link_libraries(${APP} PRIVATE ${ABMLIB})
target_include_directories(${APP}  PUBLIC ${CMAKE_SOURCE_DIR}/include)




