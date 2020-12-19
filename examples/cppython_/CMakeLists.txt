CMAKE_MINIMUM_REQUIRED(VERSION 3.13.1)
set(CMAKE_CXX_STANDARD 17)

SET(TARGET "cppython")
PROJECT(${TARGET} VERSION 1.0 LANGUAGES CXX)
SET(ABMLIB "ABM")
SET(APP "appABM")
SET(ABMBASE "ABMBASE")
SET(BINDMODULE "myBinds")
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    include(CTest)
endif()
## fetch
include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(pybind11)
set(PYBIND11_CPP_STANDARD -std=c++17)
# add CPPYABM
add_subdirectory(../../ CPPYABM)
# create abm lib
set(SOURCE_FILES "example.cpp")
add_library(${ABMLIB} STATIC ${SOURCE_FILES})
target_link_libraries(${ABMLIB} PRIVATE ABMBASE)
target_link_libraries(${ABMLIB} PRIVATE pybind11::module)
target_include_directories(${ABMLIB} PUBLIC ${CMAKE_SOURCE_DIR}/../../..)
set_target_properties(${ABMLIB} PROPERTIES POSITION_INDEPENDENT_CODE ON)
# binds
pybind11_add_module(${BINDMODULE} binds.cpp ${HEADER_LIST})
target_link_libraries(${BINDMODULE} PRIVATE pybind11::module)
# target_include_directories(${BINDMODULE} PUBLIC ${BASE_SOURCE_DIR}/include)
target_link_libraries(${BINDMODULE} PRIVATE ${ABMBASE})
target_link_libraries(${BINDMODULE} PRIVATE ${ABMLIB})



