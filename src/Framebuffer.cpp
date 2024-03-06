#include "../include/Framebuffer.h"
#include <iostream>

namespace SM
{
    Framebuffer::Framebuffer()
    {
    }

    Framebuffer::Framebuffer(int height, int width) 
    : height_(height), width_(width)
    {
        pixel_data_ = std::vector<uint32_t>(width_ * height_);
    }

    Framebuffer::Framebuffer(int height, int width, SDL_Window* window) 
    : height_(height), width_(width)
    {
        pixel_format_ = SDL_CreatePixelFormat(SDL_GetWindowPixelFormat(window));
        pixel_data_ = std::vector<uint32_t>(width_ * height_);
    }

    Framebuffer::~Framebuffer()
    {
        pixel_data_.clear();
    }

    const std::vector<uint32_t>& Framebuffer::GetPixelData() const
    {
        return pixel_data_;
    }
    
    const int Framebuffer::GetHeight() const
    {
        return height_;
    }
    
    const int Framebuffer::GetWidth() const
    {
        return width_;
    }
    
    void Framebuffer::ClearBuffer()
    {
        std::fill(pixel_data_.begin(), pixel_data_.end(), 0);
        return;
    }

    void Framebuffer::FillBuffer(Color c)
    {
        //uint32_t RGBA = c.r | c.g | c.b | c.a;
        //std::cout << RGBA << ' ';
        //std::fill(pixel_data_.begin(), pixel_data_.end(), RGBA);
        for(int i = 0; i < pixel_data_.size(); i++)
        {
            pixel_data_[i] = SDL_MapRGBA(pixel_format_, c.r, c.g, c.b, c.a);
        }
        return;
    }

    void Framebuffer::SetPixel(int r, int c, Color color)
    {
        pixel_data_[(r * width_) + c] = SDL_MapRGBA(pixel_format_, color.r, color.g, color.b, color.a);

        return;
    }

    Color Framebuffer::GetPixel(int r, int c) const
    {
        Color ReturnColor;
        int idx = (r * width_) + c;
        SDL_GetRGBA(pixel_data_[idx], pixel_format_, &ReturnColor.r, &ReturnColor.g, &ReturnColor.b, &ReturnColor.a);
        return ReturnColor;
    }
}