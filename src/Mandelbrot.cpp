#include "../include/Mandelbrot.h"

namespace SM
{
    Mandelbrot::Mandelbrot()
    {
    }

    Mandelbrot::Mandelbrot(const Mandelbrot& mandelbrot)
    : Mandelbrot(mandelbrot.height_, mandelbrot.width_, mandelbrot.pixel_format_)
    {
    }

    Mandelbrot& Mandelbrot::operator=(const Mandelbrot& mandelbrot)
    {
        if(this == &mandelbrot)
            return *this;
        
        pixel_format_ = mandelbrot.pixel_format_;
        height_ = mandelbrot.height_;
        width_ = mandelbrot.width_;
        framebuffer_ = mandelbrot.framebuffer_;
        framebuffer_.FillBuffer(Color(200, 200, 200, 0));  
        return *this;
    }

    Mandelbrot::Mandelbrot(int height, int width, SDL_PixelFormat* pixel_format)
    : height_(height), width_(width), framebuffer_(Framebuffer(height_, width_, pixel_format))
    {
    }

    Mandelbrot::~Mandelbrot()
    {
    }

    void Mandelbrot::IncreaseIterations()
    {
        if(iterations_ < max_iterations_)
            iterations_ <<= 1;
        return;
    }

    void Mandelbrot::DecreaseIterations()
    {
        if(iterations_ > min_iterations_)
            iterations_ >>= 1;
        return;
    }

    const Framebuffer& Mandelbrot::GetFrameBuffer() const
    {
        return framebuffer_;
    }

    void Mandelbrot::ComputeCycle()
    {
        // scale the visible working region of the plot to the display width and height.
        double dx = (plot_real_e_ - plot_real_s_) / (width_);
        double dy = (plot_imag_e_ - plot_imag_s_) / (height_);

        for(int y = 0; y < height_; y++)
        {
            for(int x = 0; x < width_; x++)
            {
                std::complex<double> z(0, 0);
                int curr_iter = 0;

                // set c to the current x and y values multiplied to the scaling dx and dy
                // to get an appropriate progression across the range.
                std::complex<double> c(plot_real_s_ + x * dx, plot_imag_s_ + y * dy);
                
                while(std::abs(z) < 2.0 && curr_iter < iterations_)
                {
                    z = (z * z) + c;
                    curr_iter++;
                }
                
                if(curr_iter == iterations_)
                    framebuffer_.SetPixel(y, x, Color(0, 0, 0, 0));

                else
                    framebuffer_.SetPixel(y, x, Color(255, 255, 255, 0));
            }
        }

        return;
    }
}