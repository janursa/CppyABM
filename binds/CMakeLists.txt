
pybind11_add_module(${BINDMODULE}  binds.cpp )
target_link_libraries(${BINDMODULE} PRIVATE pybind11::module)

target_include_directories(${BINDMODULE} PUBLIC ${BASE_SOURCE_DIR}/include)
source_group(TREE "${BASE_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
