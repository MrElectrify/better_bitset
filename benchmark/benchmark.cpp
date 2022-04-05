#include <benchmark/benchmark.h>

#include <better_bitset.hpp>

#include <bitset>
#include <random>
#include <vector>

constexpr float AVG_ONES = 5.f;
constexpr size_t ITERATIONS = 1000;

template<size_t N>
inline size_t find_one(const std::bitset<N>& bitset)
{
    for (size_t i = 0; i < N; ++i)
    {
        if (bitset.test(i) == true)
            return i;
    }
    return N;
}

template<size_t N>
static void BM_Bitset(benchmark::State& state)
{
    // generate
    std::vector<std::bitset<N>> bitsets;
    bitsets.reserve(ITERATIONS);
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::bernoulli_distribution b(AVG_ONES / N);
    for (size_t i = 0; i < ITERATIONS; ++i)
    {
        std::bitset<N> bitset;
        for (size_t j = 0; j < N; ++j)
            bitset.set(j, b(eng));
        bitsets.push_back(bitset);
    }
    for (size_t i = 0; state.KeepRunning() == true; ++i)
    {
        for (size_t j = 0; j < ITERATIONS; ++j)
            benchmark::DoNotOptimize(find_one(bitsets[j]));
    }
}
BENCHMARK_TEMPLATE(BM_Bitset, 4);
BENCHMARK_TEMPLATE(BM_Bitset, 8);
BENCHMARK_TEMPLATE(BM_Bitset, 16);
BENCHMARK_TEMPLATE(BM_Bitset, 32);
BENCHMARK_TEMPLATE(BM_Bitset, 64);
BENCHMARK_TEMPLATE(BM_Bitset, 96);
BENCHMARK_TEMPLATE(BM_Bitset, 128);
BENCHMARK_TEMPLATE(BM_Bitset, 256);
BENCHMARK_TEMPLATE(BM_Bitset, 512);
BENCHMARK_TEMPLATE(BM_Bitset, 1024);
BENCHMARK_TEMPLATE(BM_Bitset, 2048);
BENCHMARK_TEMPLATE(BM_Bitset, 4096);
BENCHMARK_TEMPLATE(BM_Bitset, 8192);

template<size_t N>
static void BM_BBitset(benchmark::State& state)
{
    // generate
    std::vector<better_bitset::BitSet<N>> bitsets;
    bitsets.reserve(ITERATIONS);
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::bernoulli_distribution b(AVG_ONES / N);
    for (size_t i = 0; i < ITERATIONS; ++i)
    {
        better_bitset::BitSet<N> bitset;
        for (size_t j = 0; j < N; ++j)
            bitset.set(j, b(eng));
        bitsets.push_back(bitset);
    }
    for (size_t i = 0; state.KeepRunning() == true; ++i)
    {
        for (size_t j = 0; j < ITERATIONS; ++j)
            benchmark::DoNotOptimize(bitsets[j].first_one());
    }
}
BENCHMARK_TEMPLATE(BM_BBitset, 4);
BENCHMARK_TEMPLATE(BM_BBitset, 8);
BENCHMARK_TEMPLATE(BM_BBitset, 16);
BENCHMARK_TEMPLATE(BM_BBitset, 32);
BENCHMARK_TEMPLATE(BM_BBitset, 64);
BENCHMARK_TEMPLATE(BM_BBitset, 96);
BENCHMARK_TEMPLATE(BM_BBitset, 128);
BENCHMARK_TEMPLATE(BM_BBitset, 256);
BENCHMARK_TEMPLATE(BM_BBitset, 512);
BENCHMARK_TEMPLATE(BM_BBitset, 1024);
BENCHMARK_TEMPLATE(BM_BBitset, 2048);
BENCHMARK_TEMPLATE(BM_BBitset, 4096);
BENCHMARK_TEMPLATE(BM_BBitset, 8192);

BENCHMARK_MAIN();