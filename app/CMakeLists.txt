


add_executable(${APP} main.cpp )
target_compile_features(${APP} PRIVATE cxx_std_17)
#target_link_libraries(${APP} PRIVATE ${ABMBASE})
#target_include_directories(${APP}  PUBLIC ${BASE_SOURCE_DIR}/include/ABM)
#target_include_directories(${APP}  PUBLIC ${DEALII_INCLUDE_DIR})
target_link_libraries(${APP}  pybind11::module)
if (DEALII)
	DEAL_II_SETUP_TARGET(${APP})
endif(DEALII)



