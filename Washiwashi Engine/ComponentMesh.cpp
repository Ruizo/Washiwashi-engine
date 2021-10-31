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
    if (active)
    {
        Render();
    }
}

void ComponentMesh::UpdateInspector()
{
    if (ImGui::CollapsingHeader("Local Mesh"))
    {
    }
}

bool ComponentMesh::LoadMesh(const char* path)
{
    bool ret = false;
    const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

    if (scene)
    {
        InitFromScene(scene, path);
        ret = true;
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

    glPushMatrix();
    glMultMatrixf(trans->transform.M);

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);


    for (unsigned int i = 0; i < mEntries.size(); i++) 
    {
        glTexCoordPointer(2, GL_FLOAT, 0, NULL);

        glBindBuffer(GL_ARRAY_BUFFER, mEntries[i].VB);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEntries[i].TB);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float3), (const GLvoid*)12);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (const GLvoid*)20);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEntries[i].IB);

        glDrawElements(GL_TRIANGLES, mEntries[i].numIndices, GL_UNSIGNED_INT, 0);

        const unsigned int MaterialIndex = mEntries[i].materialIndex;
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glPopMatrix();
}

void ComponentMesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
{
    mEntries.resize(pScene->mNumMeshes);

    for (unsigned int i = 0; i < mEntries.size(); i++) {
        const aiMesh* paiMeshs = pScene->mMeshes[i];
        activeMeshes.push_back(paiMeshs);
        InitMesh(i, paiMeshs);
    }
}

void ComponentMesh::InitMesh(unsigned int Index, const aiMesh* paiMeshs)
{
    mEntries[Index].materialIndex = paiMeshs->mMaterialIndex;

    std::vector<float3> Vertices;
    std::vector<float2> texCord;
    std::vector<unsigned int> Indices;

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0; i < paiMeshs->mNumVertices; i++) {
        const aiVector3D* pPos = &(paiMeshs->mVertices[i]);
        const aiVector3D* pNormal = &(paiMeshs->mNormals[i]);
        const aiVector3D* pTexCoord = paiMeshs->HasTextureCoords(0) ? &(paiMeshs->mTextureCoords[0][i]) : &Zero3D;

        Vertices.push_back(float3(pPos->x, pPos->y, pPos->z));
        texCord.push_back(float2(pTexCoord->x, pTexCoord->y));
    }

    for (unsigned int i = 0; i < paiMeshs->mNumFaces; i++) {
        const aiFace& Face = paiMeshs->mFaces[i];
        assert(Face.mNumIndices == 3);
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }

    mEntries[Index].Init(Vertices, texCord, Indices);
}

void ComponentMesh::Init(const std::vector<float3>& Vertices, const std::vector<float2>& textCord, const std::vector<unsigned int>& Indices)
{
    numIndices = Indices.size();

    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &TB);
    glBindBuffer(GL_ARRAY_BUFFER, TB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float2) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, &Indices[0], GL_STATIC_DRAW);

    struct aiLogStream stream;
    stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
    aiAttachLogStream(&stream);
}

void ComponentMesh::Clear()
{

}
