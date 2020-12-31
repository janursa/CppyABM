# CMAKE_MINIMUM_REQUIRED(VERSION 3.13.1)
# for testing
set(HEADER_LIST )

pybind11_add_module(${BINDMODULE} binds.cpp ${HEADER_LIST})
target_link_libraries(${BINDMODULE} PRIVATE pybind11::module)

target_include_directories(${BINDMODULE} PUBLIC ${BASE_SOURCE_DIR}/include)
source_group(TREE "${BASE_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
