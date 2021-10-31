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

    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexPointer(3, GL_FLOAT, 0, NULL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, textureID);


    if (mTexCoords.size() > 0)
    {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
        glTexCoordPointer(2, GL_FLOAT, 0, NULL);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisableClientState(GL_VERTEX_ARRAY);

    if (mTexCoords.size() > 0)  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glPopMatrix();
}

void ComponentMesh::InitFromScene(const aiMesh* paiMesh)
{
    unsigned int verticesNum = 0;
    unsigned int indicesNum = 0;

    materialIndex = paiMesh->mMaterialIndex;
    mIndices.reserve(paiMesh->mNumFaces * 3);

    verticesNum += paiMesh->mNumVertices;
    indicesNum += mIndices.size();

    mPosition.reserve(verticesNum);
    mTexCoords.reserve(verticesNum);
    mIndices.reserve(indicesNum);

    InitMesh(paiMesh);

    Init();
}

void ComponentMesh::InitMesh(const aiMesh* paiMeshs)
{

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0; i < paiMeshs->mNumVertices; i++) {
        const aiVector3D* pPos = &(paiMeshs->mVertices[i]);
        const aiVector3D* pNormal = &(paiMeshs->mNormals[i]);
        const aiVector3D* pTexCoord = paiMeshs->HasTextureCoords(0) ? &(paiMeshs->mTextureCoords[0][i]) : &Zero3D;

        mPosition.push_back(vec3(pPos->x, pPos->y, pPos->z));
        mTexCoords.push_back(vec2(pTexCoord->x, pTexCoord->y));
    }

    for (unsigned int i = 0; i < paiMeshs->mNumFaces; i++) {
        const aiFace& Face = paiMeshs->mFaces[i];
        mIndices.push_back(Face.mIndices[0]);
        mIndices.push_back(Face.mIndices[1]);
        mIndices.push_back(Face.mIndices[2]);
    }
}

void ComponentMesh::Init()
{
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mPosition.size() * 3, &mPosition[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &textureBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mTexCoords.size() * 2, &mTexCoords[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mIndices.size(), &mIndices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ComponentMesh::Clear()
{
    //for (unsigned int i = 0; i < mTextures.size(); i++) {
    //    SAFE_DELETE(mTextures[i]);
    //}
}
