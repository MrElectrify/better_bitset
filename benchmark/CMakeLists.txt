add_executable(benchmark benchmark.cpp)

target_link_libraries(benchmark PRIVATE better_bitset
	benchmark::benchmark benchmark::benchmark_main)
