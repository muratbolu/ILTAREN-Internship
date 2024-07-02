cmake_minimum_required (VERSION 3.27..3.29)
project(static_vector
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    src/main.cpp
    test/IterTest.cpp
    test/StaticVectorTest.cpp
    test/ObjectPoolTest.cpp
)

# Statically link Google Test
enable_testing()
add_subdirectory(googletest)
include_directories(${CMAKE_SOURCE_DIR}/googletest/include)

# Main executable
add_executable (${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} gtest)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME} PRIVATE test)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

# Address sanitizer causes the Google Test to not execute at all?
# if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
#     add_compile_definitions(_DISABLE_VECTOR_ANNOTATION) # Apparently needed because of a bug?
#     add_compile_definitions(_DISABLE_STRING_ANNOTATION) # Same reason as above
#     target_compile_options(${PROJECT_NAME} PUBLIC /fsanitize=address)
# endif()
