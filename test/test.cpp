
#include "better_bitset.hpp"

#include <iostream>


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