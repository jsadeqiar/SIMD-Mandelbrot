#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include "SDL.h"
#include "imgui.h"
#include "imgui_impl_sdlrenderer3.h"
#include "imgui_impl_sdl3.h"
#include "./Mandelbrot.h"

namespace SM
{
    void CreateImGuiContext();
    
    class Application
    {
    private:
        SDL_Window* window_;
        SDL_Renderer* renderer_;
        SDL_Texture* texture_;
        SDL_PixelFormat* pixel_format_;
        ImGuiIO& io_; 
        ImGuiID dockspace_id_;
        Mandelbrot mandelbrot_;
        bool running_;
        bool demo_;
        int texHeight_, texWidth_;
        int LIMIT_MAX_ITERATIONS_;
        int LIMIT_MIN_ITERATIONS_;
        
        SDL_Texture* GetTexture() const;
        void CreateTexture(SDL_Renderer* renderer);
        void UpdateTexture(const Framebuffer& fb);

        inline void StartFrame();
        inline void DrawDockSpace();
        void Draw();
        inline void RenderFrame();

        // From dear imgui demo
        void HelpMarker(const char* desc);

    public:
        Application(ImGuiIO& io);
        ~Application();

    };

}

#endif