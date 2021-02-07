CMAKE_MINIMUM_REQUIRED(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)
SET(version 1.0.17)
project("cppyabm" VERSION ${version}
             DESCRIPTION "An agent-based library to integrate C++ and Python"
             HOMEPAGE_URL "https://github.com/janursa/CppyABM")
include(GNUInstallDirs)
SET(BINDMODULE "binds")
SET(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
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

if (${PYBIND11}) #inlude pybind11
    message(STATUS "inside pybind11 cppyabm")
    include(FetchContent)
    FIND_PACKAGE(pybind11 ${PYBIND11_MIN_VERSION} CONFIG)
    if (NOT pybind11_FOUND)
        include(FetchContent)
        FetchContent_Declare(
          pybind11
          GIT_REPOSITORY https://github.com/pybind/pybind11.git
          GIT_TAG        master
        )
        FetchContent_GetProperties(pybind11)
        if(NOT pybind11_POPULATED)
          FetchContent_Populate(pybind11)
          add_subdirectory(${pybind11_SOURCE_DIR} ${pybind11_BINARY_DIR})
        endif()
    endif()
    set(PYBIND11_CPP_STANDARD -std=c++17)
endif()

if (${BIND_FLAG}) #generates the python library
    if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    	add_subdirectory(cppyabm)
    endif()
endif()

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(
  ${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY SameMajorVersion)
configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION
  ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)
install(EXPORT ${PROJECT_NAME}_Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)
install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
              "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)

# INSTALL (
#     DIRECTORY ${CMAKE_SOURCE_DIR}/include/
#     DESTINATION include
#     FILES_MATCHING PATTERN "*.h*")
# INSTALL (
#     DIRECTORY ${CMAKE_SOURCE_DIR}/src/cppyabm
#     DESTINATION lib)
# INSTALL(FILES cppyabmConfig.cmake DESTINATION lib/cmake/cppyabm)




