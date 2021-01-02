CMAKE_MINIMUM_REQUIRED(VERSION 3.13.1)
set(CMAKE_CXX_STANDARD 17)

SET(TARGET "cppython")
PROJECT(${TARGET} LANGUAGES CXX)
SET(ABMLIB "ABM")
SET(BINDMODULE "myBinds")
# add CPPYABM
add_subdirectory(../../ CppYABM)
# create abm lib
set(SOURCE_FILES "mixed_example.cpp")
add_library(${ABMLIB} STATIC ${SOURCE_FILES})
target_include_directories(${ABMLIB} PUBLIC ${CMAKE_SOURCE_DIR}/../../..)
set_target_properties(${ABMLIB} PROPERTIES POSITION_INDEPENDENT_CODE ON)
# binds
pybind11_add_module(${BINDMODULE} mixed_binds.cpp ${HEADER_LIST})
target_link_libraries(${BINDMODULE} PRIVATE ${ABMLIB})



