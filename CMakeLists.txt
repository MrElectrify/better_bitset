cmake_minimum_required(VERSION 3.14)

project(BetterBitset
	VERSION 0.1
	DESCRIPTION "Better bitset"
	LANGUAGES CXX)

add_library(better_bitset INTERFACE)

target_include_directories(better_bitset INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/include)
