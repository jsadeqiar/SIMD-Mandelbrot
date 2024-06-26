#ifndef MANDELBROT_H
#define MANDELBROT_H

#include "./Framebuffer.h"
#include "./Threadpool.h"
#include "xsimd/xsimd.hpp"
#include <complex>
#include <iostream>
#include <map>

namespace SM
{
    namespace xs = xsimd;
    enum Mode
    {
        SINGLETHREADED           = 1 << 0,
        MULTITHREADED   = 1 << 1,
        MT_SIMD         = 1 << 2
    };

    enum Direction
    {
        NORTH       =   1 << 0,  // 0b00000001
        SOUTH       =   1 << 1,  // 0b00000010
        WEST        =   1 << 2,  // 0b00000100
        EAST        =   1 << 3,  // 0b00001000
        NORTHEAST   =   1 << 4,  // 0b00010000
        NORTHWEST   =   1 << 5,  // 0b00100000
        SOUTHEAST   =   1 << 6,  // 0b01000000
        SOUTHWEST   =   1 << 7,  // 0b10000000
        RESET       =   1 << 8,
        ZOOMOUT     =   1 << 9,
        ZOOMIN      =   1 << 10
    };

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
        Threadpool threadpool_;
        SDL_PixelFormat* pixel_format_;

        Mode current_mode_;

        void PanPlot_North();
        void PanPlot_South();
        void PanPlot_West();
        void PanPlot_East();
        void ZoomPlot_In();
        void ZoomPlot_Out();
        void ResetPlot();

        void ComputeCycle_Singlethreaded();
        void ComputeCycle_Multithreaded();
        void ComputeCycle_MultithreadedSIMD();
        void ThreadpoolCreateJob(int xS, int xE, int yS, int yE, double dx, double dy);
        void ThreadpoolCreateJobSIMD(int xS, int xE, int yS, int yE, double dx, double dy);
        
    public:
        Mandelbrot();
        Mandelbrot(const Mandelbrot& mandelbrot);
        Mandelbrot& operator=(const Mandelbrot& mandelbrot);
        Mandelbrot(int height, int width, SDL_PixelFormat* pixel_format);
        ~Mandelbrot();

        void PanPlot(Direction dir);
        
        // Setters
        void IncreaseIterations();
        void DecreaseIterations();
        void SetCustomIteration(int val);

        // Getters
        const Framebuffer& GetFrameBuffer() const;
        bool IsStateAltered() const;
        const int GetIterationLimitMax() const;
        const int GetIterationLimitMin() const;
        const int GetCurrentIterationLimit() const;
        const double GetWorkingRealAxisLength() const;
        const double GetWorkingImagAxisLength() const;

        void ComputeCycle(Mode mode);
    };
}

#endif