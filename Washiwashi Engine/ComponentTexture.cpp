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

    ILuint imgID = 0;
    ilGenImages(1, &imgID);
    ilBindImage(imgID);

    if (ilLoadImage(path))
    {
        //Generate texture ID
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        renderMesh->textureID = ilutGLBindTexImage();

         //Bind texture ID
        glBindTexture(GL_TEXTURE_2D, renderMesh->textureID);

        //Generate texture
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        //Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);

        //Unbind texture
        glBindTexture(GL_TEXTURE_2D, NULL);

        //Check for error
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            WASHI_LOG("Error loading texture from pixels! %s\n", glewGetErrorString(error));
        }

        ilDeleteImages(1, &imgID);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

   

}

