#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"


ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleEditor::~ModuleEditor()
{}

// Load assets
bool ModuleEditor::Start()
{
    LOG("Loading Intro assets");
    bool ret = true;

    App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
    App->camera->LookAt(vec3(0, 0, 0));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
    ImGui_ImplOpenGL3_Init();

    return ret;
}

// Load assets
bool ModuleEditor::CleanUp()
{
    LOG("Unloading Intro scene");

    return true;
}

// Update: draw background
update_status ModuleEditor::Update(float dt)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    static float f = 0.0f;

    // ----- MENU -----
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Exit"))
        {
            return UPDATE_STOP;
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Options"))
    {
        if (ImGui::MenuItem("Configuration"))
            showOptionsWindow = !showOptionsWindow;
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Help"))
    {
        if (ImGui::MenuItem("Gui Demo"))
        {
            showDemoWindow = !showDemoWindow;
        }
        if (ImGui::MenuItem("Documentation"))
        {
            App->RequestBrowser("https://github.com/Ruizo/Washiwashi-engine/wiki");
        }
        if (ImGui::MenuItem("Report a Bug"))
        {
            App->RequestBrowser("https://github.com/Ruizo/Washiwashi-engine/issues");
        }
        if (ImGui::MenuItem("About"))
        {
            showAboutWindow = !showAboutWindow;
        }
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    // ----- WINDOWS -----
    if (showAboutWindow)
    {
        ImGui::Begin("About", &showAboutWindow);
        SDL_version version;
        SDL_GetVersion(&version);
        ImGui::Text("Washiwashi Engine v0.2");
        ImGui::Text("Give your games the best possible face lift!");
        ImGui::Text("By Marc Ruiz & Ignasi Pardo");
        ImGui::Text("\n3rd Party Libraries used");
        ImGui::BulletText("SDL");
        ImGui::SameLine();
        ImGui::Text("%d.%d.%d", version.major, version.minor, version.patch);
        ImGui::BulletText("ImGui 1.85");
        ImGui::BulletText("MathGeoLib 1.5");
        ImGui::BulletText("OpenGL");
        ImGui::SameLine();
        ImGui::Text("%s", glGetString(GL_VERSION));
        ImGui::Text("\nLicense:");
        ImGui::Text("MIT License");
        ImGui::Text("\nCopyright (c) 2021 Marc Ruiz & Ignasi Pardo");
        ImGui::Text("\nPermission is hereby granted, free of charge, to any person obtaining a copy\nof this software and associated documentation files (the 'Software'), to deal\nin the Software without restriction, including without limitation the rights\nto use, copy, modify, merge, publish, distribute, sublicense, and /or sell\ncopies of the Software, and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions:");
        ImGui::Text("\nThe above copyright noticeand this permission notice shall be included in all\ncopies or substantial portions of the Software.");
        ImGui::Text("\nTHE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\nLIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\nSOFTWARE.");

        ImGui::End();
    }
    if (showDemoWindow)
        ImGui::ShowDemoWindow(&showDemoWindow);

    if (showOptionsWindow)
    {
        ImGui::Begin("Configuration", &showOptionsWindow);
        ImGui::Text("Options");

        // ---- Application Child ----
        if (ImGui::CollapsingHeader("Application"))
        {
            //AppNameDisplay
            sprintf_s(name, 25, TITLE);
            ImGui::InputText("App Name", name, 25);
            //OrganizationNameDisplay (Citm)
            sprintf_s(name, 25, ORGANIZATION);
            ImGui::InputText("Organization", name, 25);
            //MaxFPSSlider
            ImGui::SliderInt("Max FPS", &App->maxFPS, 1, 60);
            //LimitFramerateText Limit Framereate: %d
            ImGui::Text("Limit Framerate: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", App->maxFPS);
            //FPSGraph
            FPSGraph(dt, 70);
            //MillisecondsGraph
            MSGraph(dt, 70);
        }

        // ---- Window Child ----
        if (ImGui::CollapsingHeader("Window"))
        {
            //SlidersBrightness
            ImGui::SliderFloat("Brightness", &App->brightness, 0.001f, 1.000f);
            //SlidersWidth&Height
            ImGui::SliderInt("Width", &App->width, 720, 1920);
            ImGui::SliderInt("Height", &App->height, 480, 1080);
            if (App->resizable) 
            {
                SDL_SetWindowSize(App->window->window, App->width, App->height);
            }
            //RefreshRateText
            ImGui::Text("Refresh rate: %d", App->maxFPS);
            //CheckBoxes Fullscreen/Borderless/Resizable/Full Desktop
            if (ImGui::Checkbox("Fullscreen", &App->fullscreen));
            ImGui::SameLine();
            if (ImGui::Checkbox("Resizable", &App->resizable));
            if (ImGui::Checkbox("Borderless", &App->borderless));
            ImGui::SameLine();
            if (ImGui::Checkbox("Full desktop", &App->fullscreenDesktop));
            //Apply Button
            if (ImGui::Button("Apply", ImVec2(60, 25)))
            {
                App->window->SetFullscreen(&App->fullscreen);
                App->window->SetFullscreen(&App->fullscreenDesktop);
                App->window->SetFullscreen(&App->borderless);
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("Click to apply changes!");
            }
            ImGui::SameLine();
            if (ImGui::Button("Save", ImVec2(60, 25)))
            {
                App->Save();
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("Click to save your preferences!");
            }
        }


        if (ImGui::CollapsingHeader("Hardware"))
        {
            ImGui::Checkbox("Active", &active);
            SDL_version version;
            SDL_GetVersion(&version);
            ImGui::Text("SDL Version:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.%d.%d", version.major, version.minor, version.patch);
            ImGui::Separator();
            ImGui::Text("CPU:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d(Cache: %dkb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
            ImGui::Text("System RAM:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.1f", (float)(SDL_GetSystemRAM() / 1024));
            ImGui::Text("Caps: ");

            if (SDL_HasRDTSC() == SDL_bool::SDL_TRUE)
            {
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "RDTSC,");
            }
            if (SDL_HasMMX() == SDL_bool::SDL_TRUE)
            {
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "MMX,");
            }
            if (SDL_HasSSE() == SDL_bool::SDL_TRUE)
            {
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "SSE,");
            }
            if (SDL_HasSSE2() == SDL_bool::SDL_TRUE)
            {
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "SSE2,");
            }
            if (SDL_HasSSE3() == SDL_bool::SDL_TRUE)
            {
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "SSE3,");
            }
            if (SDL_HasSSE41() == SDL_bool::SDL_TRUE)
            {
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "SSE41,");
            }
            if (SDL_HasSSE42() == SDL_bool::SDL_TRUE)
            {
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "SSE41,");
            }
            if (SDL_HasAVX() == SDL_bool::SDL_TRUE)
            {
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "AVX,");
            }
            if (SDL_HasAVX2() == SDL_bool::SDL_TRUE)
            {
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "AVX2");
            }
            ImGui::Separator();

            const GLubyte* vendor = glGetString(GL_VENDOR);
            const GLubyte* renderer = glGetString(GL_RENDERER);

            ImGui::Text("GPU:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", vendor);

            ImGui::Text("Brand:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", renderer);

            GLint parameter = 0;
            ImGui::Text("VRAM budget:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "%dMB", parameter / 1024);

            ImGui::Text("VRAM usage:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "%dMB", parameter / 1024);

            //glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &parameter);
            ImGui::Text("VRAM available:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "%dMB", parameter / 1024);

            ImGui::Text("VRAM reserved:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "%dMB", parameter / 1024);
        }

        ImGui::End();
    }
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return UPDATE_CONTINUE;
}

void ModuleEditor::FPSGraph(float dt, int size)
{
    fps_log.push_back(1 / dt);
    sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
    ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
    if (fps_log.size() > size)
    {
        fps_log.erase(fps_log.begin());
    }
}

void ModuleEditor::MSGraph(float dt, int size)
{
    ms_log.push_back(dt * 1000);
    sprintf_s(title, 25, "Milliseconds %0.1f", ms_log[ms_log.size() - 1]);
    ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
    if (ms_log.size() > size)
    {
        ms_log.erase(ms_log.begin());
    }
}

