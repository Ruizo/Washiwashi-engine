#pragma once
#include "GameObject.h"
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "ModuleFileSystem.h"
#include "glew.h"
#include "MathGeoLib.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "External/Assimp/x86-Release/assimp-vc142-mt.lib")
#include <assimp/Importer.hpp>

class Component;

class ComponentMesh : public Component
{
public:
	ComponentMesh() {}
	ComponentMesh(GameObject* _go);
	virtual ~ComponentMesh();

	void Update() override;

	void UpdateInspector() override;
	
	bool LoadMesh(const char* path);

	void Render();

	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	void InitFromScene(const aiScene* pScene, const std::string& Filename);
	void Init(const std::vector<float3>& Vertices, const std::vector<float2>& textCord, const std::vector<unsigned int>& Indices);
	void Clear();

public:
	GLuint textureID;
	GLuint VB;
	GLuint TB;
	GLuint IB;
	unsigned int numIndices;
	unsigned int materialIndex;

private:
	std::vector<ComponentMesh> mEntries;
	std::vector<const aiMesh*> activeMeshes;
};
