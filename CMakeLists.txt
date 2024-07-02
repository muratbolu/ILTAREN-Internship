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
  static_vector_test
  test/static_vector_test.cpp
)
target_link_libraries(
  static_vector_test
  GTest::gtest_main
)

target_include_directories(static_vector_test PRIVATE include)

include(GoogleTest)
gtest_discover_tests(static_vector_test)
