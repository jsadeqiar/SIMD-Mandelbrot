#include "gtest/gtest.h"
#include "Framebuffer_Tests.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();

    return std::cin.get();
}