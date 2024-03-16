#ifndef MANDELBROT_H
#define MANDELBROT_H

#include "./Framebuffer.h"
#include <complex>
#include <iostream>
#include <map>

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

        int current_iterations_limit_ = 1 << 5;
        const int LIMIT_MAX_ITERATIONS_ = 1 << 10;
        const int LIMIT_MIN_ITERATIONS_ = 1 << 1;

        bool state_altered_ = false;

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
        void SetCustomIteration(int val);

        const Framebuffer& GetFrameBuffer() const;
        bool IsStateAltered() const;
        const int GetIterationLimitMax() const;
        const int GetIterationLimitMin() const;
        const int GetCurrentIterationLimit() const;

        void ComputeCycle();

    };
}

#endif