#pragma once
#include "GameObject.h"
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "ModuleFileSystem.h"
#include "glew.h"
#include "glmath.h"
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

	void InitMesh(const aiMesh* paiMesh);
	void InitFromScene(const aiMesh* paiMesh);
	void Init();
	void Clear();

public:
	GLuint textureID;
	GLuint vertexBuffer;
	GLuint textureBuffer;
	GLuint indexBuffer;

	GLuint mBuffers[6] = { 0 };

	unsigned int numIndices;
	unsigned int materialIndex;

private:
	std::vector<ComponentMesh> mEntries;
	std::vector<const aiMesh*> activeMeshes;

	std::vector<vec2> mTexCoords;
	std::vector<vec3> mPosition;
	std::vector<unsigned int> mIndices;
};
