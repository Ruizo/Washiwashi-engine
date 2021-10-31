#include "ComponentMesh.h"
#include "ComponentTransform.h"

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
    ComponentTransform* t = new ComponentTransform(nullptr);
    t = dynamic_cast<ComponentTransform*>(owner->GetComponent(Component::Type::TRANSFORM));

    glPushMatrix();
    glMultMatrixf(t->GetTransform());

    VertexBuffering();

    TextureBuffering();

    IndexBuffering();

    glDrawElements(GL_TRIANGLES, meshIndexes.size(), GL_UNSIGNED_INT, NULL);

    Debuffering();

    glPopMatrix();
}

void ComponentMesh::InitFromScene(const aiMesh* paiMesh)
{
    unsigned int verticesNum = 0;
    unsigned int indicesNum = 0;

    materialIndex = paiMesh->mMaterialIndex;
    meshIndexes.reserve(paiMesh->mNumFaces * 3);

    verticesNum += paiMesh->mNumVertices;
    indicesNum += meshIndexes.size();

    InitMesh(paiMesh);

    InitBuffers();
    WASHI_LOG("Textures Initialized Correctly");
}

void ComponentMesh::InitMesh(const aiMesh* paiMeshs)
{

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0; i < paiMeshs->mNumVertices; i++) {
        const aiVector3D* pPos = &(paiMeshs->mVertices[i]);
        const aiVector3D* pTexCoord = paiMeshs->HasTextureCoords(0) ? &(paiMeshs->mTextureCoords[0][i]) : &Zero3D;

        vertexCoords.push_back(vec3(pPos->x, pPos->y, pPos->z));
        texCoords.push_back(vec2(pTexCoord->x, pTexCoord->y));
    }

    for (unsigned int i = 0; i < paiMeshs->mNumFaces; i++) {
        const aiFace& Face = paiMeshs->mFaces[i];
        meshIndexes.push_back(Face.mIndices[0]);
        meshIndexes.push_back(Face.mIndices[1]);
        meshIndexes.push_back(Face.mIndices[2]);
    }
}

void ComponentMesh::InitBuffers()
{
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCoords.size() * 3, &vertexCoords[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &textureBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texCoords.size() * 2, &texCoords[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * meshIndexes.size(), &meshIndexes[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    WASHI_LOG("All buffers Ready!");
}
