#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentTexture.h"
#include "Application.h"
#include "Module.h"

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
        ImGui::Checkbox("Enabled", &active);
        ImGui::Checkbox("Show Normals", &showNormals);
    }
}

bool ComponentMesh::LoadMesh(const char* path)
{
    bool ret = false;
    const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

    if (scene)
    {
        meshEntries.resize(scene->mNumMeshes);

        for (unsigned int i = 0; i < meshEntries.size(); i++) {
            const aiMesh* paiMeshs = scene->mMeshes[i];
            activeMeshes.push_back(paiMeshs);
            InitFromScene(i, paiMeshs);
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
    for (unsigned int i = 0; i < meshEntries.size(); ++i)
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, meshEntries[i].vertexBuffer);
        glVertexPointer(3, GL_FLOAT, 0, NULL);

        if (tex != nullptr && tex->IsEnabled())
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBindTexture(GL_TEXTURE_2D, textureID);

            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glBindBuffer(GL_ARRAY_BUFFER, meshEntries[i].textureBuffer);
            glTexCoordPointer(2, GL_FLOAT, 0, NULL);
        }

        if (showNormals == true)
        {
            glEnableClientState(GL_NORMAL_ARRAY);
            glBindBuffer(GL_ARRAY_BUFFER, meshEntries[i].normalBuffer);
            glNormalPointer(GL_FLOAT, 0, NULL);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEntries[i].indexBuffer);

        //const unsigned int MaterialIndex = meshEntries[i].materialIndex;

        glDrawElements(GL_TRIANGLES, meshEntries[i].numIndexes, GL_UNSIGNED_INT, NULL);

    }
    Debuffering();

    glPopMatrix();
}

void ComponentMesh::InitFromScene(unsigned int index, const aiMesh* aiMesh)
{
    InitMesh(index, aiMesh);

    WASHI_LOG("Textures Initialized Correctly");
}

void ComponentMesh::InitMesh(unsigned int index, const aiMesh* aiMesh)
{
    meshEntries[index].materialIndex = aiMesh->mMaterialIndex;

    std::vector<vec3> vertices;
    std::vector<vec2> texCoords;
    std::vector<vec3> normals;
    std::vector<unsigned int> indices;

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0; i < aiMesh->mNumVertices; ++i) 
    {
        const aiVector3D* pos = &(aiMesh->mVertices[i]);
        const aiVector3D* normal = &(aiMesh->mNormals[i]);
        const aiVector3D* texCoord = aiMesh->HasTextureCoords(0) ? &(aiMesh->mTextureCoords[0][i]) : &Zero3D;

        vertices.push_back(vec3(pos->x, pos->y, pos->z));
        texCoords.push_back(vec2(texCoord->x, texCoord->y));
        normals.push_back(vec3(normal->x, normal->y, normal->z));
    }

    for (unsigned int i = 0; i < aiMesh->mNumFaces; ++i) 
    {
        const aiFace& f = aiMesh->mFaces[i];
        indices.push_back(f.mIndices[0]);
        indices.push_back(f.mIndices[1]);
        indices.push_back(f.mIndices[2]);
    }

    meshEntries[index].InitBuffers(vertices, texCoords, normals, indices);
}

void ComponentMesh::InitBuffers(const std::vector<vec3>& vertices, const std::vector<vec2>& textCoords, const std::vector<vec3>& normals, const std::vector<unsigned int>& indices)
{
    numIndexes = indices.size();

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size() * 3, &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &textureBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * textCoords.size() * 2, &textCoords[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals.size() * 3, &normals[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    WASHI_LOG("All buffers Ready!");
}
