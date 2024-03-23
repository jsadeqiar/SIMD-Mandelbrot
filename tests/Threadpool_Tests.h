#ifndef THREADPOOL_TESTS_H
#define THREADPOOL_TESTS_H

#include "gtest/gtest.h"
#include "../include/Threadpool.h"
#include <iostream>
#include <vector>
#include <algorithm>

class ThreadpoolTest : public testing::Test
{

};

std::vector<int> target(10000, 0);
void EditContainerTestFunction(int num)
{
    target[num] = 1;
    return;
}

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

TEST(ThreadpoolTest, ModifyAllElementsInContainer)
{
    SM::Threadpool tp;
    tp.StartPool();

    // send a task to change each indexes value from 0 to 1.
    for(int i = 0; i < 10000; i++)
    {
        tp.QueueTask([=](){
            EditContainerTestFunction(i);
        });
    }

    while(tp.IsPoolBusy()){}

    // there should not be any 0's at any index of the vector. 
    EXPECT_TRUE(std::find(target.begin(), target.end(), 0) == target.end());

    tp.StopPool();
}

#endif