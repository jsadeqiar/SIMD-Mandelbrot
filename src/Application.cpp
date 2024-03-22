#include "../include/Application.h"

namespace SM
{
    void CreateImGuiContext()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
    }


    Application::Application(ImGuiIO& io) 
    : io_(io)
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

        pixel_format_ = SDL_CreatePixelFormat(SDL_GetWindowPixelFormat(window_));
        //pixel_format_->format = SDL_PIXELFORMAT_RGBA32; // ???

        (void)io_;
        io_.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable;

        ImGui::StyleColorsDark();

        ImGui_ImplSDL3_InitForSDLRenderer(window_, renderer_);
        ImGui_ImplSDLRenderer3_Init(renderer_);

        ImGuiStyle& style = ImGui::GetStyle();
        style.FrameBorderSize = 1.0f;

        running_ = true;
        demo_ = true;
        texHeight_ = 600;
        texWidth_ = 1000;

        mandelbrot_ = Mandelbrot(texHeight_, texWidth_, pixel_format_);
        LIMIT_MAX_ITERATIONS_ = mandelbrot_.GetIterationLimitMax();
        LIMIT_MIN_ITERATIONS_ = mandelbrot_.GetIterationLimitMin();

        // Initial cycle for texture
        mandelbrot_.ComputeCycle_Basic();

        texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA32 , SDL_TEXTUREACCESS_STREAMING, texWidth_, texHeight_); 

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

    SDL_Texture* Application::GetTexture() const
    {
        return texture_;
    }

    void Application::CreateTexture(SDL_Renderer* renderer)
    {
        texture_ = SDL_CreateTexture(renderer, pixel_format_->format, SDL_TEXTUREACCESS_STREAMING, texWidth_, texHeight_);
        return;
    }

    void Application::UpdateTexture(const Framebuffer& fb)
    {
        void* texture_pixels;
        int texture_pitch;
        SDL_LockTexture(texture_, nullptr, &texture_pixels, &texture_pitch);
        std::memcpy(texture_pixels, fb.GetPixelData().data(), sizeof(uint32_t) * fb.GetPixelData().size());
        SDL_UnlockTexture(texture_);

        return;    
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
            if(event.type == SDL_EVENT_KEY_DOWN && event.key.repeat == 0)
            {
                if(event.key.keysym.sym == SDLK_w)
                    mandelbrot_.PanPlot(NORTH);
                if(event.key.keysym.sym == SDLK_a)
                    mandelbrot_.PanPlot(WEST);
                if(event.key.keysym.sym == SDLK_s)
                    mandelbrot_.PanPlot(SOUTH);
                if(event.key.keysym.sym == SDLK_d)
                    mandelbrot_.PanPlot(EAST);
                if(event.key.keysym.sym == SDLK_f)
                    mandelbrot_.ZoomPlot_In();
                if(event.key.keysym.sym == SDLK_g)
                    mandelbrot_.ZoomPlot_Out();
            }
            }

            StartFrame();
            DrawDockSpace();

            if(demo_)
                  ImGui::ShowDemoWindow(&demo_);
            
            // main editor window
            ImGui::Begin("Main window");
            ImGui::Checkbox("Show Demo Window", &demo_);
            
            // Iteration Editor UI
            ImGui::SeparatorText("Iteration Editor");
            static int current_iterations_limit = mandelbrot_.GetCurrentIterationLimit();

            // If a +/- or "enter" of manual input, set it.
            ImGui::SetNextItemWidth(300);
            if(ImGui::InputInt("Input", &current_iterations_limit, 1, 10, ImGuiInputTextFlags_EnterReturnsTrue))
                mandelbrot_.SetCustomIteration(current_iterations_limit);
            ImGui::SameLine(); HelpMarker(
                "Input a desired iteration value from 2 to 1024\n"
                "Holding the (-) and (+) increment by:\n"
                "1 normally\n"
                "10 with CTRL"
            );

            ImGui::SeparatorText("Panning & Zooming");            
            if(ImGui::Button("N/W"))
                mandelbrot_.PanPlot(NORTHWEST);     ImGui::SameLine();
            if(ImGui::Button(" N "))
                mandelbrot_.PanPlot(NORTH);         ImGui::SameLine();
            if(ImGui::Button("N/E"))
                mandelbrot_.PanPlot(NORTHEAST);     ImGui::SameLine(); 
            
            HelpMarker(
                "Navigation can be made with W/A/S/D keys\n"
                "Zooming in and out can be done with F and G"
            );
            
            if(ImGui::Button(" W "))
                mandelbrot_.PanPlot(WEST);          ImGui::SameLine();
            if(ImGui::Button("   "));
            {};                                     ImGui::SameLine();
            if(ImGui::Button(" E "))
                mandelbrot_.PanPlot(EAST);          ImGui::SameLine();
            if(ImGui::Button(" + "))
                mandelbrot_.ZoomPlot_In();          ImGui::SameLine();

            ImGui::Text("Length of Working Real Axis: %.15f", mandelbrot_.GetWorkingRealAxisLength());

            if(ImGui::Button("S/W"))
                mandelbrot_.PanPlot(SOUTHWEST);     ImGui::SameLine();
            if(ImGui::Button(" S "))
                mandelbrot_.PanPlot(SOUTH);         ImGui::SameLine();
            if(ImGui::Button("S/E"))
                mandelbrot_.PanPlot(SOUTHEAST);     ImGui::SameLine();
            if(ImGui::Button(" - "))
                mandelbrot_.ZoomPlot_Out();         ImGui::SameLine();
            
            ImGui::Text("Length of Working Imag Axis: %.15f", mandelbrot_.GetWorkingImagAxisLength());
            
            ImGui::SeparatorText("Frametime/Framerate");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io_.Framerate, io_.Framerate);
            ImGui::End(); 

            // mode selection window
            ImGui::Begin("Mode selection");
            static int mode_selection = 1;
            ImGui::RadioButton("Basic", &mode_selection, 1 << 0); ImGui::SameLine();
            ImGui::RadioButton("Multithreaded", &mode_selection, 1 << 1); ImGui::SameLine();
            ImGui::End();

            if(mandelbrot_.IsStateAltered())
                mandelbrot_.ComputeCycle((Mode)mode_selection);
            UpdateTexture(mandelbrot_.GetFrameBuffer());

            ImGui::SetNextWindowDockID(dockspace_id_, ImGuiCond_FirstUseEver);
            ImGui::Begin("Testing internal dockspace win", nullptr);
            ImGui::Image((void*)texture_, ImVec2(texWidth_, texHeight_));
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

    void Application::HelpMarker(const char* desc)
    {
        ImGui::TextDisabled("(?)");
        if (ImGui::BeginItemTooltip())
        {
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(desc);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }
}