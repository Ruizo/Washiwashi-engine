#include "ComponentTexture.h"
#include "ComponentMesh.h"

#pragma comment (lib, "devIL.lib")  
#pragma comment (lib, "ILU.lib") 
#pragma comment (lib, "ILUT.lib")

ComponentTexture::ComponentTexture(GameObject* _go) : Component(_go)
{
    name = "Texture Component";

    ilInit();
    iluInit();
    ilutInit();
}

void ComponentTexture::UpdateInspector()
{
    if (ImGui::CollapsingHeader("Local Texture"))
    {
        ImGui::Checkbox("Enabled", &active);
        ImGui::Text("Local Texture is now:");
        ImGui::SameLine();
        if (IsEnabled()) 
        {
            ImGui::TextColored(ImVec4(0, 255, 0, 100), "Enabled");
        }
        else
        {
            ImGui::TextColored(ImVec4(255, 0, 0, 100), "Disabled");
        }
        ImGui::Text("Texture Path: %s", texPath);
        ImGui::Image((void*)(intptr_t)texture, ImVec2(texWidth/4, texHeight/4));
        ImGui::Text("Texture Width: %d\nTexture Height: %d", texWidth, texHeight);
    }
}

void ComponentTexture::LoadTexture(const char* path)
{
    ComponentMesh* renderMesh = new ComponentMesh(nullptr);
    renderMesh = dynamic_cast<ComponentMesh*>(owner->GetComponent(Component::Type::MESH));

    ILuint image = 0;
    ilGenImages(1, &image);
    ilBindImage(image);

    if (ilLoadImage(path))
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        renderMesh->textureID = ilutGLBindTexImage();

        glBindTexture(GL_TEXTURE_2D, renderMesh->textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, NULL);

        texPath = path;
        texture = renderMesh->textureID;
        texWidth = ilGetInteger(IL_IMAGE_WIDTH);
        texHeight = ilGetInteger(IL_IMAGE_HEIGHT);

        ilDeleteImages(1, &image);
        glBindTexture(GL_TEXTURE_2D, 0);

    }
}

