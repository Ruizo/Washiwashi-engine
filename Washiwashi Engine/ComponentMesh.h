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
class Module;

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

	void Debuffering()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void InitFromScene(unsigned int index, const aiMesh* paiMesh);
	void InitMesh(unsigned int index, const aiMesh* paiMesh);
	void InitBuffers(const std::vector<vec3>& vertices, const std::vector<vec2>& textCoords, const std::vector<vec3>& normals, const std::vector<unsigned int>& indices);



public:
	GLuint textureID;
	GLuint vertexBuffer;
	GLuint textureBuffer;
	GLuint indexBuffer;
	GLuint normalBuffer;

	unsigned int numIndexes;

	bool showNormals = false;

private:
	unsigned int materialIndex;
	std::vector<ComponentMesh> meshEntries;
	std::vector<const aiMesh*> activeMeshes;
};
