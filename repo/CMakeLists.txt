CMAKE_MINIMUM_REQUIRED(VERSION 3.13.1)
# for testing
# set(HEADER_LIST "${CMAKE_SOURCE_DIR}/include/CA/agent.h"
# 	"${CMAKE_SOURCE_DIR}/include/CA/mesh.h"
# 	"${CMAKE_SOURCE_DIR}/include/CA/model.h"
# 	"${CMAKE_SOURCE_DIR}/include/CA/patch.h"
# 	"${CMAKE_SOURCE_DIR}/include/CA/settings.h"
# 	"${CMAKE_SOURCE_DIR}/include/CA/toolbox.h")
set(HEADER_LIST "${CMAKE_SOURCE_DIR}/include/CA/frame.h")
add_library(${TARGET}_sharedlib main.cpp ${HEADER_LIST})
target_include_directories(${TARGET}_sharedlib PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${TARGET}_sharedlib PUBLIC pybind11::module)
source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
