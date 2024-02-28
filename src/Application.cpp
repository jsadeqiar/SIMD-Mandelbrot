#include "Application.h"

namespace SM
{
    void CreateImGuiContext()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
    }

    Application::Application(ImGuiIO& io) : io_(io)
    {
        if(SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cout << "Couldn't initialize SDL context: " << SDL_GetError();
            exit(0);
        }

        if(SDL_CreateWindowAndRenderer(1600, 900, SDL_WINDOW_RESIZABLE, &window_, &renderer_) < 0)
        {
            std::cout << "Couldn't initialize SDL window and renderer: " << SDL_GetError();
            exit(0);
        }

        SDL_SetWindowTitle(window_, "Main Window");

        (void)io_;
        io_.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable;

        ImGui::StyleColorsDark();

        ImGui_ImplSDL3_InitForSDLRenderer(window_, renderer_);
        ImGui_ImplSDLRenderer3_Init(renderer_);

        running_ = true;
        demo_ = true;

        Draw();
    }

    Application::~Application()
    {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();

        SDL_DestroyRenderer(renderer_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }

    inline void Application::StartFrame()
    {
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        return;
    }

    void Application::Draw()
    {
        while(running_)
        {
            SDL_Event event;
            while(SDL_PollEvent(&event))
            {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if(event.type == SDL_EVENT_QUIT)
                running_ = false;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window_))
                running_ = false;
            }

            StartFrame();

            // DRAW HERE

            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | 
                                            ImGuiWindowFlags_NoMove | 
                                            ImGuiWindowFlags_NoResize | 
                                            ImGuiWindowFlags_NoCollapse;

            //if(ImGui::Begin("Test", nullptr, window_flags))
            //{
                if(demo_)
                    ImGui::ShowDemoWindow(&demo_);

                {
                ImGui::Begin("Main window");
                ImGui::Checkbox("Show Demo Window", &demo_);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io_.Framerate, io_.Framerate);
                ImGui::End();
                }
            //}

            

            RenderFrame();
        }
        
        return;
    }

    inline void Application::RenderFrame()
    {
        ImGui::Render();
        SDL_SetRenderDrawColor(renderer_, (uint8_t)(167), (uint8_t)(199), (uint8_t)(231), (uint8_t)(255));
        SDL_RenderClear(renderer_);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer_);

        return;
    }
}