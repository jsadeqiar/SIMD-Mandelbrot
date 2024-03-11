#ifndef MANDELBROT_H
#define MANDELBROT_H

#include "./Framebuffer.h"
#include <complex>
#include <iostream>

namespace SM
{
    class Mandelbrot
    {
    private:
        int height_;
        int width_;
        double plot_real_s_ = -2.0;
        double plot_real_e_ = 1.0;
        double plot_imag_s_ = -1.0;
        double plot_imag_e_ = 1.0;

        int iterations_ = 1 << 5;
        const int max_iterations_ = 1 << 10;
        const int min_iterations_ = 1 << 5;

        Framebuffer framebuffer_;
        SDL_PixelFormat* pixel_format_;
        
    public:
        Mandelbrot();
        Mandelbrot(const Mandelbrot& mandelbrot);
        Mandelbrot& operator=(const Mandelbrot& mandelbrot);
        Mandelbrot(int height, int width, SDL_PixelFormat* pixel_format);
        ~Mandelbrot();
        void IncreaseIterations();
        void DecreaseIterations();
        const Framebuffer& GetFrameBuffer() const;

        void ComputeCycle();

    };
}

#endif