cmake_minimum_required(VERSION 3.8)
project(hdl)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(verilator REQUIRED)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)
enable_testing()

include_directories(cpp)

add_executable(hdl
  cpp/counter/counter.cpp
  cpp/main.cpp
)

target_link_libraries(hdl
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hdl)

set(verilog_sources
  sv/counter/counter.sv
  )

# For info: https://verilator.org/guide/latest/verilating.html
verilate(hdl
  SOURCES ${verilog_sources}
)
