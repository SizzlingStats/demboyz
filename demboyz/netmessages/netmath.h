
#pragma once

#include <cstddef>

namespace math
{
    static size_t log2(size_t value)
    {
        size_t res = 0;
        while (value >>= 1)
            ++res;
        return res;
    }

    static size_t BitsToBytes(size_t bits)
    {
        return ((bits + 7) >> 3);
    }
}
