cmake_minimum_required(VERSION 3.18)
project(hdl)

Set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG main
)
FetchContent_MakeAvailable(googletest)
enable_testing()

find_package(verilator REQUIRED)

include_directories(cpp)

add_executable(hdl
  /usr/share/verilator/include/verilated_vcd_c.cpp
  cpp/counter.cpp
  cpp/icache.cpp
  cpp/main.cpp
  cpp/register_file.cpp
  cpp/tileIO.cpp
  cpp/type_asserter.cpp
)

target_compile_options(hdl PRIVATE
  -Werror
  -Wall
  -Wextra
  -DVL_DEBUG
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
set(sv_tile          "sv/tile.sv")
set(sv_tileIO        "sv/tileIO.sv")
set(sv_type_asserter "sv/type_asserter.sv")

# Top level wrappers around interfaces (because verilator does not support top level interfaces)
set(sv_tile_top   "sv/tile_top.sv"   ${sv/tile.sv} ${sv_icache} ${sv_register_file} ${sv_tileIO})
set(sv_tileIO_top "sv/tileIO_top.sv" ${sv_tileIO})

# This is a single list of all top level modules, so we can iterate over it below.
# NOTE: This is a list of 'list names'. It is a 1D list of variable names, not a 2D list.
set(sv_top_modules
  sv_counter
  sv_icache
  sv_register_file
  sv_tileIO_top
  sv_type_asserter
  )

foreach(sv_top ${sv_top_modules})
  verilate(hdl
    INCLUDE_DIRS "sv/inc"
    SOURCES ${${sv_top}}
    TRACE
    VERILATOR_ARGS "--Wall" "--assert"
  )
endforeach()

# For debugging: print out all cmake variables and their values
# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
