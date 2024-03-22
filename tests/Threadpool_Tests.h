#ifndef THREADPOOL_TESTS_H
#define THREADPOOL_TESTS_H

#include "gtest/gtest.h"
#include "../include/Threadpool.h"
#include <iostream>

class ThreadpoolTest : public testing::Test
{
    
};

TEST(ThreadpoolTest, ConstructThreadpool)
{
    SM::Threadpool tp;

    EXPECT_TRUE(tp.StartPool());

    tp.StopPool();
}

TEST(ThreadpoolTest, CloseThreadpool)
{
    SM::Threadpool tp;

    tp.StartPool();

    EXPECT_TRUE(tp.StopPool());
}

TEST(ThreadpoolTest, ProperThreadCount)
{
    SM::Threadpool tp;

    tp.StartPool();

    EXPECT_EQ(tp.GetThreadCount(), std::thread::hardware_concurrency());

    tp.StopPool();
}

#endif