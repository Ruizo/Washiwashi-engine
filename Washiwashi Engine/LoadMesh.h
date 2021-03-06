#pragma once
#include <vector>
#include <string>
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#pragma comment (lib, "External/Assimp/x86-Release/assimp-vc142-mt.lib")

#include "glew.h"

#include <assimp/Importer.hpp>      // C++ importer interface  // Output data structure
#include "MathGeoLib.h"
#include <gl/GL.h>
#include <gl/GLU.h>

struct Vertex
{
    float3 meshPosition;
    float2 meshTexture;
    float3 meshNormal;

    Vertex() {}

    Vertex(const float3& pos, const float2& tex, const float3& normal)
    {
        meshPosition = pos;
        meshTexture = tex;
        meshNormal = normal;
    }

    Vertex(const float3& pos, const float2& tex)
    {
        meshPosition = pos;
        meshTexture = tex;
        meshNormal = float3(0.0f, 0.0f, 0.0f);
    }
};

class Mesh
{
public:
    Mesh();

    ~Mesh();

    bool LoadMesh(const std::string& Filename);

    void Render();

private:
    bool InitFromScene(const aiScene* pScene, const std::string& Filename);
    void InitMesh(unsigned int Index, const aiMesh* paiMesh);
    //bool InitMaterials(const aiScene* pScene, const std::string& Filename);
    void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF

    struct MeshEntry {
        MeshEntry();

        ~MeshEntry();

        void Init(const std::vector<Vertex>& Vertices,
            const std::vector<unsigned int>& Indices);

        GLuint VB;
        GLuint IB;
        unsigned int NumIndices;
        unsigned int MaterialIndex;
    };

    std::vector<MeshEntry> meshEntries;
    //std::vector<Texture*> meshTextures;
};