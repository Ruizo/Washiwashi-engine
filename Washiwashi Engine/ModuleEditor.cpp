#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include <list> 
#include <fstream>
#include <string>

extern std::list<std::string> consoleLogs;


ModuleEditor::ModuleEditor(Application* app, bool startEnabled) : Module(app, startEnabled)
{

}

ModuleEditor::~ModuleEditor()
{}

// Load assets
bool ModuleEditor::Start()
{
    WASHI_LOG("Loading Editor assets");
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
    WASHI_LOG("Unloading Intro scene");

    return true;
}

void ModuleEditor::UpdateGameObjects(GameObject* go)
{
    if (go->components.size() > 0)
    {
        for (size_t i = 0; i < go->components.size(); i++)
        {
            if (go->components.at(i)->IsEnabled())
            {
                go->components.at(i)->UpdateInspector();
            }
        }
    }
}


// Update: draw background
UpdateStatus ModuleEditor::Update(float dt)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    static float f = 0.0f;

    // ----- MENU -----
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Menu"))
    {
        if (ImGui::MenuItem("Close Tabs"))
        {
            showDemoWindow = false;
            showAboutWindow = false;
            showOptionsWindow = false;
            showHierarchyTab = false;
            showInspectorTab = false;
            console = false;
        }
        if (ImGui::MenuItem("Debug Console"))
        {
            console = !console;
        }
        
        if (ImGui::MenuItem("Quit"))
        {
            return UPDATE_STOP;
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Game Object"))
    {
        if (ImGui::BeginMenu("Tabs"))
        {
            if (ImGui::MenuItem("Hierarchy"))
            {
                showHierarchyTab = !showHierarchyTab;
            }
            if (ImGui::MenuItem("Inspector"))
            {
                showInspectorTab = !showInspectorTab;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Create"))
        {
            if (ImGui::MenuItem("Cube"))
            {
                GameObject* cube = App->scene->SpawnGameObject("Cube", App->scene->root);
                cube->CreateComponent(Component::Type::MESH);
                cube->LocateComponent("Engine/Library/Meshes/Prefabs/SimpleShapes/cube.fbx", Component::Type::MESH);
            }
            if (ImGui::MenuItem("Sphere"))
            {
                GameObject* sphere = App->scene->SpawnGameObject("Sphere", App->scene->root);
                sphere->CreateComponent(Component::Type::MESH);
                sphere->LocateComponent("Engine/Library/Meshes/Prefabs/SimpleShapes/sphere.fbx", Component::Type::MESH);
            }
            if (ImGui::MenuItem("Cylinder"))
            {
                GameObject* cylinder = App->scene->SpawnGameObject("Cylinder", App->scene->root);
                cylinder->CreateComponent(Component::Type::MESH);
                cylinder->LocateComponent("Engine/Library/Meshes/Prefabs/SimpleShapes/cylinder.fbx", Component::Type::MESH);
            }
            if (ImGui::MenuItem("Pyramid"))
            {
                GameObject* pyramid = App->scene->SpawnGameObject("Pyramid", App->scene->root);
                pyramid->CreateComponent(Component::Type::MESH);
                pyramid->LocateComponent("Engine/Library/Meshes/Prefabs/SimpleShapes/pyramid.fbx", Component::Type::MESH);
            }
            if (ImGui::MenuItem("Plane"))
            {
                GameObject* plane = App->scene->SpawnGameObject("Plane", App->scene->root);
                plane->CreateComponent(Component::Type::MESH);
                plane->LocateComponent("Engine/Library/Meshes/Prefabs/SimpleShapes/plane.fbx", Component::Type::MESH);
            }
            ImGui::EndMenu();
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
        if (ImGui::BeginMenu("About Us"))
        {
            if (ImGui::MenuItem("Documentation"))
            {
                App->RequestBrowser("https://github.com/Ruizo/Washiwashi-engine/wiki");
            }
            if (ImGui::MenuItem("Download latest"))
            {
                App->RequestBrowser("https://github.com/Ruizo/Washiwashi-engine/releases");
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
    }
    ImGui::EndMainMenuBar();


    // ----- WINDOWS -----
    /// ----- Console -----
    if (console)
    {
        ImGui::Begin("Console", &console);
        for (auto& a : consoleLogs)
        {
            ImGui::TextWrapped(a.c_str());
        }
        ImGui::End();
    }

    /// ----- About -----
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

    /// ----- DemoWindow -----
    if (showDemoWindow)
        ImGui::ShowDemoWindow(&showDemoWindow);

    /// ----- Options -----
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
                WASHI_LOG("Saving");
            }
            if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("Click to save your preferences!");
            }
        }

        // ---- Hardware Child ----
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

    /// ----- HierarchyTab -----
    if (showHierarchyTab) 
    {
        ImGui::Begin("Hierarchy", &showHierarchyTab);
        if (App->scene->root != nullptr)
        {
            HierarchyListTree(App->scene->root);
        }
        ImGui::End();
    }

    /// ----- InspectorTab -----
    if (showInspectorTab)
    {
        ImGui::Begin("Inspector", &showInspectorTab);
        UpdateComponentsInspector();
        ImGui::End();
    }

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return UPDATE_CONTINUE;
}

void ModuleEditor::FPSGraph(float dt, int size)
{
    fpsLog.push_back(1 / dt);
    sprintf_s(title, 25, "Framerate %.1f", fpsLog[fpsLog.size() - 1]);
    ImGui::PlotHistogram("##framerate", &fpsLog[0], fpsLog.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
    if (fpsLog.size() > size)
    {
        fpsLog.erase(fpsLog.begin());
    }
}

void ModuleEditor::MSGraph(float dt, int size)
{
    msLog.push_back(dt * 1000);
    sprintf_s(title, 25, "Milliseconds %0.1f", msLog[msLog.size() - 1]);
    ImGui::PlotHistogram("##milliseconds", &msLog[0], msLog.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
    if (msLog.size() > size)
    {
        msLog.erase(msLog.begin());
    }
}

void ModuleEditor::HierarchyListTree(GameObject* go)
{
    ImGuiTreeNodeFlags parentFlags = ImGuiTreeNodeFlags_None | ImGuiTreeNodeFlags_DefaultOpen | (go->children.empty() ? ImGuiTreeNodeFlags_Leaf : 0);
    if (go == selectedGameObject)
    {
        parentFlags |= ImGuiTreeNodeFlags_Selected;
    }

    bool open = ImGui::TreeNodeEx(go->name.c_str(), parentFlags);

    if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left))
    {
        selectedGameObject = go;
    }

    if (open) {
        for (size_t i = 0; i < go->children.size(); i++)
        {
            HierarchyListTree(go->children.at(i));
        }
        ImGui::TreePop();
    }
}

void ModuleEditor::UpdateComponentsInspector()
{
    if (App->scene->root != nullptr)
    {
        for (int i = 0; i < App->scene->root->children.size(); ++i)
        {
            if (App->scene->root->children.at(i) == selectedGameObject)
            {
                for (int j = 0; j < App->scene->root->children.at(i)->components.size(); ++j)
                {
                    App->scene->root->children.at(i)->components.at(j)->UpdateInspector();
                }
            }
        }
    }
}