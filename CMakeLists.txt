cmake_minimum_required(VERSION 3.18)
project(hdl)

Set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVL_DEBUG")

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
  /usr/share/verilator/include/verilated_vcd_c.cpp
  cpp/counter/counter.cpp
  cpp/icache/icache.cpp
  cpp/main.cpp
  cpp/RF/RF.cpp
)

target_link_libraries(hdl
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hdl)

# For info: https://verilator.org/guide/latest/verilating.html
# We need one verilate call for each "top level" module we would like to simulate

# These are top level modules
set(sv_counter "sv/counter/counter.sv")
set(sv_RF "sv/RF/RF.sv")
set(sv_icache "sv/icache/icache.sv")

# This is a single list of all top level modules, so we can iterate over it below
set(sv_top_modules ${sv_counter} ${sv_RF} ${sv_icache})

foreach(sv_top ${sv_top_modules})
  verilate(hdl
    INCLUDE_DIRS "sv"
    SOURCES ${sv_top}
    TRACE
  )
endforeach()
