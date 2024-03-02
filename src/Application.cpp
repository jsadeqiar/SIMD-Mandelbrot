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

    inline void Application::DrawDockSpace()
    {
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | 
                                        ImGuiWindowFlags_NoDocking | 
                                        ImGuiWindowFlags_NoScrollbar | 
                                        ImGuiWindowFlags_NoMove | 
                                        ImGuiWindowFlags_NoResize | 
                                        ImGuiWindowFlags_NoCollapse |
                                        ImGuiWindowFlags_NoBringToFrontOnFocus |
                                        ImGuiWindowFlags_NoNavFocus | 
                                        ImGuiWindowFlags_NoTitleBar;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::Begin("Main Window", nullptr, window_flags); // Dockspace begin
        if(io_.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            dockspace_id_ = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id_, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        if(ImGui::BeginMenuBar())
        {
            if(ImGui::BeginMenu("Test"))
            {
                ImGui::Separator();
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        
        ImGui::End();                                       // Dockspace end

        

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
            DrawDockSpace();

            // Draw within docking space under.

            if(demo_)
                  ImGui::ShowDemoWindow(&demo_);
            ImGui::Begin("Main window");
            ImGui::Checkbox("Show Demo Window", &demo_);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io_.Framerate, io_.Framerate);
            ImGui::End();   

            ImGui::SetNextWindowDockID(dockspace_id_, ImGuiCond_FirstUseEver);
            ImGui::Begin("Testing internal dockspace win", nullptr);
            ImGui::End();
                                        

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