#ifndef SIMD_TESTS_H
#define SIMD_TESTS_H

#include "gtest/gtest.h"
#include "xsimd/xsimd.hpp"
#include <iostream>

namespace xs = xsimd;

TEST(SIMDTest, FillBatchFromScalarThenAdd)
{
    // declare 3 batches, 2 that hold values and 1 for result.
    xs::batch<double, xs::avx2> batch1;
    xs::batch<double, xs::avx2> batch2;
    xs::batch<double, xs::avx2> result;

    // fill batch1 with 4 doubles of value: 5.0
    // fill batch2 with 4 doubles of value: 60.0
    batch1 = xs::batch<double, xs::avx2>::broadcast(5.0);
    batch2 = xs::batch<double, xs::avx2>::broadcast(60.0);

    // SIMD add the 2 branches and store result in the result batch.
    result = xs::add(batch1, batch2);

    // vector to act as a buffer, size 8.
    std::vector<double> result_buffer(4);
    result_buffer.reserve(4);
    
    // store the result in the vector
    result.store_unaligned(&result_buffer[0]);

    for(int i = 0; i < result_buffer.size(); i++)
    {
        EXPECT_EQ(result_buffer[i], 65.0);
    }


}

#endif