cmake_minimum_required (VERSION 3.27..3.30)
project(ILTAREN-Internship
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Do not link C++ Standard Library
unset(CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES)
unset(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES)
set(CMAKE_CXX_STANDARD_LIBRARIES "")

# Disable exceptions
if(MSVC)
    string(REGEX REPLACE "/GR" "/GR-" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REGEX REPLACE "/EHsc" "/EHs-c-" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")
endif()

# City traveler executable
add_executable (traveler src/traveler.cpp)
target_include_directories(traveler PRIVATE include)

# Bus schedule generator executable
add_executable (generator src/generator.cpp)
target_include_directories(generator PRIVATE include)

# Bus schedule analyzer executable
add_executable (analyzer src/analyzer.cpp)
target_include_directories(analyzer PRIVATE include)

# ----- TESTS -----

# Test sources
set(TEST_SOURCES
    test/ContIterTest.cpp
    test/NodeIterTest.cpp
    test/StaticVectorTest.cpp
    test/ObjectPoolTest.cpp
    test/NodeTest.cpp
    test/LinkedListTest.cpp
    test/MaybeTest.cpp
    test/BusTest.cpp
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
