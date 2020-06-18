# CMAKE_MINIMUM_REQUIRED(VERSION 3.13.1)
# for testing
set(HEADER_LIST )

set(SOURCE_FILES "env.cpp"
				 "agents.cpp"
				 "patch.cpp"
				 "bind_tools.cpp"
				 "pybases.cpp"
				 "mesh.cpp")

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
add_library(${STATICLIBS} STATIC ${SOURCE_FILES})
target_link_libraries(${STATICLIBS} PRIVATE pybind11::module)
set_target_properties(${STATICLIBS} PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(${STATICLIBS} PUBLIC ../include/)

source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
