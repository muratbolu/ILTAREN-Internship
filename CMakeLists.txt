cmake_minimum_required (VERSION 3.27..3.30)
project(ILTAREN-Internship
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Main executable
add_executable (main src/main.cpp)
target_include_directories(main PRIVATE include)

# ----- TESTS -----

# Test sources
set(TEST_SOURCES
    test/ContIterTest.cpp
    test/NodeIterTest.cpp
    test/StaticVectorTest.cpp
    test/ObjectPoolTest.cpp
    test/NodeTest.cpp
    test/LinkedListTest.cpp
)

# Statically link Google Test
enable_testing()
add_subdirectory(googletest)
include_directories(${CMAKE_SOURCE_DIR}/googletest/include)

# Test executable
add_executable (tests src/tests.cpp ${TEST_SOURCES})
target_link_libraries(tests gtest)
target_include_directories(tests PRIVATE include)
target_include_directories(tests PRIVATE test)

include(GoogleTest)
gtest_discover_tests(tests)

# Address sanitizer causes the Google Test to not execute at all?
# if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
#     add_compile_definitions(_DISABLE_VECTOR_ANNOTATION) # Apparently needed because of a bug?
#     add_compile_definitions(_DISABLE_STRING_ANNOTATION) # Same reason as above
#     target_compile_options(${PROJECT_NAME} PUBLIC /fsanitize=address)
# endif()
