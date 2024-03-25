#ifndef SIMD_TESTS_H
#define SIMD_TESTS_H

#include "gtest/gtest.h"
#include "xsimd/xsimd.hpp"
#include <iostream>
#include <complex>

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

    // SIMD add the 2 batches and store result in the result batch.
    result = xs::add(batch1, batch2);

    // vector to act as a buffer, size 4.
    std::vector<double> result_buffer(4);
    result_buffer.reserve(4);
    
    // store the result in the vector
    result.store_unaligned(&result_buffer[0]);

    for(int i = 0; i < result_buffer.size(); i++)
    {
        EXPECT_EQ(result_buffer[i], 65.0);
    }
}

TEST(SIMDTest, GrabComplexFromBufferThenMultiply)
{
    // shorthand type alias.
    using complex_vector = std::vector<std::complex<double>>;

    complex_vector buffer;
    complex_vector result_vector(10, std::complex<double>(0.0, 0.0));
    complex_vector reference_result_vector;

    buffer.reserve(10);
    reference_result_vector.reserve(10);

    // fill buffer with complex values from (0, 0) ... (9, 9).
    for(int i = 0; i < 10; i++)
    {
        buffer.push_back(std::complex<double>((double)i, (double)i));
    }

    // shorthand type alias
    using complex_batch = xs::batch<std::complex<double>, xs::avx2>;
    
    // batch size of how many std::complex<double> we can compute at once
    std::size_t complex_branch_size = complex_batch::size;

    // we can only vectorize regions of the buffer that are of the branch size. 
    // need to trim off the edge if needed.
    std::size_t vectorizable_range = buffer.size() - (buffer.size() % complex_branch_size);

    // 2 computation batches, 1 for result.
    complex_batch batch1;
    complex_batch batch2;
    complex_batch result_batch;

    // fill batch with 4 std::complex (100.0, 50.0).
    const std::complex<double> COMPLEX_CONST_VAL(100.0, 50.0);
    batch1 = complex_batch::broadcast(COMPLEX_CONST_VAL);

    // buffer is of size 10, AVX2 allows 256-bit register (4 doubles).
    // we can only vectorize buffer range 0-3, then 4-7.
    for(int i = 0; i < vectorizable_range; i += complex_branch_size)
    {
        batch2 = complex_batch::load_unaligned(&buffer[i]);
        result_batch = batch1 * batch2;
        result_batch.store_unaligned(&result_vector[i]);
    }

    // complete calculations for remaining buffer range.
    // in this case: 8-9.
    for(int i = vectorizable_range; i < buffer.size(); i++)
    {
        result_vector[i] = buffer[i] * COMPLEX_CONST_VAL;
    }

    // compute the non-simd result into reference_result_vector to compare
    for(int i = 0; i < buffer.size(); i++)
    {
        reference_result_vector.push_back(buffer[i] * COMPLEX_CONST_VAL);
    }

    // validate the simd computations with the manual ones
    for(int i = 0; i < result_vector.size(); i++)
    {
        EXPECT_EQ(result_vector[i], reference_result_vector[i]);
    }
}


#endif