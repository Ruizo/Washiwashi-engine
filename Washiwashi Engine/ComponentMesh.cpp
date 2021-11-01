#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentTexture.h"

ComponentMesh::ComponentMesh(GameObject* _go) : Component(_go)
{
    name = "Mesh Component";
}

ComponentMesh::~ComponentMesh()
{
}

void ComponentMesh::Update()
{
    if (IsEnabled())
    {
        Render();
    }
}

void ComponentMesh::UpdateInspector()
{
    if (ImGui::CollapsingHeader("Local Mesh"))
    {
        ComponentTexture* tex = new ComponentTexture(nullptr);
        tex = dynamic_cast<ComponentTexture*>(owner->GetComponent(Component::Type::TEXTURE));

        ImGui::Checkbox("Enabled", &active);
        ImGui::Text("Local Mesh is now:");
        ImGui::SameLine();
        if (IsEnabled())
        {
            ImGui::TextColored(ImVec4(0, 255, 0, 100), "Enabled");
        }
        else
        {
            ImGui::TextColored(ImVec4(255, 0, 0, 100), "Disabled");
            tex->Disable();
        }
    }
}

bool ComponentMesh::LoadMesh(const char* path)
{
    bool ret = false;
    const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

    if (scene)
    {
        for (int i = 0; i < scene->mNumMeshes; i++)
        {
            InitFromScene(scene->mMeshes[i]);
            ret = true;
        }
    }
    else
    {
        WASHI_LOG("Error loading '%s'", path);
    }

    return ret;
}

void ComponentMesh::Render()
{
    ComponentTransform* trans = new ComponentTransform(nullptr);
    trans = dynamic_cast<ComponentTransform*>(owner->GetComponent(Component::Type::TRANSFORM));
    ComponentTexture* tex = new ComponentTexture(nullptr);
    tex = dynamic_cast<ComponentTexture*>(owner->GetComponent(Component::Type::TEXTURE));

    glPushMatrix();
    glMultMatrixf(trans->GetTransform());

    VertexBuffering();

    if(tex->IsEnabled()) 
        TextureBuffering();

    IndexBuffering();

    glDrawElements(GL_TRIANGLES, meshIndexes.size(), GL_UNSIGNED_INT, NULL);

    Debuffering();

    glPopMatrix();
}

void ComponentMesh::InitFromScene(const aiMesh* aiMesh)
{
    InitMesh(aiMesh);

    InitBuffers();

    WASHI_LOG("Textures Initialized Correctly");
}

void ComponentMesh::InitMesh(const aiMesh* aiMesh)
{

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0; i < aiMesh->mNumFaces; i++) 
    {
        const aiFace& f = aiMesh->mFaces[i];
        meshIndexes.push_back(f.mIndices[0]);
        meshIndexes.push_back(f.mIndices[1]);
        meshIndexes.push_back(f.mIndices[2]);
    }

    for (unsigned int i = 0; i < aiMesh->mNumVertices; i++) 
    {
        const aiVector3D* pPos = &(aiMesh->mVertices[i]);
        const aiVector3D* pTexCoord = aiMesh->HasTextureCoords(0) ? &(aiMesh->mTextureCoords[0][i]) : &Zero3D;

        vertexCoords.push_back(vec3(pPos->x, pPos->y, pPos->z));
        texCoords.push_back(vec2(pTexCoord->x, pTexCoord->y));
    }

}

void ComponentMesh::InitBuffers()
{
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCoords.size(), &vertexCoords[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &textureBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * meshIndexes.size(), &meshIndexes[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    WASHI_LOG("All buffers Ready!");
}
