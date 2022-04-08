
#include "better_bitset.hpp"

#include <iostream>

// constexpr tests
void test()
{
    constexpr better_bitset::BitSet<8> a(0b00110101);
    static_assert(a[0] == true);
    static_assert(a[1] == false);
    static_assert(a.test(1) == false);
    static_assert(a.all() == false);
    static_assert(a.any() == true);
    static_assert(a.none() == false);
    static_assert(a.count() == 4);
    static_assert(a.first_one() == 0);
    static_assert(a.first_zero() == 1);
    constexpr better_bitset::BitSet<8> b(0b11111111);
    static_assert(b.all() == true);
    static_assert(b.any() == true);
    static_assert(b.none() == false);
    static_assert(b.count() == 8);
    static_assert(a.first_one() == 0);
    static_assert(b.first_zero() == 8);
    constexpr better_bitset::BitSet<65> c({ 0xffffffffffffffff, 1 });
    static_assert(c.all() == true);
    static_assert(c.any() == true);
    static_assert(c.none() == false);
    static_assert(c.count() == 65);
    static_assert(a.first_one() == 0);
    static_assert(c.first_zero() == 65);
    constexpr better_bitset::BitSet<70> d;
    static_assert(d.all() == false);
    static_assert(d.any() == false);
    static_assert(d.none() == true);
    static_assert(d.count() == 0);
    static_assert(d.first_one() == 70);
    static_assert(d.first_zero() == 0);
    constexpr better_bitset::BitSet<129> e{ {0, 0, 1} };
    static_assert(e.all() == false);
    static_assert(e.any() == true);
    static_assert(e.none() == false);
    static_assert(e.count() == 1);
    static_assert(e.first_one() == 128);
    static_assert(e.first_zero() == 0);
    constexpr better_bitset::BitSet<4> f;
    static_assert(f.first_one() == 4);
}

template <size_t MAX_SIZE, size_t TEST_ID = 0, size_t DEPTH = 0>
void runTest() {
    
    if constexpr ((1ull << DEPTH) < MAX_SIZE) {
        runTest<MAX_SIZE, TEST_ID | (1ull << DEPTH), DEPTH + 1>();
        runTest<MAX_SIZE, TEST_ID, DEPTH + 1>();
    }
    else {
        constexpr size_t SIZE = TEST_ID + 1;
        constexpr size_t numChunks = (SIZE + 63) / 64;
        better_bitset::BitSet<SIZE> bs;
        if (bs.first_one() != SIZE) {
            std::cerr << "bs.first_one()=" << bs.first_one()
                        << ", for empty"
                        << ", size=" << SIZE
                        << ", NUM_CHUNKS=" << numChunks
                        <<  std::endl;
            abort();
        }

        if (bs.first_zero() != 0) {
            std::cerr << "bs.first_zero()=" << bs.first_one()
                        << ", for empty"
                        << ", size=" << SIZE
                        << ", NUM_CHUNKS=" << numChunks
                        <<  std::endl;
            abort();
        }

        bs.flip();

        if (bs.first_one() != 0) {
            std::cerr << "bs.first_one()=" << bs.first_one()
                        << ", for filled"
                        << ", size=" << SIZE
                        << ", NUM_CHUNKS=" << numChunks
                        <<  std::endl;
            abort();
        }

        if (bs.first_zero() != SIZE) {
            std::cerr << "bs.first_zero()=" << bs.first_one()
                        << ", for filled"
                        << ", size=" << SIZE
                        << ", NUM_CHUNKS=" << numChunks
                        <<  std::endl;
            abort();
        }

        for (size_t index = 0; index < SIZE; ++index) {
            better_bitset::BitSet<SIZE> bs;
            bs.set(index, true);
            if (bs.first_one() != index) {
                std::cerr << "bs.first_one()=" << bs.first_one()
                          << ", for index=" << index
                          << ", size=" << SIZE
                          << ", NUM_CHUNKS=" << numChunks
                          <<  std::endl;
                abort();
            }
            bs.flip();
            bs.set(index, false);
            if (bs.first_zero() != index) {
                std::cerr << "bs.first_zero()=" << bs.first_one()
                          << ", for index=" << index
                          << ", size=" << SIZE
                          << ", NUM_CHUNKS=" << numChunks
                          <<  std::endl;
                abort();
            }
        }
    }
}


int main() {

    runTest<1024>();

    return 0;
}