# set cmake minimum version
cmake_minimum_required(VERSION 3.20) 

# set the project name
project(ToyLang VERSION 0.1.0)

# C++17
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# LLVM
find_package(LLVM REQUIRED CONFIG)

# LLVM include directories
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# ToyLang Source
file (GLOB_RECURSE SOURCES "src/*.cpp")

# ToyLang Executable
add_executable(${PROJECT_NAME} ${SOURCES} src/Compiler.hpp)

# LLVM Linking
llvm_map_components_to_libnames(llvm_libs support core irreader)
target_link_libraries(${PROJECT_NAME} ${llvm_libs})

# GTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG        release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file (GLOB_RECURSE TESTS "test/*.cpp")

add_executable(ToyLangTests ${TESTS})
target_link_libraries(
  ToyLangTests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(ToyLangTests)