CMAKE_MINIMUM_REQUIRED(VERSION 3.13.1)
FetchContent_Declare(
catch2
GIT_REPOSITORY https://github.com/catchorg/Catch2.git
GIT_TAG        v2.9.1
)
message(STATUS "Running tests")
set(cppyabm_SOURCE_DIR  "/Users/matin/Downloads/testProjs/CppyABM/")
FetchContent_MakeAvailable(catch2)

add_executable(testApp test_ABM.cpp)

target_compile_features(testApp PRIVATE cxx_std_17)

target_link_libraries(testApp PRIVATE Catch2::Catch2)

# add_subdirectory(${cppyabm_SOURCE_DIR} ${cppyabm_BINARY_DIR})

target_include_directories(testApp  PUBLIC ${cppyabm_SOURCE_DIR}/include)
