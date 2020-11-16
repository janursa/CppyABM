CMAKE_MINIMUM_REQUIRED(VERSION 3.13.1)
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(TARGET "ABM")
PROJECT(${TARGET} VERSION 1.0 LANGUAGES CXX)
SET(ABMLIB "ABM")
SET(APP "appABM")
SET(BINDSMODULE "myBinds")
SET(FUZZYLITE "fuzzylite")
SET(FUZZYLIB "fuzzy")
SET(DIFFUSIONLIB "diffusion")
SET(APPDIFFUSION "appDiffusion")
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
find_package (Python COMPONENTS Interpreter Development)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(pybind11)

include(FetchContent)

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.7.3)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

if (DEALII)
	FIND_PACKAGE(deal.II 9.1.0 REQUIRED
	HINTS ${DEALII_DIR}/build ../ ../../ $ENV{DEALII_DIR}/build
	)
	DEAL_II_INITIALIZE_CACHED_VARIABLES()
	include_directories(${DEALII_DIR}/include)
	add_definitions(-DDEALII)
endif(DEALII)


add_subdirectory(../CPPYABM CPPYABM)
add_subdirectory(src)
#add_subdirectory(app)
add_subdirectory(binds)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTING)
    add_subdirectory(tests)
endif()


