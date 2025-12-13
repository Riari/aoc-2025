#include "maths.h"

ull sum_ull(ull items[], size_t size)
{
    ull total = 0;
    for (size_t i = 0; i < size; ++i) total += items[i];
    return total;
}

ull product_ull(ull items[], size_t size)
{
    if (size == 0) return 0;

    ull total = 1;
    for (size_t i = 0; i < size; ++i) total *= items[i];
    return total;
}
