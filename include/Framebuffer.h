#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <vector>
#include <memory>
#include "SDL.h"
#include "./Color.h"

namespace SM
{
    class Framebuffer
    {
    private:
        std::vector<uint32_t> pixel_data_;
        SDL_PixelFormat* pixel_format_; //Requires PixelFormat to know how to map pixel values.
        int height_;
        int width_;

    public:
        Framebuffer();
        Framebuffer(int height, int width);
        Framebuffer(int height, int width, SDL_Window* window);
        ~Framebuffer();
        
        const std::vector<uint32_t>& GetPixelData() const;
        const int GetHeight() const;
        const int GetWidth() const;
        const int GetBufferSize() const;
        void ClearBuffer();
        void FillBuffer(Color c);
        void SetPixel(int r, int c, Color color);
        Color GetPixel(int r, int c) const;
    };
}

#endif