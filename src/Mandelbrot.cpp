#include "../include/Mandelbrot.h"

namespace SM
{
    Mandelbrot::Mandelbrot()
    {
    }

    // copy constructor
    Mandelbrot::Mandelbrot(const Mandelbrot& mandelbrot)
    : Mandelbrot(mandelbrot.height_, mandelbrot.width_, mandelbrot.pixel_format_)
    {
    }

    // copy assignment operator 
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

    // overloaded constructor
    Mandelbrot::Mandelbrot(int height, int width, SDL_PixelFormat* pixel_format)
    : height_(height), width_(width), framebuffer_(Framebuffer(height_, width_, pixel_format))
    {
    }

    Mandelbrot::~Mandelbrot()
    {
        threadpool_.StopPool();
    }

    void Mandelbrot::PanPlot(Direction dir)
    {
        switch(dir)
        {
            case NORTH:
                PanPlot_North();
                break;
            case SOUTH:
                PanPlot_South();
                break;
            case WEST:
                PanPlot_West();
                break;
            case EAST:
                PanPlot_East();
                break;
            case NORTHEAST:
                PanPlot_North();
                PanPlot_East();
                break;
            case NORTHWEST:
                PanPlot_North();
                PanPlot_West();
                break;
            case SOUTHEAST:
                PanPlot_South();
                PanPlot_East();
                break;
            case SOUTHWEST:
                PanPlot_South();
                PanPlot_West();
                break;
            default:
                break;
        }

        state_altered_ = true;
        return;
    }

    void Mandelbrot::PanPlot_North()
    {
        // shift total region by 20%, requires scale.
        double total_distance_imag = plot_imag_e_ - plot_imag_s_;
        double dy = total_distance_imag * 0.20;
        plot_imag_s_ -= dy;
        plot_imag_e_ -= dy;

        return;
    }

    void Mandelbrot::PanPlot_South()
    {
        // shift total region by 20%, requires scale.
        double total_distance_imag = plot_imag_e_ - plot_imag_s_;
        double dy = total_distance_imag * 0.20;
        plot_imag_s_ += dy;
        plot_imag_e_ += dy;

        return;
    }

    void Mandelbrot::PanPlot_West()
    {
        // shift total region by 20%, requires scale.
        double total_distance_real = plot_real_e_ - plot_real_s_;
        double dx = total_distance_real * 0.20;
        plot_real_s_ -= dx;
        plot_real_e_ -= dx;

        return;
    }

    void Mandelbrot::PanPlot_East()
    {
        // shift total region by 20%, requires scale.
        double total_distance_real = plot_real_e_ - plot_real_s_;
        double dx = total_distance_real * 0.20;
        plot_real_s_ += dx;
        plot_real_e_ += dx;
        
        return;
    }

    void Mandelbrot::ZoomPlot_In()
    {
        // 50% zoom_factor 
        double zoom_factor = 0.50;

        // calculate midpoint of current active plot area on both axis
        double real_midpoint = (plot_real_s_ + plot_real_e_) / 2.0;
        double imag_midpoint = (plot_imag_s_ + plot_imag_e_) / 2.0;

        // subtract the current active plot area by zoom_factor on both axis
        plot_imag_e_ -= (plot_imag_e_ * zoom_factor);
        plot_imag_s_ -= (plot_imag_s_ * zoom_factor);
        plot_real_e_ -= (plot_real_e_ * zoom_factor);
        plot_real_s_ -= (plot_real_s_ * zoom_factor);

        // add the midpoints (scaled by the zoom_factor) to both axis to prevent converging to 0,0
        plot_imag_e_ += (imag_midpoint * zoom_factor);
        plot_imag_s_ += (imag_midpoint * zoom_factor);
        plot_real_e_ += (real_midpoint * zoom_factor);
        plot_real_s_ += (real_midpoint * zoom_factor);

        state_altered_ = true;
        return;
    }

    void Mandelbrot::ZoomPlot_Out()
    {
        // 50% zoom factor
        double zoom_factor = 1.5;

        // calculate midpoint of current active plot area on both axis
        double real_midpoint = (plot_real_s_ + plot_real_e_) / 2.0;
        double imag_midpoint = (plot_imag_s_ + plot_imag_e_) / 2.0;

        // add the current active plot area by zoom_factor on both axis
        plot_imag_e_ += (plot_imag_e_ * zoom_factor);
        plot_imag_s_ += (plot_imag_s_ * zoom_factor);
        plot_real_e_ += (plot_real_e_ * zoom_factor);
        plot_real_s_ += (plot_real_s_ * zoom_factor);

        // subtract the midpoints (scaled by the zoom_factor) to both axis to prevent converging to 0,0
        plot_imag_e_ -= (imag_midpoint * zoom_factor);
        plot_imag_s_ -= (imag_midpoint * zoom_factor);
        plot_real_e_ -= (real_midpoint * zoom_factor);
        plot_real_s_ -= (real_midpoint * zoom_factor);
        
        state_altered_ = true;
        return;
    }

    void Mandelbrot::ResetPlot()
    {
        plot_real_s_ = -2.0;
        plot_real_e_ = 1.0;
        plot_imag_s_ = -1.0;
        plot_imag_e_ = 1.0;

        state_altered_ = true;
        return;
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

    const double Mandelbrot::GetWorkingRealAxisLength() const
    {
        return plot_real_e_ - plot_real_s_;
    }

    const double Mandelbrot::GetWorkingImagAxisLength() const
    {
        return plot_imag_e_ - plot_imag_s_;
    }

    void Mandelbrot::ComputeCycle(Mode mode)
    {
        if(mode == BASIC)
            ComputeCycle_Basic();
        else if(mode == MULTITHREADED)
            ComputeCycle_Multithreaded();
        else if(mode == MT_SIMD)
            ComputeCycle_MultithreadedSIMD();

        return;
    }

    void Mandelbrot::ComputeCycle_Basic()
    {
        if(threadpool_.GetThreadCount() != 0)
            threadpool_.StopPool();

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
                
                if(curr_iter == current_iterations_limit_)
                    framebuffer_.SetPixel(y, x, Color(0, 0, 0, 0));

                else
                    framebuffer_.SetPixel(y, x, Color((int)(255 * ((double)curr_iter / (double)current_iterations_limit_)), 0, 0, 0));
            }
        }

        state_altered_ = false;
        return;
    }

    void Mandelbrot::ComputeCycle_Multithreaded()
    {
        if(threadpool_.GetThreadCount() == 0)
            threadpool_.StartPool();

        // divide the work among the number of threads
        int numThreads = threadpool_.GetThreadCount();

        // scale the visible working region of the plot to the display width and height.
        double dx = (plot_real_e_ - plot_real_s_) / (width_);
        double dy = (plot_imag_e_ - plot_imag_s_) / (height_);

        for(double i = 0.0; i < (double)numThreads; i++)
        {
            int xS = (i       / (double)numThreads) * width_;
            int xE = ((i + 1) / (double)numThreads) * width_;
            int yS = 0;
            int yE = height_;
            threadpool_.QueueTask([=, &dx, &dy]{
                ThreadpoolCreateJob(xS, xE, yS, yE, dx, dy);
            });
        }
        while(threadpool_.IsPoolBusy()){}
        state_altered_ = false;
        return;
    }

    void Mandelbrot::ComputeCycle_MultithreadedSIMD()
    {
        if(threadpool_.GetThreadCount() == 0)
            threadpool_.StartPool();
        
        // divide the work among the number of threads
        int numThreads = threadpool_.GetThreadCount();

        // scale the visible working region of the plot to the display width and height.
        double dx = (plot_real_e_ - plot_real_s_) / (width_);
        double dy = (plot_imag_e_ - plot_imag_s_) / (height_);

        for(double i = 0.0; i < (double)numThreads; i++)
        {
            int xS = (i       / (double)numThreads) * width_;
            int xE = ((i + 1) / (double)numThreads) * width_;
            int yS = 0;
            int yE = height_;
            threadpool_.QueueTask([=, &dx, &dy]{
                ThreadpoolCreateJobSIMD(xS, xE, yS, yE, dx, dy);
            });
        }
        while(threadpool_.IsPoolBusy()){}
        state_altered_ = false;
        return;

    }

    void Mandelbrot::ThreadpoolCreateJob(int xS, int xE, int yS, int yE, double dx, double dy)
    {
        for(int y = yS; y < yE; y++)
        {
            for(int x = xS; x < xE; x++)
            {
                std::complex<double> z(0, 0);
                std::complex<double> c(plot_real_s_ + x * dx, plot_imag_s_ + y * dy);
                int curr_iter = 0;

                while(std::abs(z) < 2.0 && curr_iter < current_iterations_limit_)
                {
                    z = (z * z) + c;
                    curr_iter++;
                }

                if(curr_iter == current_iterations_limit_)
                    framebuffer_.SetPixel(y, x, Color(0, 0, 0, 0));

                else
                    framebuffer_.SetPixel(y, x, Color((int)(255 * ((double)curr_iter / (double)current_iterations_limit_)), 0, 0, 0));

            }
        }

        return;
    }

    void Mandelbrot::ThreadpoolCreateJobSIMD(int xS, int xE, int yS, int yE, double dx, double dy)
    {
        using double_batch = xs::batch<double, xs::avx2>;
        using bool_batch   = xs::batch_bool<double, xsimd::avx2>;

        std::size_t x_range = xE - xS;
        std::size_t batch_size = double_batch::size;
        std::size_t simd_range = x_range - (x_range % batch_size);
        
        // send off portions that wont fill up a simd batch to be computed normally with multithreading
        threadpool_.QueueTask([=]{
            ThreadpoolCreateJob(xS + simd_range, xE, yS, yE, dx, dy);
        });

        double_batch zr, zi, cr, ci;
        double_batch zr2, zi2, zr2subzi2, newzr, zr2addzi2;
        double_batch zrmulzi, newzi;
        double_batch curr_iter;

        bool_batch condition_mask1, condition_mask2, combined_mask;

        // constant batches
        double_batch two = double_batch::broadcast(2.0);
        double_batch four = double_batch::broadcast(4.0);
        double_batch current_iterations_limit = double_batch::broadcast((double)current_iterations_limit_);

        std::vector<double> temp(batch_size, 0.0);


        // now to compute the main region of the thread that can be SIMD'd
        for(int y = yS; y < yE; y++)
        {
            ci = double_batch::broadcast(plot_imag_s_ + y * dy);
            
            for(int x = xS; x < (xS + simd_range); x += batch_size)
            {
                curr_iter = double_batch::broadcast(0.0);
                zr = double_batch::broadcast(0.0);
                zi = double_batch::broadcast(0.0);
                cr = double_batch::batch(plot_real_s_ + x * dx,
                                         plot_real_s_ + (x + 1) * dx,
                                         plot_real_s_ + (x + 2) * dx,
                                         plot_real_s_ + (x + 3) * dx);

                do
                {
                    zr2         = xs::mul(zr, zr);
                    zi2         = xs::mul(zi, zi);
                    zr2subzi2   = xs::sub(zr2, zi2);
                    newzr       = xs::add(zr2subzi2, cr);
                    zrmulzi     = xs::mul(zr, zi);
                    newzi       = xs::fma(zrmulzi, two, ci);
                    zr2addzi2   = xs::add(zr2, zi2);

                    condition_mask1 = xs::lt(zr2addzi2, four);
                    condition_mask2 = xs::lt(curr_iter, current_iterations_limit);
                    combined_mask   = xs::bitwise_and(condition_mask1, condition_mask2);
                    
                    
                    curr_iter = xs::incr_if(curr_iter, combined_mask);

                    zr = newzr;
                    zi = newzi;
                    //std::cout << combined_mask.mask() << '\n';
                } while (combined_mask.mask());
                
                curr_iter.store_unaligned(&temp[0]);

                for(int i = 0; i < temp.size(); i++)
                {
                    if(temp[i] == current_iterations_limit_)
                        framebuffer_.SetPixel(y, x + i, Color(0, 0, 0, 0));
                    else
                    framebuffer_.SetPixel(y, x + i, Color((int)(255 * ((double)temp[i] / (double)current_iterations_limit_)), 0, 0, 0));
                }
                
            }
        }

        return;
    }
}