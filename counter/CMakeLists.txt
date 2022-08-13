cmake_minimum_required(VERSION 3.8)
project(counter)

find_package(verilator)
#find_package(verilator HINTS $ENV{VERILATOR_ROOT} ${VERILATOR_ROOT})
if (NOT verilator_FOUND)
	message(FATAL_ERROR "Verilator not found")
endif()

add_executable(counter main.cpp)

verilate(counter
	INCLUDE_DIRS "."
	SOURCES counter.sv
	)
