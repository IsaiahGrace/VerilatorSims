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
  GIT_TAG main
)
FetchContent_MakeAvailable(googletest)
enable_testing()

include_directories(cpp)

add_executable(hdl
  /usr/share/verilator/include/verilated_vcd_c.cpp
  cpp/counter.cpp
  cpp/icache.cpp
  cpp/main.cpp
  cpp/register_file.cpp
)

target_link_libraries(hdl PUBLIC
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hdl)

# For info: https://verilator.org/guide/latest/verilating.html
# We need one verilate call for each "top level" module we would like to simulate

# These are top level modules for simulation
set(sv_counter       "sv/counter.sv")
set(sv_icache        "sv/icache.sv")
set(sv_register_file "sv/register_file.sv")
set(sv_tileIO_top    "sv/tileIO_top.sv" "sv/tileIO.sv")
#set(sv_tile_top     "sv/tile_top.sv" "sv/tile.sv" ${sv_icache} ${sv_register_file} ${sv_tileIO})

# This is a single list of all top level modules, so we can iterate over it below
set(sv_top_modules sv_counter sv_register_file sv_icache sv_tileIO_top)

foreach(sv_top ${sv_top_modules})
  verilate(hdl
    INCLUDE_DIRS "sv/inc"
    SOURCES ${${sv_top}}
    TRACE
    VERILATOR_ARGS "--Wall"
  )
endforeach()
