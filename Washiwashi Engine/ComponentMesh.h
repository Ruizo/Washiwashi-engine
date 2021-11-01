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
	void VertexBuffering()
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
	}

	void IndexBuffering()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	}

	void TextureBuffering()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, textureID);


		if (texCoords.size() > 0)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
			glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		}
	}

	void Debuffering()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (texCoords.size() > 0)  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void InitFromScene(const aiMesh* paiMesh);
	void InitMesh(const aiMesh* paiMesh);
	void InitBuffers();



public:
	GLuint textureID;
	GLuint vertexBuffer;
	GLuint textureBuffer;
	GLuint indexBuffer;

	unsigned int numeshIndexes;

private:
	std::vector<vec2> texCoords;
	std::vector<vec3> vertexCoords;
	std::vector<unsigned int> meshIndexes;
};
