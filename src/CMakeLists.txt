set(HEADER_LIST )

set(SOURCE_FILES "env.cpp"
				 "agents.cpp"
				 "patch.cpp"
				 "bind_tools.cpp"
				 "pybases.cpp"
				 "mesh.cpp")

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
add_library(${ABMBASE} STATIC ${SOURCE_FILES})
target_link_libraries(${ABMBASE} PRIVATE pybind11::module)
set_target_properties(${ABMBASE} PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(${ABMBASE} PUBLIC ${BASE_SOURCE_DIR}/include/)

source_group(TREE "${BASE_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
