


add_executable(${APP} main.cpp )
target_compile_features(${APP} PRIVATE cxx_std_17)
target_link_libraries(${APP} PRIVATE pybind11::module)
target_link_libraries(${APP} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${APP} PRIVATE ${STATICLIBS})
target_include_directories(${APP}  PUBLIC ${CMAKE_SOURCE_DIR}/include)





