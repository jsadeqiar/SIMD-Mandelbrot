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
        if(current_iterations_limit_ < LIMIT_MAX_ITERATIONS_)
        {
            current_iterations_limit_ <<= 1;
            state_altered_ = true;
            std::cout << "Iter++ " << current_iterations_limit_ << '\n';
        }
        
        return;
    }

    void Mandelbrot::DecreaseIterations()
    {
        if(current_iterations_limit_ > LIMIT_MIN_ITERATIONS_)
        {
            current_iterations_limit_ >>= 1;
            state_altered_ = true;
            std::cout << "Iter-- " << current_iterations_limit_ << '\n';
        }

        return;
    }

    void Mandelbrot::SetCustomIteration(int val)
    {
        if(val > LIMIT_MAX_ITERATIONS_)
            current_iterations_limit_ = LIMIT_MAX_ITERATIONS_;

        else if(val < LIMIT_MIN_ITERATIONS_)
            current_iterations_limit_ = LIMIT_MIN_ITERATIONS_;

        else
        {
            current_iterations_limit_ = val;
        }
        state_altered_ = true;
        std::cout << "Manual Iter change: " << current_iterations_limit_ << '\n';

        return;
    }

    const Framebuffer& Mandelbrot::GetFrameBuffer() const
    {
        return framebuffer_;
    }

    bool Mandelbrot::IsStateAltered() const
    {
        return state_altered_;
    }

    const int Mandelbrot::GetIterationLimitMax() const
    {
        return LIMIT_MAX_ITERATIONS_;
    }

    const int Mandelbrot::GetIterationLimitMin() const
    {
        return LIMIT_MIN_ITERATIONS_;
    }

    const int Mandelbrot::GetCurrentIterationLimit() const
    {
        return current_iterations_limit_;
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
                
                while(std::abs(z) < 2.0 && curr_iter < current_iterations_limit_)
                {
                    z = (z * z) + c;
                    curr_iter++;
                }

                //int temp = (int)(255 * ((double)curr_iter / (double)iterations_));
                
                if(curr_iter == current_iterations_limit_)
                    framebuffer_.SetPixel(y, x, Color(0, 0, 0, 0));

                else
                    framebuffer_.SetPixel(y, x, Color((int)(255 * ((double)curr_iter / (double)current_iterations_limit_)), 0, 0, 0));
            }
        }

        state_altered_ = false;
        std::cout << "Computing cycle...\n";
        return;
    }
}