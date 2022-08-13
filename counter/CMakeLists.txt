cmake_minimum_required(VERSION 3.8)
project(counter)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

#include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

find_package(verilator)
#find_package(verilator HINTS $ENV{VERILATOR_ROOT} ${VERILATOR_ROOT})
if (NOT verilator_FOUND)
	message(FATAL_ERROR "Verilator not found")
endif()

add_executable(counter main.cpp)

target_link_libraries(counter
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(counter)

verilate(counter
	INCLUDE_DIRS "."
	SOURCES counter.sv
	)
