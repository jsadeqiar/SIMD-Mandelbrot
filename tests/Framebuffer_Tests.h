#ifndef FRAMEBUFFER_TESTS_H
#define FRAMEBUFFER_TESTS_H

#include "gtest/gtest.h"
#include "../include/Framebuffer.h"
#include <iostream>

class FramebufferTest : public testing::Test
{
protected:
    static void SetUpTestSuite()
    {
        if(SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cout << "Couldn't initialize SDL context: " << SDL_GetError();
            exit(0);
        }
        if(SDL_CreateWindowAndRenderer(1600, 900, SDL_WINDOW_RESIZABLE, &window_, &renderer_) < 0)
        {
            std::cout << "Couldn't initialize SDL window and renderer: " << SDL_GetError();
            exit(0);
        }

        pixel_format_ = SDL_CreatePixelFormat(SDL_GetWindowPixelFormat(window_));
    }

    static void TearDownTestSuite()
    {
        SDL_DestroyRenderer(renderer_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }
    
    static SDL_Window* window_;
    static SDL_Renderer* renderer_;
    static SDL_PixelFormat* pixel_format_;
};

SDL_Window* FramebufferTest::window_ = nullptr;
SDL_Renderer* FramebufferTest::renderer_ = nullptr;
SDL_PixelFormat* FramebufferTest::pixel_format_ = nullptr;

TEST_F(FramebufferTest, ConstructFramebuffer)
{
    SM::Framebuffer fb(100, 500);

    EXPECT_EQ(fb.GetHeight(), 100);
    EXPECT_EQ(fb.GetWidth(), 500);
    EXPECT_EQ(fb.GetPixelData().size(), fb.GetHeight() * fb.GetWidth());
}

TEST_F(FramebufferTest, SetPixelInBuffer)
{
    SM::Framebuffer fb(100, 100, window_);
    SM::Color ColorSet(0xF0, 0x00, 0x0F, 0xFF);
    SM::Color ColorRes;
    std::size_t idx = 5 * 100 + 5;

    EXPECT_EQ(ColorSet.r, 0xF0);
    EXPECT_EQ(ColorSet.g, 0x00);
    EXPECT_EQ(ColorSet.b, 0x0F);
    EXPECT_EQ(ColorSet.a, 0xFF);
    EXPECT_EQ(ColorRes.r, 0x00);
    EXPECT_EQ(ColorRes.g, 0x00);
    EXPECT_EQ(ColorRes.b, 0x00);
    EXPECT_EQ(ColorRes.a, 0x00);

    fb.SetPixel(5, 5, ColorSet);
    ColorRes = fb.GetPixel(5, 5);

    EXPECT_EQ(ColorRes.r, 0xF0);
    EXPECT_EQ(ColorRes.g, 0x00);
    EXPECT_EQ(ColorRes.b, 0x0F);
    EXPECT_EQ(ColorRes.a, 0xFF);
}

TEST_F(FramebufferTest, ColorTest)
{
    SM::Color Color(100, 23, 0, 254);

    EXPECT_EQ(Color.r, 100);
    EXPECT_EQ(Color.g, 23);
    EXPECT_EQ(Color.b, 0);
    EXPECT_EQ(Color.a, 254);
}

#endif