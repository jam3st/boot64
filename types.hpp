#pragma once

namespace Gx {
    typedef unsigned int size_t;
    static_assert(sizeof(size_t) == 4, "Expecting 32 bits for size_t");

    typedef signed int ssize_t;
    static_assert(sizeof(ssize_t) == 4, "Expecting 32 bits for ssize_t");

    typedef signed char int8_t;
    static_assert(sizeof(int8_t) == 1, "Expecting 8 bits for int8_t");
    typedef signed short int16_t;
    static_assert(sizeof(int16_t) == 2, "Expecting 16 bits for int16_t");
    typedef signed int int32_t;
    static_assert(sizeof(int32_t) == 4, "Expecting 32 bits for int32_t");
    typedef signed long long int64_t;
    static_assert(sizeof(int64_t) == 8, "Expecting 16 bits for int64_t");

    typedef unsigned char uint8_t;
    static_assert(sizeof(uint8_t) == 1, "Expecting 8 bits for uint8_t");
    typedef unsigned short uint16_t;
    static_assert(sizeof(uint16_t) == 2, "Expecting 16 bits for uint16_t");
    typedef unsigned int uint32_t;
    static_assert(sizeof(uint32_t) == 4, "Expecting 32 bits for uint32_t");
    typedef unsigned long long uint64_t;
    static_assert(sizeof(uint64_t) == 8, "Expecting 16 bits for uint64_t");
}
