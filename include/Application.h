#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include "SDL.h"
#include "imgui.h"
#include "imgui_impl_sdlrenderer3.h"
#include "imgui_impl_sdl3.h"

namespace SM
{
    void CreateImGuiContext();
    
    class Application
    {
    private:
        SDL_Window* window_;
        SDL_Renderer* renderer_;
        ImGuiIO& io_; 
        bool running_;
        bool demo_;
        
        inline void StartFrame();
        void Draw();
        inline void RenderFrame();

    public:
        Application(ImGuiIO& io);
        ~Application();

    };

}

#endif