cmake_minimum_required (VERSION 3.27..3.29)
project(static_vector
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    src/main.cpp
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/34ad51b3dc4f922d8ab622491dd44fc2c39afee9.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable (${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)

enable_testing()

add_executable(
  test_static_vector
  test/test_static_vector.cpp
)
target_link_libraries(
  test_static_vector
  GTest::gtest_main
)

target_include_directories(test_static_vector PRIVATE include)

include(GoogleTest)
gtest_discover_tests(test_static_vector)
