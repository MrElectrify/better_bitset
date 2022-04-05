# better_bitset
A better `std::bitset` that supports scanning for bits optimally

## Purpose
`std::bitset` has been neglected as of late. A common use case of a bitset in the game development world is to track slots in a server, claiming the
lowest slot. The current API of `std::bitset` does not provide a good way to bitscan for zeros (empty slots) in contexts above 64-bits (for 64 slots
and below, you can conveniently use `std::bitset::to_ullong` and `std::countr_ones`). In my case, I needed 256-bit sets, and chaining together bitsets
is hacky at best.

## Enter `better_bitset`
`better_bitset` is actually smaller than `std::bitset` for sizes 16 bits and below, so if you have a bunch of small bitsets, this can save a few bytes.
The biggest change is the addition of `first_zero` and `first_one` in the bitset. It uses `std::countr_ones` and `std::countr_zeros` underneath, which
is optimized for x86 targets to extensions of the `BSF` instruction such as `TZCNT`, resulting in improvements averaging 60x versus the iterative
`std::bitset` approach as tested in the benchmarks folder.

## Compatibility
`better_bitset` is not quite a drop-in replacement. I only implemented stuff that I needed and generally thought others may need. The usual suspects
like `all`, `any`, `none`, `count`, `size`, `set`, `reset`, `flip`, and `to_string` are there, but stuff such as bitwise operators and references are
missing. If you need them, they can be trivially implemented
