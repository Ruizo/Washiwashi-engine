#include "ComponentTexture.h"
#include "ComponentMesh.h"

#pragma comment (lib, "devIL.lib")  
#pragma comment (lib, "ILU.lib") 
#pragma comment (lib, "ILUT.lib")

ComponentTexture::ComponentTexture(GameObject* _go) : Component(_go)
{
    name = "Mesh Component";

    ilInit();
    iluInit();
    ilutInit();
}

void ComponentTexture::UpdateInspector()
{
    if (ImGui::CollapsingHeader("Local Texture"))
    {
    }
}

void ComponentTexture::LoadTexture(const char* path)
{
    ComponentMesh* renderMesh = new ComponentMesh(nullptr);
    renderMesh = dynamic_cast<ComponentMesh*>(owner->GetComponent(Component::Type::MESH));

    if (ilLoadImage(path))
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &renderMesh->textureID);
        glBindTexture(GL_TEXTURE_2D, renderMesh->textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

        renderMesh->textureID = ilutGLBindTexImage();
        glBindTexture(GL_TEXTURE_2D, 0);

        WASHI_LOG("Image Loaded from: '%s'", path);
    }
    else
    {
        WASHI_LOG("No image found in this path");
    }

    ilDeleteImages(1, &renderMesh->textureID);
}
