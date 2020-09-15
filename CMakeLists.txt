CMAKE_MINIMUM_REQUIRED(VERSION 3.13.1)
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)


SET(TARGET "CPPYABM")
PROJECT(${TARGET} VERSION 1.0 LANGUAGES CXX)
SET(ABMBASE "ABMBASE")
SET(BASE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(BINDMODULE "CppyABM")
SET(APP "testapp")
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    include(CTest)
    find_package(Doxygen)
    if(Doxygen_FOUND)
        add_subdirectory(docs)
    else()
        message(STATUS "Doxygen not found, not building docs")
    endif()
    include(FetchContent)
    if(${CMAKE_VERSION} VERSION_LESS 3.14)
        include(cmake/add_FetchContent_MakeAvailable.cmake)
    endif()

    include(FetchContent)
    FetchContent_Declare(
      pybind11
      GIT_REPOSITORY https://github.com/pybind/pybind11.git
      GIT_TAG        master
    )
    FetchContent_MakeAvailable(pybind11)
    set(PYBIND11_CPP_STANDARD -std=c++17)
    
	if (DEALII)
	    FIND_PACKAGE(deal.II 9.1.0 REQUIRED
	    HINTS ${DEALII_DIR}/build ../ ../../ $ENV{DEALII_DIR}/build
	    )
	    DEAL_II_INITIALIZE_CACHED_VARIABLES()
	    include_directories(${DEALII_DIR}/include)
	    add_definitions(-DDEALII)
	endif(DEALII)
	if (VTK)

	    FIND_PACKAGE(VTK REQUIRED COMPONENTS 
	          vtkCommonColor
	          vtkCommonComputationalGeometry
	          vtkCommonCore
	          vtkCommonDataModel
	          vtkCommonExecutionModel
	          vtkCommonMath
	          vtkCommonMisc
	          vtkCommonSystem
	          vtkCommonTransforms
	          vtkFiltersCore
	          vtkFiltersGeneral
	          vtkFiltersSources
	          vtkImagingCore
	          vtkInteractionStyle
	          vtkRenderingCore
	          vtkkwiml
	          vtkInteractionStyle
	          QUIET
	    HINTS ${VTK_DIR}/build ../ ../../ $ENV{VTK_DIR}/build
	    )
	    #include_directories(${VTK_INCLUDE_DIRS})
	    message (STATUS "VTK_VERSION: ${VTK_VERSION}")
	    # this definitin is to act as a flag inside c++ code
	    add_definitions(-DVTK)
	endif(VTK)
endif()

# include(FetchContent)
# FetchContent_Declare(
#     dealii
#     GIT_REPOSITORY https://github.com/dealii/dealii.git
#     GIT_TAG        master
# )
# FetchContent_MakeAvailable(dealii)



# set(JSON_BuildTests OFF CACHE INTERNAL "")
# find_package(nlohmann_json REQUIRED)

add_subdirectory(src)
add_subdirectory(binds)
add_subdirectory(app)



