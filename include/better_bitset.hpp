/// @file better_bitset.hpp
/// @brief The better bitset

#ifndef BETTER_BITSET_H_
#define BETTER_BITSET_H_

// STL includes
#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <numeric>
#include <string>
#include <type_traits>

#ifdef _DEBUG
#define BITSET_ASSERT(x) assert(x)
#else
#define BITSET_ASSERT(x) (void)(x)
#endif

namespace better_bitset
{

    /// @brief A proper bitset that supports scanning
    template<size_t N> requires (N > 0)
        class BitSet
    {
    private:
        /// @brief The inner stored data type
        using Inner_t = std::conditional_t<(N > 32), uint64_t,
            std::conditional_t<(N > 16), uint32_t,
            std::conditional_t<(N > 8), uint16_t, uint8_t>>>;
        /// @brief The mask of the last bit
        constexpr static size_t LAST_MASK = ~(~1ull << ((N - 1ull) % (64ull)));
        /// @brief The number of chunks stored
        constexpr static size_t NUM_CHUNKS = (N + 63) / 64;
        /// @brief The storage type. Bits are stored from LSB to MSB
        /// and populate lower order storage positions first
        using Storage_t = std::array<Inner_t, NUM_CHUNKS>;
    public:
        constexpr BitSet() noexcept : m_storage() {}
        /// @param storage The storage
        constexpr BitSet(Storage_t storage) noexcept :
            m_storage(storage)
        {
            BITSET_ASSERT(std::bit_width(storage[NUM_CHUNKS - 1]) <= N);
            m_storage[NUM_CHUNKS - 1] &= LAST_MASK;
        }
        /// @param storage The storage
        constexpr BitSet(Inner_t storage) noexcept requires(N <= 64) :
            BitSet(Storage_t{ storage }) {}

        /* ACCESSORS */

        /// @return True if all of the bits are set to 1
        constexpr bool all() const noexcept
        {
            // make sure they are all maximum value up until the last storage,
            // and that the last storage is the mask of the bit size
            return std::all_of(m_storage.begin(), std::prev(m_storage.end()),
                [](Inner_t val) { return val == std::numeric_limits<Inner_t>::max(); }) &&
                m_storage[NUM_CHUNKS - 1] == LAST_MASK;
        }
        /// @return True if any of the bits are set to 1
        constexpr bool any() const noexcept
        {
            return std::any_of(m_storage.begin(), m_storage.end(),
                [](Inner_t val) { return val != 0; });
        }
        /// @return True if all of the bits are set to 0
        constexpr bool none() const noexcept
        {
            // make sure they are all 0
            return std::all_of(m_storage.begin(), m_storage.end(),
                [](Inner_t val) { return val == 0; });
        }
        /// @return The number of 1 bits
        constexpr size_t count() const noexcept
        {
            return std::accumulate(m_storage.begin(), m_storage.end(), size_t(0),
                [](size_t acc, Inner_t val) { return acc + std::popcount(val); });
        }
        /// @return The position of the first one in the bitset
        constexpr size_t first_one() const noexcept
        {
            size_t pos = 0;
            for (size_t i = 0; i < NUM_CHUNKS; ++i)
            {
                size_t chunk_pos = std::countr_zero(m_storage[i]);
                pos += chunk_pos;
                if (chunk_pos != sizeof(Inner_t) * 8)
                    return pos;
            }
            return N;
        }
        /// @return The position of the first zero in the bitset
        constexpr size_t first_zero() const noexcept
        {
            size_t pos = 0;
            for (size_t i = 0; i < NUM_CHUNKS; ++i)
            {
                size_t chunk_pos = std::countr_one(m_storage[i]);
                pos += chunk_pos;
                if (chunk_pos != sizeof(Inner_t) * 8)
                    return pos;
            }
            return pos;
        }
        /// @brief Tests the bit at a an index. Does not perform a bounds
        /// check in release
        /// @param pos The bit position
        /// @return The bit's value
        constexpr bool test(size_t pos) const noexcept
        {
            return (*this)[pos];
        }

        /* CAPACITY */

        constexpr size_t size() const noexcept { return N; }

        /* MODIFIERS */

        /// @brief Sets all bits to true
        BitSet& set() noexcept
        {
            for (size_t i = 0; i < NUM_CHUNKS - 1; ++i)
                m_storage[i] = std::numeric_limits<Inner_t>::max();
            m_storage[NUM_CHUNKS - 1] = LAST_MASK;
            return *this;
        }
        /// @brief Sets the bit as pos to value
        BitSet& set(size_t pos, bool value = true) noexcept
        {
            BITSET_ASSERT(pos < N);
            if (value == false)
                return reset(pos);
            if constexpr (N > 64)
            {
                const size_t chunk = pos / 64;
                const size_t shift = pos % 64;
                m_storage[chunk] |= 1ull << shift;
                return *this;
            }
            m_storage[0] |= static_cast<Inner_t>(1ull << pos);
            return *this;
        }
        /// @brief Flips all bits
        BitSet& flip() noexcept
        {
            for (size_t i = 0; i < NUM_CHUNKS - 1; ++i)
                m_storage[i] = ~m_storage[i];
            m_storage[NUM_CHUNKS - 1] = static_cast<Inner_t>(
                ~static_cast<size_t>(m_storage[NUM_CHUNKS - 1]) & LAST_MASK);
            return *this;
        }
        /// @brief Sets all bits to false
        BitSet& reset() noexcept
        {
            for (Inner_t& chunk : m_storage)
                chunk = 0;
            return *this;
        }
        /// @brief Sets the bit at pos to 0
        BitSet& reset(size_t pos) noexcept
        {
            BITSET_ASSERT(pos < N);
            if constexpr (N > 64)
            {
                const size_t chunk = pos / 64;
                const size_t shift = pos % 64;
                if (chunk == NUM_CHUNKS - 1)
                    m_storage[chunk] &= ~(1ull << shift) & LAST_MASK;
                else
                    m_storage[chunk] &= ~(1ull << shift);
                return *this;
            }
            m_storage[0] &= ~(1ull << pos) & LAST_MASK;
            return *this;
        }

        /* CONVERSIONS */

        /// @return The bitset as a string
        std::string to_string() const noexcept
        {
            std::string result;
            for (size_t i = N; i > 0; --i)
                result += (test(i - 1) ? '1' : '0');
            return result;
        }

        /* OPERATORS*/

        /// @brief Fetches the bit at a an index. Does not perform a bounds
        /// check in release
        /// @param pos The bit position
        /// @return The bit's value
        constexpr bool operator[](size_t pos) const
        {
            BITSET_ASSERT(pos < N);
            if constexpr (N > 64)
            {
                const size_t chunk = pos / 64;
                const size_t shift = pos % 64;
                return static_cast<bool>(m_storage[chunk] >> shift);
            }
            return static_cast<bool>((m_storage[0] >> pos) & 0x1);
        }

        constexpr bool operator==(const BitSet<N>& other) const noexcept
        {
            return m_storage == other.m_storage;
        }
        constexpr bool operator==(Storage_t storage) const noexcept requires(N > 64)
        {
            return m_storage == storage;
        }
        constexpr bool operator==(Inner_t storage) const noexcept requires(N <= 64)
        {
            return m_storage[0] == storage;
        }
    private:
        /// @brief The internal value
        Storage_t m_storage;
    };
}

#endif
