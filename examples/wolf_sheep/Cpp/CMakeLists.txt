CMAKE_MINIMUM_REQUIRED(VERSION 3.13.1)
set(CMAKE_CXX_STANDARD 17)

SET(TARGET "cpp_example")
PROJECT(${TARGET} LANGUAGES CXX)
SET(APP "appABM")
# add_executable(${APP} single_run.cpp )
add_executable(${APP} batch_run.cpp )

target_compile_features(${APP} PRIVATE cxx_std_17)
FIND_PACKAGE(cppyabm CONFIG)

find_package(MPI REQUIRED)
if (MPI_FOUND)
    include_directories(SYSTEM ${MPI_INCLUDE_PATH})
else (MPI_FOUND)
    message(SEND_ERROR "couldn't find MPI")
endif (MPI_FOUND)

if (cppyabm_FOUND)
	message(STATUS "CppyABM found")
	target_link_libraries(${APP}  PUBLIC cppyabm::cppyabm)
else()

	message(STATUS "CppyABM is not found.")
  # local
  set(CPPYABM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../")
  message(STATUS "The provided CPPYABM_DIR is ")
  message(STATUS ${CPPYABM_DIR})
  target_include_directories(${APP}  PUBLIC ${CPPYABM_DIR}/include)
  target_link_libraries(${APP}  ${MPI_LIBRARIES})

  # online

 #  message(STATUS "It will be cloned and linked.")
	# SET(CPP TRUE)
 #    include(FetchContent)
 #    FetchContent_Declare(
 #      cppyabm
 #      GIT_REPOSITORY https://github.com/janursa/CppyABM.git
 #      GIT_TAG        master
 #    )
 #    FetchContent_GetProperties(cppyabm)
 #    if(NOT cppyabm_POPULATED)
 #      FetchContent_Populate(cppyabm)
 #      add_subdirectory(${cppyabm_SOURCE_DIR} ${cppyabm_BINARY_DIR})
 #    endif()
 #    target_include_directories(${APP}  PUBLIC ${cppyabm_SOURCE_DIR}/include)

endif()




