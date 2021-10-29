#pragma once
#include "GameObject.h"
#include <string>
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "ModuleFileSystem.h"
#pragma comment (lib, "External/Assimp/x86-Release/assimp-vc142-mt.lib")

#include "glew.h"

#include <assimp/Importer.hpp>      // C++ importer interface  // Output data structure
#include "MathGeoLib.h"
#include <gl/GL.h>
#include <gl/GLU.h>

using namespace std;

class Component;

class ComponentMesh : public Component
{
public:
	ComponentMesh() {}
	ComponentMesh(GameObject* _go);
	virtual ~ComponentMesh();

	void Update() override;

	void UpdateInspector() override;

	void LoadComponentsData(const char*) override;
	
	bool LoadMesh(const std::string& path);
	void Render();

private:
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	void InitFromScene(const aiScene* pScene, const std::string& Filename);
	bool LoadTexture(const std::string& Filename);
	void Init(const std::vector<float3>& Vertices, const std::vector<float2>& textCord,
		const std::vector<unsigned int>& Indices);
	void Clear();

	GLuint meshTextureID;
	GLuint texture;
	GLuint textureID;
	GLubyte checkerImage[640][426][4];
	GLuint VB;
	GLuint TB;
	GLuint IB;
	unsigned int numIndices;
	unsigned int materialIndex;

	std::vector<ComponentMesh> mEntries;
	std::vector<const aiMesh*> activeMeshes;
};
