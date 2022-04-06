cmake_minimum_required(VERSION 3.14)

project(better-bitset
	VERSION 0.1
	DESCRIPTION "Better bitset"
	LANGUAGES CXX)

option(BB_BENCH "Build better_bitset benchmark" OFF)
option(BB_TEST "Build better_bitset tests" OFF)

add_library(better_bitset INTERFACE)

target_include_directories(better_bitset INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_features(better_bitset INTERFACE cxx_std_20)

if (BB_BENCH)
	find_package(benchmark REQUIRED)
	add_subdirectory(benchmark)
endif()
if (BB_TEST)
	add_subdirectory(test)
endif()